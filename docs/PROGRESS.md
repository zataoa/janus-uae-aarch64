# Progress

Last updated: 2026-11-10.
Working branch: `ccr-11844832-soh3di`.

## Current phase

Phase 3 (native AArch64 build). Started 2026-11-10 at Stevie's request. Phase 1 and 2 skipped for now, see decisions.

## Decisions by Stevie

- 2026-11-10: Gate 0 passed. Phase 5 is a port of the Janus integration from `j-uae-1.x` onto the WinUAE core. The plain-window-only option is rejected.
- 2026-11-10: No testing on hosted AROS or in QEMU. Stevie tests only on the real Pi 400. The hosted AROS build was stopped part-way (toolchain stage) and is not needed for this. Hosted Gate 1 is dropped. Phase order to be confirmed.
- 2026-11-10: Stevie needs a way to get files onto the Pi AROS machine over the network. Method not yet chosen.
- Not yet decided: the vendor branch method for Phase 2 (recommended: new vendor branch from real WinUAE tags, joined to `master` once). Needs approval before Phase 2.

## Done

- Added remotes `upstream` (`o11i/janus-uae`) and `winuae` (`tonioni/WinUAE`). Fetched all branches and tags.
- Unshallowed the fork. Full history is now present.
- Wrote `docs/INVENTORY.md`.
- Checked the mainline AROS, contrib and ports repositories on GitHub for the Pi AArch64 port.
- Put `SPEC.md` in the repo root and corrected the facts that turned out to be wrong.

## Broken (found by reading, not by building)

- `src/mmakefile.src` lists two source files that do not exist (`sounddep/sound`, `gfx`). It probably does not build.
- `amiga/mmakefile.src` has its build rules commented out. The m68k helpers do not build with it.
- `src/include/sysdeps.h` stops with `#error unrecognized CPU type` on AArch64.
- `src/od-aros/aros.cpp` uses x86 `cpuid` and `rdtsc` with no guard.
- `src/Makefile` and the other plain Makefiles hard-code `$(HOME)/tc/svn` and `/home/oli/...`.

## Disabled or missing in Janus-UAE 2 (`master`)

- Sound output: `od-aros/sound.cpp` is a stub.
- Clipboard sharing: stubs only in `od-aros/stubs.cpp`.
- All Janus host-side integration (mouse sync, rootless windows, custom screens, launch, "J-UAE Run" port for `amirun`, commodity): not present. It exists only on the `j-uae-1.x` branch, which is E-UAE based.
- JIT: x86 only. Off for any other CPU.
- OpenGL video (`sdlgfx-gl.cpp`): does not work on AROS and is not built.

## Not yet known

- Whether `contrib` and SDL 1.2 build for Pi AArch64.
- Which toolchain the Pi AArch64 port expects in practice.
- Whether audio and USB input work on the Pi AArch64 port.
- Which perl modules install and work on a modern Perl (`Switch` is a risk).
- How much of the 1.x integration code can be reused on the WinUAE core.

## Next

1. Stevie reads `docs/INVENTORY.md` and approves, or changes, the plan for Phase 1.
2. Phase 1: a known-good build on hosted `linux-x86_64` AROS. Details depend on the answer to the question in the Gate 0 report.

## Phase 3 log

- 2026-11-10: Cross toolchain build for `raspi-aarch64` started in `/home/user/aros-build/build-pi` (GNU: binutils 2.40, gcc 6.5.0, the mainline default). Stevie has not yet said which AROS build is on the Pi, so toolchain match is unconfirmed.
- Source tarballs for isl, gmp, mpfr, mpc come from `github.com/aros-development-team/external-sources` because the usual mirrors are blocked in this sandbox.
- Code changes so far (not compiled for AArch64 yet): `sysdeps.h` knows AArch64; `read_processor_time` and `figure_processor_speed` use timer.device `ReadEClock` on AArch64 instead of x86 `rdtsc`/`cpuid`; `os_64bit` in `memory_aros.cpp`; `brk` instead of `int3` in SDL_Config; `mmakefile.src` sets `-D__WORDSIZE=64` for AArch64 and leaves the JIT off.
- The x87 FPU code in `fpp.cpp` and the native-call code in `uaenative.cpp` are already guarded, so they should drop out. Not confirmed by compiling.
