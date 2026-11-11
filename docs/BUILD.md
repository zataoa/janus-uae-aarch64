# Building Janus-UAE for native Raspberry Pi AArch64 AROS

Written 2026-11-11. These are the steps that worked in the cloud build machine (Ubuntu, x86_64).
The result has been compiled and linked. It has not been run yet.

## 1. Host packages

    apt-get install build-essential flex bison gawk netpbm libpng-dev zlib1g-dev \
      gperf texinfo patch wget autoconf automake libtool-bin libmpc-dev libmpfr-dev \
      libgmp-dev xz-utils bzip2 unzip subversion libsdl1.2-dev python3-venv \
      libswitch-perl libtree-simple-perl libdata-treedumper-perl liblist-moreutils-perl

The four Perl modules the spec lists are the right ones. `Switch` loads and works with Perl 5.38.

AROS configure also needs the Python modules `mako` and `pyyaml`. Put them in a virtual environment and put that first in `PATH`:

    python3 -m venv venv && venv/bin/pip install mako pyyaml
    export PATH=$PWD/venv/bin:$PATH

## 2. AROS source and cross toolchain

Mainline repositories (branch `master` in each):

- https://github.com/aros-development-team/AROS
- https://github.com/aros-development-team/contrib
- https://github.com/aros-development-team/ports

    mkdir build-pi && cd build-pi
    ../AROS/configure --target=raspi-aarch64 --with-portssources=<dir for downloaded sources>
    make -j4 crosstools

This builds binutils 2.40 and gcc 6.5.0 (the mainline defaults for this target). It takes about an hour on 4 cores.

### Blocked downloads

In the cloud sandbox most source mirrors are blocked. The fixes used:

- isl, gmp, mpfr, mpc, zlib and other sources: copy them from `https://github.com/aros-development-team/external-sources` into the ports sources directory.
- libssh 0.12.2: `git archive` of tag `libssh-0.12.2` from `https://github.com/libssh/libssh-mirror`, packed as `libssh-0.12.2.tar.xz`.
- Boost 1.89: not reachable. Ubuntu's Boost 1.88 source was used, with the headers merged into one `boost/` folder and repacked as `boost_1_89_0.tar.gz`. The AROS patch for Boost was dropped. Only the preprocessor headers are needed.
- SDL 1.2.15: `libsdl1.2_1.2.15.orig.tar.gz` from the Ubuntu archive, saved as `SDL-1.2.15.tar.gz`.
- Mesa and GL: not reachable. The GL link library was removed from the `linklibs` list in the local AROS checkout (`workbench/libs/gl/mmakefile.src`) and Mesa was removed from `ports-includes`.

On a machine with normal internet access these steps should not be needed. They are listed so the build can be repeated here.

## 3. Libraries from AROS

After the toolchain, build the libraries the emulator links:

    make -j4 workbench-libs-lowlevel workbench-libs-z workbench-libs-png

`libmui.a` and `libarossupport.a` come from the main `make`.

### SDL 1.2 (static)

The mmake route for SDL pulls in many optional downloads, so SDL was compiled directly with the cross compiler. In the unpacked, patched SDL tree:

- In `include/SDL_config_aros.h`, comment out `HAVE_ICONV_H` and `HAVE_ICONV` (no iconv in this SDK).
- Compile the files listed in `FILES` in `contrib/SDL/SDL_main/mmakefile.src` with `aarch64-aros-gcc -O2 -mcmodel=large -std=gnu99`, adding include paths for the copied GL headers (`gl.h`, `glext.h`), the AHI headers (`workbench/devs/AHI/Include/C`) and the SDK.
- Do not compile `aros_init.c` (that is for the shared library).
- Archive the objects as `AROS/Developer/lib/libSDL.a` and copy `include/*.h` to `AROS/Developer/include/SDL/`.

The Pi image may or may not have an SDL library. Linking SDL statically avoids needing one.

## 4. Janus-UAE

    cd janus-uae/src
    make -f Makefile.cross AROS_BUILD=<path to build-pi> -j4

`Makefile.cross` does this:

- Host compiler: `build68k`, `gencpu`, `genblitter`, `genlinetoscr`, and the two Perl generators for the GUI.
- AArch64 AROS compiler: everything else, including `libgui`, `libSDL_Config` and a small libmpeg2.
- Output: `build-aarch64/J-UAE`.

Interpreted CPU core only. The x86 JIT is not built.

The older `Makefile` and `mmakefile.src` build files are left as they were, apart from removing the hard-coded paths. They are not used for the AArch64 build.

## 5. Known toolchain gaps

- The GCC 6.5 AROS AArch64 `libgcc` has no binary128 (`long double`) comparison routines. `od-aros/aarch64_softld.c` supplies weak versions.
- SDL's OpenGL entry points come from Mesa. `od-aros/sdl_gl_stubs.c` supplies empty ones.
