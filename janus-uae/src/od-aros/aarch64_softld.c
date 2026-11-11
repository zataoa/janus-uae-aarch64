/*
 * 128-bit long double comparisons for AArch64 AROS.
 *
 * Copyright 2026 Stevie (zataoa). New file.
 *
 * This file is part of Janus-UAE.
 *
 * Janus-UAE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Janus-UAE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * The libgcc of the AROS GCC 6.5 AArch64 cross toolchain has no soft-fp
 * routines for binary128 (long double). libstdc++ calls these three for
 * long double parsing and printing. They are weak, so a libgcc that has
 * the real ones wins. The emulator itself uses double, not long double.
 */

#if defined(__aarch64__)

typedef unsigned __int128 u128;

union ld_bits { long double f; u128 u; };

static int ld_is_nan(u128 u)
{
	u128 exp_mask = ((u128)0x7fff) << 112;
	u128 man_mask = (((u128)1) << 112) - 1;
	return ((u & exp_mask) == exp_mask) && (u & man_mask);
}

/* returns -1, 0, 1, or 2 when unordered */
static int ld_cmp(long double a, long double b)
{
	union ld_bits x, y;
	u128 sign = ((u128)1) << 127;
	u128 ma, mb;
	int sa, sb;

	x.f = a;
	y.f = b;
	if (ld_is_nan(x.u) || ld_is_nan(y.u))
		return 2;
	sa = (x.u & sign) != 0;
	sb = (y.u & sign) != 0;
	ma = x.u & ~sign;
	mb = y.u & ~sign;
	if (ma == 0 && mb == 0)
		return 0;
	if (sa != sb)
		return sa ? -1 : 1;
	if (ma == mb)
		return 0;
	if (sa)
		return ma > mb ? -1 : 1;
	return ma > mb ? 1 : -1;
}

__attribute__((weak)) int __eqtf2(long double a, long double b)
{
	int c = ld_cmp(a, b);
	return c == 0 ? 0 : 1;
}

__attribute__((weak)) int __netf2(long double a, long double b)
{
	int c = ld_cmp(a, b);
	return c == 0 ? 0 : 1;
}

__attribute__((weak)) int __gttf2(long double a, long double b)
{
	int c = ld_cmp(a, b);
	if (c == 2)
		return -1;
	return c;
}

#endif
