# Janus-UAE inventory (Phase 0)

Written on 2026-11-10 by Claude Code for Stevie.
Copyright note: this file is new, so it carries Stevie's copyright only where a header is needed. It has no code.

This document records what is in the repository today. It is a reading of the code, history and remote trees. Nothing here has been built or run. Where a fact is not confirmed, it says so.

## 0. Summary of the main findings

1. `master` is a port of **WinUAE 3.2.0** (`UAEMAJOR 3`, `UAEMINOR 2`, `UAESUBREV 0` in `src/include/options.h`). The latest WinUAE release tag is **6030** (6.0.3, dated 2026-03-03). The gap is about ten years of WinUAE work.
2. The vendor branch method in the spec does not match the real history. There are **no merge commits** on `master`. The `win-uae` branch has **no common ancestor** with `master`. The 3.2.0 import exists only as a tag. Section 2 has the detail.
3. **Janus-UAE 2 (`master`) has none of the Janus host-side integration.** The `janusd`, `clipd` and `launchd` m68k helpers are present in `amiga/`, and `amirun` is in `aros/`. The emulator side that talks to them (`j-cli-thread.c`, `j-mouse.c`, `j-clipboard.c`, `j-screen.c` and others) exists only on `j-uae-1.x`, which is based on E-UAE, not WinUAE. Clipboard code in `master` is stubs. Sound is a stub. Phase 5 is therefore a port of the 1.x integration onto the WinUAE core. It is not a restore.
4. The build is **not x86-only at the top level, but it will not compile for AArch64 today**. `src/include/sysdeps.h` stops with `#error unrecognized CPU type` for AArch64. `od-aros/aros.cpp` reads the x86 `rdtsc` register without a guard. Section 7 lists everything found.
5. The licence situation is mixed. Section 9 has the detail.
6. Native Raspberry Pi AArch64 AROS **is in mainline** (`aros-development-team/AROS`, branch `master`). Section 10 has the evidence.

## 1. Branches and tags

Remotes now configured: `origin` (the fork), `upstream` (`o11i/janus-uae`), `winuae` (`tonioni/WinUAE`).

The fork's history had been fetched shallow at first. It was unshallowed, so all figures below are from full history.

### Branches on `upstream`

| Branch | Commits | Last commit | What it holds |
|---|---|---|---|
| `master` | 1501 | 2026-08-06 | Janus-UAE 2. A port of WinUAE 3.2.0 to AROS, with SDL video and a Zune (MUI) GUI. Identical to `origin/master`. |
| `trunk` | 1501 | 2026-08-06 | Same commit as `master`. An alias. |
| `win-uae` | 3 | 2014-08-28 | Vendor branch. Holds WinUAE 2.8.10 only. It does not hold 3.2.0. |
| `j-uae-1.x` | 653 | 2017-06-19 | Janus-UAE 1.x. Based on E-UAE (itself UAE 0.8.29). Holds the host-side integration code in `src/od-amiga/j-*.c`. |
| `j-uae-1.1` | 466 | 2011-08-30 | Older Janus-UAE 1.1 line. |
| `p-uae` | 8 | 2013-07-31 | P-UAE experiment. |

Branches on `origin` (the fork): `master`, and the working branch `ccr-11844832-soh3di`. Both start at upstream `master` commit `770910d`.

### Branches on `winuae`

`master` (last commit 2026-11-10, 1181 commits after tag 6030) and `Freezer-custom-ROM-support`.

### Tags

- Janus: `j-uae-0.2` to `j-uae-1.4`, `j-uae2-0.1`, `j-uae2-0.1@1443`, `p-uae-0.1`, `p-uae-2013.01.31`, `e-uae-0.8.29-WIP4`, `list`.
- Vendor tags: `win-uae-2.3.1`, `win-uae-2.8.10`, `win-uae-3.2.0`. See section 2. `list` is just a stray tag on the current head.
- WinUAE release tags (from the `winuae` remote): `2100` to `6030`, 42 tags. The numbering is `MmmP`, so 6.0.3 is `6030`. Two oddities exist: `41000` and `41010` mean 4.10.0 and 4.10.1.

WinUAE tags, in order, with dates: 2100 (2010-04), 2200, 2300, 2310, 2320, 2330, 2400, 2410, 2500, 2510, 2600, 2610, 2700, 2800 (2014-05), 2810, 3000 (2014-12), 3100 (2015-06), 3200 (2015-11), 3210, 3220, 3300 (2016-06), 3400, 3500, 3600 (2018-01), 4000 (2018-06), 4010, 4100 (2018-12), 4200, 4300, 4400 (2020-07), 4900 (2021-12), 4910, 41000 (2022-12), 41010 (2023-01), 5200 (2024-02), 5300, 5310 (2024-10), 6000 (2025-07), 6010, 6020, 6030 (2026-03).
Tags `5.0.0` and `5.1.0` also exist on the `winuae` remote (2023-06 and 2023-12). They are not in the numeric scheme. Their relation to `4xxx` and `5200` is not checked.

### Activity by year (commits on `master`)

2009: 195. 2010: 220. 2011: 198. 2012: 28. 2013: 9. 2014: 210. 2015: 337. 2016: 215. 2017: 87. 2019: 1. 2026: 1.
The spec says the code dates "mainly from 2015 to 2019". The history is 2009 to 2017. The 2019 commit is a bulk add of generated GUI headers. The 2026 commit is the GUI fix.

## 2. WinUAE version in `master`, with evidence

**Answer: WinUAE 3.2.0 (tag `3200`, dated 2015-11-11).**

Evidence:

1. `src/include/options.h` on `master` has `UAEMAJOR 3`, `UAEMINOR 2`, `UAESUBREV 0`.
2. `src/od-aros/docs/ReadMe.txt` says "It is a direct port of WinUAE 3.2.0".
3. The tag `win-uae-3.2.0` (commit `4b35dec`, "Import WinUAE 3.2.0 sources", 2015-11-16) has 854 files under `src/`. Compared with the WinUAE tag `3200`, 838 files are identical. The other 16 are all in `src/dosbox/` and differ by one line each. I did not look at what that line is. This is probably a line-ending or header edit made on import.
4. Compared with `win-uae-3.2.0`, `master` has 740 of those 854 files byte-identical, 112 modified and 2 removed. `master` adds 366 files not in the vendor import.
5. `git log` on `master` has 16 commits titled "3.2.0 merge", all on 2015-12-22 (`ce4381d` through `0d4d844`, and others).

Not confirmed: whether 3.2.0 was the vendor import or whether the author also pulled single files from 3.2.1 or 3.2.2. The tags `3210` and `3220` exist. The vendor import differs from `3210` in 34 files.

## 3. How the author merged WinUAE updates

Evidence from `git log`:

- `master` has **0 merge commits**.
- `win-uae` and `master` share **no common ancestor** (`git merge-base` returns nothing).
- The `win-uae` branch has 3 commits (2014-08-18 and 2014-08-28): "WinUAE vendor branch", "Import WinUAE 2.8.10", "Really import WinUAE 2.8.10".
- The tags `win-uae-2.3.1` (commit `b24f9a4`), `win-uae-2.8.10` (`735707b`) and `win-uae-3.2.0` (`4b35dec`) are **not reachable from `master`**. They are separate lines of history.
- The 3.2.0 vendor commit `4b35dec` has no parent.
- Earlier import: `6c75b1c` "Import WinUAE 2.3.1 sources" (2011-05-03) is on `master`, after a "Start clean" commit.
- The 3.2.0 update was done **by hand, file by file**, as a series of commits named "3.2.0 merge" (2015-12-22). Earlier, 2.8.10 was merged the same way: for example "Marged a2065.cpp" (2014-09-01), "Merged audio.cpp" (2014-09-01).
- The history looks converted from SVN. The project readme points to a SourceForge SVN repository. The first commits are dated 2009-03-25 and some commits are marked "grafted".

What this means for Phase 2: a plain `git merge` of a new vendor import into `master` will not work, because git has no base to compare against. There are two sensible ways to fix this. Both need your approval before I do either.

- **Option A (recommended).** Create a new vendor branch in the fork from the real WinUAE tags (tag `3200` first). Join it to `master` once, using an "ours" merge with `--allow-unrelated-histories`, so that git records 3.2.0 as the common base. Later imports (3210, 3220, 3300 and so on) then merge three-way against it. The WinUAE tree has its files at the repository root, but `master` keeps them under `janus-uae/src/`. A subtree-style merge handles that.
- **Option B.** Keep doing it by hand, file by file, as the author did. This is slower and error-prone across about forty releases.

Also note: the spec says never to edit WinUAE source on the `win-uae` branch and never to rewrite `upstream` or `win-uae` history. A new vendor branch in the fork respects both rules. The upstream `win-uae` branch stays untouched.

## 4. Build systems

Two build systems exist side by side.

### 4.1 `mmakefile.src` (AROS "mmake" system)

This is the one that the author actually used for the Janus-UAE 2 work. It needs to sit inside a full AROS source tree under `contrib`.

| File | Role |
|---|---|
| `janus-uae/mmakefile.src` | Top level. Copies `files/` to `$(AROS_CONTRIB)/Emu/J-UAE/` and builds the icons. |
| `src/mmakefile.src` | The emulator. 446 lines. Builds `J-UAE`. Builds host tools. Generates sources. |
| `src/od-aros/gui/mmakefile.src` | Builds `libgui.a` (the Zune GUI). Runs `parse_fixed.pl`. |
| `src/od-aros/gui/png2c/mmakefile.src` | Builds `png2c` (image to C converter). |
| `src/od-aros/SDL_Config/mmakefile.src` | Builds `libSDL_Config.a`. |
| `src/od-aros/libmpeg2/mmakefile.src` | Builds libmpeg2 with `configure`. |
| `src/od-aros/icons/mmakefile.src` | Icons. |
| `amiga/mmakefile.src` | m68k helpers. **The build rules are commented out** (`##%build_prog`). The helpers do not build with this file today. |
| `aros/mmakefile.src` | Builds `amirun`. |

Notes on `src/mmakefile.src`:

- The JIT is enabled only when `AROS_TARGET_CPU` is `x86_64` (`DOJIT := yes`, plus `-D__WORDSIZE=64`). The `i386` line is commented out. For any other CPU the JIT is off. That part already does the right thing for AArch64, but `-D__WORDSIZE=64` is also needed there. Check this in Phase 3.
- It lists source files by name. Two of them do not exist: `sounddep/sound` and `gfx`. The commit "move sounddep stuff one dir up. WIP commit" moved the sound file to `od-aros/sound.cpp`, and `mmakefile.src` was not updated. So **`mmakefile.src` is probably already broken**. The other ~150 listed files all exist. Not confirmed by building.
- It lists `joystick`, `mouse`, `keyboard_aros` and others. Not listed: `od-aros/sound.cpp`, `od-aros/video.cpp`, `od-aros/keyboard.cpp`. The plain `Makefile` lists `video`, `sound` and `keyboard`. The two lists disagree.
- Libraries linked: `png SDL mpeg2convert mpeg2 arossupport z mui lowlevel`, plus the local `libgui` and `libSDL_Config`.
- `PERL:=/usr/bin/perl` is hard-coded.
- Extra compiler flag: `-static-libstdc++`. Exceptions on (`-fexceptions`).

### 4.2 Plain `Makefile` (cross-compile from Linux using an AROS crosstools install)

- `src/Makefile` (148 lines) builds the host tools, then calls `od-aros/Makefile` (403 lines) for the target. `gui/Makefile` and `gui/png2c/Makefile` do the same for their parts.
- Targets: `make x86_64`, `make i386`, `make m68k`. There is **no AArch64 target.**
- Output name: `wuae`, copied into `$(SVN)/$(ABI)/AROS/bin/<ARCH>-<CPU>/AROS/wuae`. The `mmakefile.src` build produces a binary called `J-UAE` instead.
- These Makefiles are older than the mmake files. The generated-files commit says "autogenerated files, but not buildable with the build system so far".

### 4.3 Hard-coded paths

The spec says several Makefiles hard-code `$HOME/projects`. **That is wrong for this tree.** The string `projects` does not appear in any build file. What is actually hard-coded:

| File | Path |
|---|---|
| `src/Makefile` | `SVN = $(HOME)/tc/svn` (64-bit), `$(HOME)/svn` (32-bit), `RELEASEDIR = $(HOME)/tc/releases`, and `SVN="/home/oli/tc/svn"` again at line 128 (this one overrides the first for the `x86_64`, `i386` and `m68k` targets). ABI string `v1.gcc.6.1`. |
| `src/od-aros/gui/Makefile` | `SVN = /home/oli/tc/svn` and `/home/oli/svn` |
| `src/od-aros/gui/png2c/Makefile` | same |
| `aros/Makefile` | `TARGETDIR = /home/oli/svn/aros/AROS/bin/linux-i386/AROS/C/` |
| `amiga/Makefile` | `TARGETDIR = /home/oli/svn/aros/AROS/bin/linux-i386/AROS/m68k/` |
| `src/od-aros/Makefile` | Compiler paths built from `$(SVN)/$(ABI)/AROS/bin/linux-x86_64/tools/crosstools/` (host is fixed to `linux-x86_64`). Two commented-out `/home/oli` lines. |
| `src/mmakefile.src` and `src/od-aros/gui/mmakefile.src` | `PERL:=/usr/bin/perl` |

### 4.4 Perl

- Scripts: `src/od-aros/parse_res.pl` (turns `winuae.rc` and `resource.h` into `string_resource.h`), `src/od-aros/gui/parse_fixed.pl` (turns the rc dialogs into `mui_data.cpp`), and `src/od-aros/parse_gui.pl` (an older version, not used by `mmakefile.src`).
- Modules needed: `Switch` (source filter, removed from the Perl core in 5.14, and known to break on modern Perl), `Tree::Simple`, `Data::TreeDumper`, `List::MoreUtils`. Plus the core modules `File::Basename` and `Fcntl`. `parse_res.pl` uses only `strict`.
- The spec's module list is **correct** and matches the code.
- This machine has Perl 5.38.2. None of the four modules is installed here (checked with `perl -M`). `Switch` on Perl 5.38 is a known risk: `Switch` is a source filter and can fail on newer syntax. I have not tried to install or run it yet.

### 4.5 Generated files, and host-side code generators

All of these must be built with the **host** compiler and run on the build host. They are the main cross-compile concern.

| Tool | Source files | Output | Used by |
|---|---|---|---|
| `build68k` | `build68k.cpp`, `unix_missing.cpp`; reads `table68k` | `cpudefs.cpp` | everything below that needs CPU tables |
| `gencpu` | `gencpu.cpp`, `missing.cpp`, `readcpu.cpp`, generated `cpudefs.cpp` | `cpuemu_0`, `_11`, `_13`, `_20` to `_24`, `_31`, `_32`, `_33`, `_40`, `_50`, and `cpustbl.cpp` (plus a header) | interpreted CPU core |
| `genblitter` | `genblitter.cpp`, `blitops.cpp` | `blit.h`, `blitfunc.h`, `blitfunc.cpp`, `blittable.cpp` | blitter |
| `genlinetoscr` | `genlinetoscr.cpp` | `linetoscr.cpp` | drawing |
| `gencomp` | `jit/gencomp.cpp`, `readcpu.cpp`, generated `cpudefs.cpp`, `unix_missing2.cpp` | `jit/compemu.cpp`, `jit/compstbl.cpp` | x86 JIT only. Not needed when the JIT is off. |
| `png2c` | `od-aros/gui/png2c/` (own Makefile; derived from zapek.com png2c) | `.c` image data | GUI images |
| `parse_res.pl` | Perl | `string_resource.h` | GUI strings |
| `parse_fixed.pl` | Perl | `mui_data.cpp` | GUI dialogs |

Other generator sources exist in `src/` that the `mmakefile.src` does not use: `gengenblitter.cpp`, `genp2c.cpp`, `gencpu_mini.cpp`, `gencpu.oli.cpp`, and `cpuopti.cpp`. I did not look into whether any of them is needed. `gencpu_mini.cpp` and `gencpu.oli.cpp` look like leftovers.

Known weak spots:

- `gencpu` writes its output into the directory it runs in (`cd $(JUAEHOSTTOOLSDIR); ./gencpu`).
- The `src/Makefile` comment says "`cpuemu_11.cpp` and `compstbl.cpp` might fail to be generated on the first run. Run make again." That points to a missing dependency rule.
- `readcpu.cpp` is built twice, once for the host and once for the target. The `Makefile` uses `host_readcpu.o` for the host copy.
- `sysconfig.h`, `tchar.h` and `target.h` are copied from `od-aros/` for the host tools, and `$(AROS_INCLUDES)/aros` is symlinked into the host dir. The host tools are built with `-DCROSS_COMPILE`.
- The commit `0672cba` ("autogenerated files, but not buildable with the build system so far") added 27 generated `.h` files under `od-aros/gui/png2c/`. They are generated image headers and should not be in the repo. Not confirmed whether they are used by any build.

## 5. Files in `src/od-aros/`

Purposes below come from file names, header comments and quick reads. I have not audited the code. "WinUAE-derived" means a replacement for a file from `od-win32`.

### 5.1 Core platform files

| File | Purpose |
|---|---|
| `aros.cpp` | Main platform layer. Replaces `od-win32/win32.cpp`. Start-up, timing, CPU info (contains the x86 `cpuid`/`rdtsc` code). 
| `aros.h` | Platform declarations and Windows-type shims. |
| `main_aros.cpp` | Program entry and start-up for AROS (Oliver Brunner, 2011). |
| `options.cpp` | AROS command line and config options. |
| `parser.cpp` | Parallel port stub code (from UAE). |
| `registry.cpp`, `registry.h` | INI-file replacement for the Windows registry (uses `SDL_Config`). |
| `rp.cpp`, `rp.h` | RetroPlatform interface code carried over from WinUAE. Large (1842 lines). Probably unused on AROS. |
| `stubs.cpp`, `stubs.h` | Stubs for functions not yet implemented. Includes the clipboard stubs. |
| `target.h` | Target-specific definitions, AmigaOS version. |
| `sysconfig.h` | Build configuration (the equivalent of `config.h`). The JIT switch comes from the makefile. |
| `tchar.h` | Unicode mapping layer. |
| `winnt.h` | Windows NT status flags shim. |
| `unicode.cpp` | Dummy Unicode functions. |
| `unrar.h` | Header taken from the unrar.dll package. |
| `thread.cpp`, `thread.h` | Thread and semaphore support for AROS. |
| `threaddep/thread.h` | Thread-dependent definitions (UAE style). |
| `shm.cpp`, `sys/shm.h`, `sys/ipc.h`, `sys/mman.h` | Shared memory shims. |
| `mman.cpp` | `mprotect()` implementation (from the Win32 version). |
| `vm.cpp` | Virtual memory functions for UAE. |
| `memory_aros.cpp` | Memory handling for AROS. Has an `__x86_64__` check. |
| `posixemu.cpp` | POSIX emulation layer used by the file system code. |
| `writelog.cpp` | Logging. |
| `test.cpp`, `sample1.cpp` | Test and sample code. Not in the build lists. |
| `extensions.cpp` | UAE extensions. |
| `machdep/m68k.h`, `maccess.h`, `rpt.h` | Machine-dependent headers. `maccess.h` has a Windows-only x86 assembly path. |
| `md-fpp.h` | FPU helper header with x86 assembly (Windows style). |
| `docs/ReadMe.txt` | Janus-UAE 2 readme (alpha notes). |

### 5.2 Graphics, input, sound, media

| File | Purpose |
|---|---|
| `arosgfx.cpp` | AROS drawing interface (the main video interface code, as `od-win32/win32gfx.cpp` was). |
| `sdlgfx.cpp` | SDL 1.2 video: modes, surfaces, events. Based on E-UAE `sdlgfx.c`. |
| `sdlgfx.h` | Header for the above. |
| `sdlgfx-gl.cpp` | OpenGL version of the above. Does not work on AROS (see commit "Stuff does not work on AROS"). Not built. |
| `sdl_aros.h` | SDL include shim. |
| `sdlkeys_dik.cpp`, `.h` | SDL key codes to DirectInput-style key codes. |
| `gfx.h` | Graphics definitions (RGBFF bit masks). |
| `picasso96_aros.cpp`, `.h` | Picasso96 (RTG) support. Imported from FS-UAE in 2017. 5315 lines. |
| `aros_scaler.cpp`, `scaler_more.cpp` | Software scalers. |
| `screenshot.cpp` | Screenshot support. |
| `video.cpp` | Video code taken from FS-UAE. |
| `libfsemu/video/` | Seven C files from libfsemu (FS-UAE) for video output. Not obviously in the build. |
| `keyboard.cpp`, `keyboard_aros.cpp` | AROS keyboard handling. |
| `keymap.cpp`, `keymap.h` | Key map tables. |
| `hotkeys.h` | Hot key definitions. |
| `mouse.cpp` | Mouse handling. |
| `joystick.cpp` | Joystick via `lowlevel.library`. |
| `sound.cpp`, `sound.h` | **A stub.** 25 lines, defines empty buffers only. No audio output. |
| `cda_play.cpp`, `.h` | CD audio player (Windows-based code, a stub on AROS). |
| `avioutput.cpp` | AVI output stubs. |
| `mp3decoder.h` | MP3 decoder header. |
| `serial.cpp` | Serial port. |

### 5.3 File system, disk and network

| File | Purpose |
|---|---|
| `filesys.cpp` | 66 lines. In WinUAE the equivalent is included from `src/filesys.cpp`. Here it holds `filesys_addexternals` (adds host drives as Amiga units). Not in the `mmakefile.src` list; it is in the plain `Makefile`. |
| `aros_filesys.cpp` | 67 lines. Same comment as `filesys.cpp` (included from `src/filesys.cpp` in WinUAE). Listed in `mmakefile.src`. The two files look like near-copies. Not yet read in detail. |
| `fsdb_aros.cpp` | AROS file system database functions. |
| `fsusage.cpp` | Free-space reporting. |
| `hardfile_aros.cpp` | Hard file access. |
| `aros_uaenet.h` | Network device definitions. |
| `nogui_aros.cpp` | The no-GUI start-up path. |

### 5.4 GUI

| File | Purpose |
|---|---|
| `muigui.cpp`, `.h` | The WinUAE configuration GUI, converted to Zune (MUI). 20377 lines. Adapted from `od-win32/win32gui.cpp`. |
| `muigui_helper.cpp` | Helpers for the above. |
| `parse_res.pl`, `parse_gui.pl` | Perl generators. See 4.4. |
| `gui/` | `libgui`: MUI classes, tree, listview, combo, file request, message box, dialog, window, fixed-position parser, cursor, `leftframe_class.cpp` (the WinUAE-style left tree), and 27 images in `gui/images/`. Plus `gui/png2c/`. |

### 5.5 Other directories

| Path | Purpose |
|---|---|
| `SDL_Config/` | A small INI-file library (SDL_Config, third party). |
| `libmpeg2/` | Bundled libmpeg2 (autotools). Used for CD32 FMV. |
| `include/` | Windows header shims (`direct3d.h`, `dxwrap.h`, `registry.h`, `resource.h`, `win32gfx.h`, `win32gui.h`) and `include/uae/`. |
| `icons/` | Icon sources (`.info.src` and PNG). |
| `7z/` | 35 headers from 7-Zip. No sources. Purpose not checked. |
| `Makefile` | Plain cross-compile Makefile. See 4.2. |

## 6. Integration code (helpers)

### On `master`

| Path | What it is |
|---|---|
| `amiga/janus-daemon.c`, `sync-mouse.c`, `sync-windows.c`, `sync-screens.c`, `public-screen.c`, `lock-window.c`, `patch.c`, `messages.c`, `debug.c` | `janusd` source (m68k). |
| `amiga/clip-daemon.c` | `clipd`. |
| `amiga/launch-daemon.c` | `launchd`. |
| `amiga/include/` | `wbstart` headers. |
| `amiga/old/`, `amiga/test/` | Old and test programs. |
| `aros/amirun.c` | `amirun` for the AROS side. |

Licence of these: GPL v3 or later (see section 9).

### Not on `master`

The AROS-side parts that these helpers talk to are only in `j-uae-1.x`, under `janus-uae/src/od-amiga/`: `j-cli-thread.c` (the "J-UAE Run" message port that `amirun` uses), `j-clipboard.c`, `j-mouse.c`, `j-screen.c`, `j-screen-thread.c`, `j-custom-screen-thread.c`, `j-win-thread.c`, `j-win-thread-gadgets.c`, `j-launch.c`, `j-launch-thread.c`, `j-menu.c`, `j-dispatch.c`, `j-global.c`, `j-protect.c`, `j-reset.c`, `j-splash.c`, `j-bsdsocket-thread.c`, `j-debug.c`. `amirun.c` says: "keep this in sync with j-cli-thread.c".

I searched `master` for `J-UAE Run`, `CLI_PORT_NAME`, commodity code and Janus sync functions. None are present. The only hits are the clipboard stubs in `stubs.cpp`.

I have not read the 1.x code yet. How hard the port to the WinUAE core will be is not known. That is a Phase 5 question, but it may affect the order of phases (see the recommendation in the report).

## 7. x86-specific code

### 7.1 Host-x86 code that blocks an AArch64 build

| Where | What | Action for Phase 3 |
|---|---|---|
| `src/include/sysdeps.h:52-64` | CPU detection ends in `#error unrecognized CPU type`. It knows `x86_64`, `i386`, `arm` (32-bit), `powerpc`, `m68k`. **AArch64 is missing.** | Add an AArch64 case. |
| `src/od-aros/aros.cpp:~1500` (`read_processor_time`) | Inline `cpuid` and `rdtsc`. The `#if` guard is commented out. | Replace with a portable timer (AROS `timer.device` or a generic counter). |
| `src/od-aros/memory_aros.cpp:13` | `os_64bit` set only for `__x86_64__`. | Fix. |
| `src/mmakefile.src` | `-D__WORDSIZE=64` and `DOJIT` only for `x86_64`. | Extend or replace. |
| `src/support/time.cpp` | `rdtsc` path (MSVC assembly, guarded by `X86_MSVC_ASSEMBLY`). | Check the fallback path compiles and works. |
| `src/fpp.cpp` | `fldcw` assembly in two forms (MSVC and GCC), plus MSVC `__asm` blocks at ~lines 997 and 2484. FPU rounding control for the 68881/2 emulation. | Check guards. AArch64 needs a non-x87 path. |
| `src/od-aros/md-fpp.h` | MSVC-style x87 assembly. | Check guards. |
| `src/od-aros/machdep/maccess.h` | Windows-only x86 assembly (`bswap`) with a portable fallback behind a macro. | Probably fine. Confirm. |
| `src/uaenative.cpp` | Calling convention code for native-call from m68k, with `__x86_64__`/`__i386__` branches and one MSVC asm block. | Check what happens on other CPUs. May need disabling. |
| `src/include/uae/attributes.h` | GCC `regparm` for `__i386__`. | Likely fine. |
| `src/include/sysdeps.h:74-77` | `JITCALL` uses `regparm(2)`. | JIT only. |
| `src/od-aros/SDL_Config/SDL_config_cfg.h:170-175` | `int3` breakpoint in asm. | Replace with a no-op or a builtin trap on AArch64. |
| `src/filesys.cpp` | Includes `<aros/asmcall.h>`. Check this exists for AArch64. | Check. |

Not checked: the `#ifdef` `WORDSIZE`, pointer-size and `long` assumptions. A full 64-bit audit is Phase 3 work.

### 7.2 The JIT

- Directory `src/jit/`: about 32,400 lines in total. Files: `codegen_x86.cpp/.h`, `compemu_midfunc_x86.cpp/.h`, `compemu_raw_x86.cpp`, `compemu_support.cpp`, `compemu_support_codegen.cpp`, `compemu_fpp.cpp`, `compemu_prefs.cpp`, `compemu.h`, `flags_x86.h`, `exception_handler.cpp`, `gencomp.cpp`, `readme.txt`.
- It is enabled by `-DJIT` from the makefile only when the target CPU is `x86_64`.
- `newcpu.cpp` has 13 `#if ... JIT` blocks, `main.cpp` 4, `memory.cpp` 1, `cpummu.cpp` 1. These are the points where the JIT leaves the interpreter. With `JIT` undefined, the interpreter alone should build. Not confirmed.
- `src/jit/readme.txt` lists the symbols the JIT objects use from the main executable.

### 7.3 Code that is x86 *emulation*, not host-x86 code

These are not a porting problem for AArch64, because they emulate PC hardware. They only need to compile.

- `src/x86.cpp`, `src/fake86_cpu.cpp` and `src/dosbox/` (13 files in the build, 33 in the directory, including `fpu_instructions_x86.h`). These implement the PC bridgeboard (A2286/A2386).
- `src/ppc/` (PearPC based, 45 files), `src/mame/` (A2410 board, TMS34010), `src/qemuvga/` (Cirrus, NE2000, LSI SCSI and others).
- `src/ppc/pearpc/system/arch/x86` and `x86_64` hold endian headers for those hosts only.

`src/archivers/chd/windows/eivcx86.h` and `winwork.cpp` are Windows x86 files. They are not in the build.

## 8. Libraries the AROS build depends on

| Library | Where it comes from | Confirmed |
|---|---|---|
| SDL 1.2 (`libSDL`, headers in `$(AROS_INCLUDES)/SDL`) | The spec says `contrib`. The `contrib` repo has `SDL/SDL_main/SDL-1.2.15-aros.diff`, which patches SDL 1.2.15. Add-on libs (SDL_image, mixer, net, ttf, gfx, sound) are in `contrib/SDL/`. `contrib` also has `SDL2` and `SDL3`. The emulator uses the 1.2 API (`SDL_SetVideoMode`, `SDL_HWSURFACE`), so SDL2 and SDL3 do not help without a rewrite. | The patch exists. I have not checked that it builds for AArch64. |
| `libpng` (`-lpng`, `<png.h>`) | AROS contrib or ports | Not checked |
| `zlib` (`-lz`, `<zlib.h>`) | AROS | Not checked |
| `libmpeg2`, `libmpeg2convert` | Bundled in `src/od-aros/libmpeg2/`, built by the `mmakefile.src` through `configure` | Bundled |
| `arossupport`, `mui` (Zune), `lowlevel` | AROS core | Present in AROS core by name. Not checked for AArch64. |
| `SDL_Config` | Bundled in `src/od-aros/SDL_Config/` | Bundled |
| libGL, GLUT | Only in `sdlgfx-gl.cpp` and the plain Makefile (`-lGL` for ABI v0). Not needed | |
| libiconv | The commit "somehow the move of libiconv to Extras/Developer in main AROS tree causes problems" suggests an iconv dependency. Not found in the link lists. | Unclear |

## 9. Licence wording

**Short answer: it is mixed, and "GPL v2 only" or "v2 or later" cannot be answered for the project as a whole.**

- `janus-uae/COPYING` is the **GPL version 2** text.
- `od-aros/docs/ReadMe.txt` says Janus-UAE2 "is distributed under the same license as WinUAE, GPLv2".
- But the files written by Oliver Brunner say **GPL version 3 or later** ("either version 3 of the License, or (at your option) any later version"). That covers `amiga/*` (14 files), `aros/amirun.c`, `src/Makefile`, `src/od-aros/Makefile`, and 28 files in `src/od-aros/`.
- The WinUAE core files are mostly **copyright lines only, with no licence text**: for example `newcpu.cpp` ("(c) 1995 Bernd Schmidt") and `custom.cpp`. By my count, 114 of the top-level `src/*.cpp` and `*.h` files have no licence text (80 with a copyright line only, 34 with no header), 110 in `src/include/`, 34 in `src/archivers/` and 120 in `src/prowizard/`. WinUAE states its licence elsewhere (not in these files). I could not find a licence file in the WinUAE tree at tag `6030` (checked `README.md`, no `COPYING`), so I cannot confirm WinUAE's own wording from the source alone.
- Some inherited files say **"version 2 or later"**: for example `src/cpummu30.cpp` ("version 2 or at your option any later version"), `src/cpummu.cpp` (ARAnyM), and most of `src/dosbox/` (36 files) and `src/jit/` (9 files).
- Some say **"version 2 as published" only**, with no "or later": 37 files in `src/ppc/pearpc/`, which is PearPC.
- Some are other licences: LGPL in `src/archivers/chd/sha1.*` (nettle) and `src/qemuvga/` (2 files, LGPL), and permissive code in `archivers/`, `mame/`, `slirp/`.
- `libmpeg2` is GPL (own `COPYING`).

What follows from this, with the usual warning that I am not a lawyer:

- The combined emulator can only be distributed as **GPL v2** while the `ppc/` PearPC files (v2 only) are built in. Dropping `ppc/` from the build, or a licence ruling, could change this. The `ppc/` code is in the build list (`JUAEPPCSRCS`) today.
- Oliver's own files are v3-or-later. That is compatible with distributing under v3, but the whole cannot be v3 while v2-only code is linked in.
- For the "Later work" ARM64 JIT from Amiberry (GPL v3): it needs the receiving code to be "v2 or later". The WinUAE core files that carry no licence text, and the PearPC files, do not give us that. **This question is not settled and I do not think it can be settled from the headers alone.** It needs a licence decision from you or a statement from WinUAE's author. I recommend that nothing from Amiberry is touched until this is decided.

Spec point to correct: the spec says the headers' wording is "not yet confirmed". It is now confirmed to be mixed.

## 10. Target platform findings

All from reading the mainline repositories on 2026-11-10 (shallow, blobless clones of `master`).

### Repositories (Open question 3)

| Repository | URL | Branches | Head on 2026-11-10 |
|---|---|---|---|
| AROS | `https://github.com/aros-development-team/AROS` | `master` only | `df64853` |
| contrib | `https://github.com/aros-development-team/contrib` | `master` only | `3e446a9` |
| ports | `https://github.com/aros-development-team/ports` | `master` only | `a78d725` |

There is a separate fork `deadw00d/AROS` with branches such as `exp-gcc-16`, `exp-multicore` and `alt-abiv0`. I did not look at it.

### The Pi AArch64 port

- The tree has `arch/aarch64-raspi`, `arch/aarch64-native` and `arch/aarch64-all`. The `README.md` build table lists `raspi-aarch64` and `raspi-aarch64-smp`, built with LLVM.
- `configure.in` has a Raspberry Pi target (`r*pi`) with an AArch64 branch. It sets `aros_object_format="aarch64elf_aros"` and `gcc_default_cpu="armv8-a+crc+simd"`, tuned for `cortex-a53`.
- The `ISA_AARCH64_FLAGS` are `-mcmodel=large`, and for LLVM also `--target=aarch64-unknown-aros -fno-pic -fno-pie`.
- The kernel has `platform_bcm2711.c` (Pi 4 and 400) and `platform_bcm2712.c` (Pi 5). `tools/modetest2711` exists under `arch/aarch64-raspi/tools`.
- The only graphics driver listed for the Pi AArch64 is `arch/aarch64-raspi/hidd/fbgfx`, a framebuffer driver.

### Toolchain (Open question 3)

- The mainline defaults are `config/llvm_def` = **11.0.0**, `config/gcc_def` = **6.5.0**, `config/gcc_exp` = **16.2.0**. These are defaults for the whole tree, not for the Pi AArch64 target.
- The Apple Silicon (Macaros) AArch64 branch of `configure.in` sets `default_llvm_version="20.1.0"`. I found **no** such override in the Pi AArch64 branch.
- The nightly build variables are set in the Azure project settings, not in the repository, so I cannot tell from the source which toolchain the Pi nightly uses.
- The spec says third-party software was built with the 20260822 SDK and Clang 20.1.0. I could not confirm that from the source. It agrees with the Macaros default, but Macaros is not the Pi target.
- **Conclusion: not established.** What Phase 3 needs: download the actual Pi AArch64 SDK (nightly or release) and read its `crosstools`. I cannot do that until you tell me which distribution you run on the Pi 400.

### Register x18 (Open question 6)

- I searched `configure.in`, `arch/aarch64-*` and `config/` for `x18`. There are **no** `-ffixed-x18` flags anywhere in the Pi AArch64 path.
- `arch/aarch64-all/exec/execstubs.s` saves and restores `x18` in its stubs, with the comment "AAPCS64 caller-saved set is x0-x18". That treats `x18` as an ordinary register.
- **Conclusion: the native Pi target does not appear to reserve x18.** This is from reading source, not from inspecting a compiled binary. If building with `-ffixed-x18` costs nothing, as the spec allows, I will add it in Phase 3 behind a build variable. I would not rely on it.

### contrib and SDL (Open question 4)

- `contrib/SDL/SDL_main` carries the AROS patch for SDL 1.2.15. Add-on libs exist for 1.2, 2 and 3.
- I have **not** built `contrib` for any target. Whether it builds for the Pi AArch64 target is **not known**. The SDL 1.2 patch is from the m68k/x86/ARM era. It may need 64-bit fixes.
- The Pi AArch64 `README.md` build table only mentions the main `AROS` repository.

### Audio and USB (Open question 5)

- Audio: AHI drivers for the Pi exist in the tree (`workbench/devs/AHI/Drivers/RPiPWM`, `RPiHDMI`, `RPiI2S`). Their build files are `Makefile.in` (the older AHI configure system), not `mmakefile.src`. I did not find evidence that any of them is built for AArch64, and none is for the BCM2711 specifically. **Not known.** Per the spec, if no audio path exists in Phase 6, I stop and ask.
- Separately, the Janus-UAE 2 emulator has no audio code at all (`sound.cpp` is a stub). So audio needs emulator work as well as a working AROS audio path.
- USB: `arch/arm-native/soc/broadcom/2708/usb/usb2otg` is the Pi 1 to 3 USB controller. The Pi 4 and 400 use an xHCI controller on PCIe. The tree has `rom/usb/pcixhci`. **Whether it works on the Pi 400 in the AArch64 port is not known.** Stevie's report that the Pi 400 has run an AArch64 AROS application does not say whether USB keyboard and mouse worked.

## 11. Gaps in this inventory

- No code has been built or run. All build statements are from reading files.
- I did not read the WinUAE `od-win32` source or the 1.x integration code in depth.
- The `od-aros` purposes are one-line summaries from names and headers.
- The SDL, libpng and zlib builds for AArch64 are unchecked.
- The nightly toolchain for the Pi AArch64 target is unchecked.
- I did not check the 16-file difference between `win-uae-3.2.0` and WinUAE tag `3200` beyond seeing that each is a one-line change in `dosbox/`.
