# Janus-UAE for native AArch64 AROS: project specification

## 1. Purpose

Port Janus-UAE to native AArch64 AROS on the Raspberry Pi, so that classic Amiga (m68k) software runs on AROS with the same integration Janus-UAE provides on other AROS platforms. The emulator core must be brought up to the current WinUAE release as part of the work.

The primary test machine is a Raspberry Pi 400 running native AArch64 AROS.

## 2. Roles

- **Stevie** is the project owner, supervisor and hardware tester. Stevie runs every test on the Pi 400 and approves each milestone.
- **Claude Code** is the developer. Claude Code writes the code, builds it, runs what it can test itself, and prepares exact test steps for Stevie.

## 3. Repositories

- **This repo:** Stevie's fork of `o11i/janus-uae` (GitHub user `zataoa`). All work happens here.
- **Upstream Janus-UAE:** `https://github.com/o11i/janus-uae`. Original author Oliver Brunner. The `master` branch is Janus-UAE 2 (WinUAE-based). Other branches include `win-uae` (vendor branch of untouched WinUAE; the branch holds 2.8.10 only), `j-uae-1.x`, `j-uae-1.1`, `p-uae` and `trunk` (same commit as `master`). The 3.2.0 import exists only as the tag `win-uae-3.2.0`, which is not on any branch and has no common history with `master`. `master` has no merge commits. Add as remote `upstream`. (Corrected after Phase 0.)
- **WinUAE:** `https://github.com/tonioni/WinUAE`. Release tags are numeric (for example `6020`, `6030`). Add as remote `winuae`.
- **AROS:** the mainline AROS source tree, plus its `contrib` and `ports` repositories. Claude Code must confirm the correct current repository locations before cloning, and confirm that the native Raspberry Pi AArch64 port is in mainline and which branch it lives on.
- **Reference only, do not merge without a licence decision:** Amiberry (`https://github.com/BlitterStudio/amiberry`), GPL v3, WinUAE-based, has an ARM64 JIT.

## 4. Known facts about the starting point

- The `master` history runs from 2009 to 2017, with one commit in 2019 and one in 2026. The core is WinUAE 3.2.0 (2015). The most recent upstream commit (August 2026) is a 10-line GUI fix in `src/od-aros/gui/mui_tree.cpp`. (Corrected after Phase 0.)
- Source lives under `janus-uae/`. AROS-specific code is in `janus-uae/src/od-aros/`. There is an `mmakefile.src` for the AROS build system and a plain `Makefile` in `src/`.
- Older build notes say: build inside a full AROS source tree with `contrib` built (libSDL came from contrib); the Makefiles hard-code `$(HOME)/tc/svn`, `$(HOME)/svn` and `/home/oli/...` for the AROS tree (no `$HOME/projects` was found; corrected after Phase 0); the Perl modules `Switch`, `Data::TreeDumper`, `List::MoreUtils` and `Tree::Simple` are required; the target was `make x86_64` for hosted linux-x86_64 AROS; the output binary was named `wuae` by the plain Makefiles and `J-UAE` by `mmakefile.src`. Treat these notes as possibly out of date and verify each one. (Phase 0 checked them; see the corrections above.)
- Integration is split in two:
  - **AmigaOS side (m68k binaries, run inside the emulated Amiga, normally from `S:User-Startup`):** `janusd` (mouse sync, rootless/coherency mode), `clipd` (clipboard sync), `launchd` (start AmigaOS programs from Wanderer). These are m68k code and need no CPU porting, but must be rebuilt or verified if any host interface changes.
  - **AROS side:** the emulator itself plus `amirun`, which starts AmigaOS programs from an AROS shell.
- The licence file is GPL v2. The source headers are mixed (checked in Phase 0, see `docs/INVENTORY.md` section 9): files by the Janus author say GPL v3 or later; most WinUAE core files carry no licence text; some inherited files say v2 or later; the PearPC files in `src/ppc/` say v2 only.
- `master` (Janus-UAE 2) does not contain the emulator side of the Janus integration. That code (`j-cli-thread.c`, `j-mouse.c`, `j-clipboard.c` and others) is only on `j-uae-1.x`, which is E-UAE based. `master` has the m68k helpers in `amiga/`, `amirun` in `aros/`, and clipboard stubs. Sound is also a stub. (Added after Phase 0.)
- The existing JIT is x86 only.

## 5. Target platform facts (verify before relying on them)

- Native AArch64 AROS for the Raspberry Pi exists and uses ABI v1.
- Third-party AArch64 AROS software has been built with the SDK from the 20260822 system distribution and an AROS-patched Clang 20.1.0. AROS news also reports a gcc 16.2.0 patch and LLVM patches forward-ported to 23.1. Claude Code must establish which toolchain the current mainline Pi port expects.
- Other developers have smoke-tested AArch64 AROS binaries under QEMU `raspi3b` using the official AROS Raspberry Pi boot scripts. Real Pi 400 hardware has run at least one AArch64 AROS application.
- Some AROS Pi images lack an LhA command. Release archives must therefore be `.zip`.
- Not known: whether `contrib` (including SDL) builds for the Pi AArch64 target, and whether audio works on the Pi port. Find out early.
- The Macaros hosted Apple Silicon target reserves register x18. That rule is for Macaros, not necessarily for the native Pi target. Do not assume either way: check the native Pi ABI. If x18 reservation costs nothing, building with `-ffixed-x18` is acceptable for future portability.

## 6. Phases and milestones

Work one phase at a time. Each phase ends at a **gate**: Claude Code stops, writes a short report, and gives Stevie the test to run. Do not start the next phase until Stevie approves.

### Phase 0: Inventory

- Clone the fork, add `upstream` and `winuae` remotes, fetch all branches and tags.
- Write `docs/INVENTORY.md` covering:
  - branches and what each contains;
  - the exact WinUAE version currently merged into `master`, with evidence;
  - how the original author merged WinUAE updates (vendor branch history);
  - the build system: `mmakefile.src`, `Makefile`, hard-coded paths, Perl modules, generated files;
  - code generators that must run on the build host when cross-compiling (for example `build68k`, `gencpu`, `genblitter`, `genlinetoscr` and any others found);
  - every file in `src/od-aros/` with a one-line purpose;
  - all x86-specific code (JIT, inline assembly, CPU feature checks);
  - the licence wording in source headers (v2 only, or v2 or later);
  - libraries the AROS build depends on (SDL version, zlib, others).
- **Gate 0:** Stevie reads the inventory and approves the plan for Phase 1.

### Phase 1: Known-good baseline on hosted x86_64 AROS

- Set up an AROS build on a Linux x86_64 host for hosted `linux-x86_64`, ABI v1. Build `contrib`.
- Remove hard-coded `$HOME/projects` paths in favour of a variable or relative paths.
- Build Janus-UAE unchanged apart from build fixes.
- Boot test using the free AROS m68k ROM replacement first, then Stevie's own Kickstart ROM.
- Record exact build steps in `docs/BUILD.md` so the build is repeatable.
- **Gate 1:** Stevie confirms the emulator boots to Workbench on hosted AROS.

### Phase 2: Update to current WinUAE

- Follow the vendor branch method:
  1. Import WinUAE source unchanged onto the `win-uae` branch, one release at a time.
  2. Merge each import into the working branch.
  3. Fix `src/od-aros/` to match changed core interfaces, using WinUAE's own `od-win32` changes as the guide to what moved.
- Step through major WinUAE versions in order (2.8.x, 3.x, 4.x, 5.x, 6.x). Do not jump straight to the latest.
- After each step: build, boot-test with the AROS ROM, commit, tag the step (for example `winuae-merge-4.10`).
- Keep the integration features compiling at every step, even if some are temporarily disabled. Log anything disabled in `docs/PROGRESS.md`.
- Stop and report if a step needs a design decision, for example a GUI rewrite, or a WinUAE subsystem with no sensible AROS equivalent.
- Final target: the latest WinUAE release tag at the time the phase starts.
- **Gate 2 (repeats per major version):** Stevie confirms boot to Workbench and basic use on hosted AROS.

### Phase 3: Native AArch64 build

- Set up the cross toolchain for native Raspberry Pi AArch64 AROS.
- Build host-side code generators with the host compiler, and the emulator with the target compiler. Fix the build system so this split is explicit.
- Build with the interpreted CPU core only. Disable the x86 JIT cleanly at configure or build time.
- Fix 64-bit and alignment issues. AArch64 is little-endian, like x86, so byte-order logic should carry over, but check any code that assumes x86 memory or atomic behaviour.
- First run in QEMU `raspi3b` with native AArch64 AROS, if that is workable. Then hand over to Stevie for the Pi 400.
- Goal: a Kickstart boots in a plain window, with keyboard and mouse working. Integration may stay off for this phase.
- **Gate 3:** Stevie boots Workbench on the Pi 400.

### Phase 4: Storage and media

- ADF floppy images, hard file images, and AROS directories mounted as Amiga volumes.
- Confirm the emulated Amiga can read and write to an AROS directory volume.
- **Gate 4:** Stevie installs and runs a program from an AROS directory volume.

### Phase 5: AROS integration

Restore each integration feature on AArch64 and test it separately:

1. Mouse sync (`janusd`).
2. Rootless/coherency mode: AmigaOS windows appear as AROS windows, including public screens.
3. Custom screens.
4. Clipboard sync both ways (`clipd`).
5. Double-clicking an m68k program in Wanderer runs it in the emulator (`launchd`).
6. `amirun` from an AROS shell, including its documented error codes.
7. Each emulator instance appears as a commodity that Exchange can hide, show and quit.

The original integration goals named WordWorth 6, Organizer 2, Datastore 2 and Money Matters 4 as reference applications. Use any of these Stevie owns as acceptance tests.

- **Gate 5:** Stevie runs at least one productivity application rootless on the Pi 400 desktop.

### Phase 6: Sound, joysticks, polish

- Sound output through whatever audio path the Pi port supports. If none exists, document it and stop: do not write an audio driver without Stevie's approval.
- USB joysticks and gamepads.
- Sensible defaults in `uaerc.config` for a Pi 400.
- **Gate 6:** Stevie tests a game with sound and a joystick.

### Phase 7: Release packaging

- `.zip` archive containing the AArch64 binary, the m68k helpers, a default config, and a plain-English `README` with install steps.
- No Kickstart ROMs in the archive or the repo. The AROS m68k ROM may be included only if its licence allows it; check first.
- Offer the work back to `o11i/janus-uae` as a pull request only when Stevie decides to.
- **Gate 7:** Stevie installs from the archive on a clean AROS Pi image.

### Later work (out of scope unless Stevie approves)

- ARM64 JIT. Amiberry's ARM64 JIT is the obvious source, but Amiberry is GPL v3. Merging it is only possible if the Janus-UAE and WinUAE code it touches is licensed "GPL v2 or later". Settle the licence question first.
- Performance tuning for RTG (Picasso96) screens.
- Support for the Raspberry Pi 4 and 5 beyond the Pi 400.
- Reading real floppies through the DrawBridge driver being developed separately for AROS on the Pi 400.

## 7. Testing rules

- Never commit Kickstart ROMs, Workbench disks or commercial software.
- Use the free AROS m68k ROM for any test Claude Code runs itself.
- Never report a test as passed unless it was actually run. State clearly what was tested in QEMU, what on hosted AROS, and what is waiting for Stevie on the Pi 400.
- For every Pi 400 test, give Stevie: the file to copy, the single action to perform, and what to report back (a screenshot or the exact error text).

## 8. Working rules for Claude Code

- One phase at a time. Stop at every gate.
- Keep `docs/PROGRESS.md` up to date: what is done, what is broken, what is disabled, what is next.
- Small commits with clear messages. Never rewrite `upstream` or `win-uae` history. Never edit WinUAE source on the `win-uae` branch.
- Keep the existing GPL headers and copyright notices. Add Stevie's copyright only to new files.
- If a fact in this spec turns out to be wrong, say so in the next report and correct the spec in a commit.
- Flag uncertainty plainly. Do not guess version numbers, API behaviour or test results.
- Write all documents and reports in British English, short plain sentences, no em-dashes.
- When a decision is needed from Stevie, ask one question at a time, with a recommended option.
- Stevie works in writing only. Never suggest a phone call or a live session.

## 9. Open questions to settle during Phase 0

1. Exact WinUAE version currently in `master`.
2. Licence: GPL v2 only, or v2 or later.
3. Correct mainline AROS repository and branch for the Pi AArch64 port, and its expected toolchain.
4. Whether `contrib` and SDL build for Pi AArch64.
5. Whether the Pi port has working audio and USB input suitable for the emulator.
6. Whether the native Pi target reserves register x18.
