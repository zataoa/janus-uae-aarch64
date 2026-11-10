# Progress

Last updated: 2026-11-10.
Working branch: `ccr-11844832-soh3di`.

## Current phase

Phase 1 (hosted x86_64 baseline). Started 2026-11-10.

## Decisions by Stevie

- 2026-11-10: Gate 0 passed. Phase 5 is a port of the Janus integration from `j-uae-1.x` onto the WinUAE core. The plain-window-only option is rejected.
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
