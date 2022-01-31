# OpenVi

**OpenVi** — Portable **OpenBSD** `vi`

## Overview

**OpenVi** is an enhanced and portable implementation of the Berkeley
`vi` / `ex` text editor, originally developed by *Bill Joy*.

**OpenVi** is a fork of the `vi` editor included with **OpenBSD**,
which is derived from the `nvi` editor originally distributed as
part of the *Fourth Berkeley Software Distribution* (**4BSD**). The
`nvi` editor was developed by *Keith Bostic* of the *Computer
Systems Research Group* (**CSRG**) at the *University of
California, Berkeley*, *Sven Verdoolaege*, and other contributors.
`Nvi` itself was derived from *Steve Kirkendall*'s `elvis` editor.

## Building

### Prerequisites

#### Required prerequisites

- **POSIX** environment: POSIX shell (`sh`) and utilities, Awk (`mawk`, `nawk`), etc.
- **GNU Make** (version *3.81* or later)
- **C99** compiler (e.g. `xlc`, `suncc`, `clang`, `gcc`, etc.)
- **Curses** (*BSD curses*, `ncurses`, `PDCurses`, `PDCursesMod`, etc.)

#### Optional prerequisites

- **Perl** 5+
- **C shell** (`csh`, `tcsh`, etc.)
- `nroff`, `groff`, etc.

#### Supported platforms

- **OpenVi** is easily portable to most platforms with *UNIX*-like operating
  systems that are mostly conforming to the programming interface and user
  environment described by *IEEE Std 1003.1*, also known as *POSIX.1*.

- The following operating systems are fully supported and regularly tested
  using ix86/AMD64, ARM/AArch64, m68k, MIPS, POWER, and RISC-V processors:
  - **Linux** (*glibc* and *musl*)
  - **FreeBSD**
  - **OpenBSD**
  - **Darwin** / **macOS** / **MacOS X** (*PowerPC*, *Intel*, *ARM64*)
  - *Microsoft* **Windows** (*WSL2*)

- The following compilers are fully supported and regularly tested:
  - **Clang** V6+
  - *GNU* **GCC** V7.5+
  - *IBM* **XL C/C++ Compiler** V16.1+
  - *Intel* **C Compiler Classic** V19.1+
  - *Intel* **oneAPI DPC++/C++ Compiler** V2021+
  - *Oracle* **Developer Studio** V12.6+

##### Unsupported platforms

- The following platforms are not currently supported, but **support is planned**.
  - *IBM* **AIX** 7.2+
  - **Haiku** Walter
  - *Oracle* **Solaris** 10/11+
  - *illumos* **OpenIndiana** Hipster
  - *Microsoft* **Windows** (*Cygwin*, *MSYS2*)

- The following platforms are not currently supported, and **no support is planned**.
  - **AROS** / **AmigaOS** 3/4+
  - **EmuTOS** / **TOS**
  - *VSI* / *HP* / *Compaq* / *DEC* **OpenVMS** 8/9+
  - *Microsoft* **Windows** (*MinGW*, *MSVC*, *UCRT*)

User contributions to enhance platform support are welcomed.

### Compilation

- Compilation can be performed by invoking GNU Make (usually `gmake` or
  `make`) from the top-level directory of a source release or git checkout.
- GNU Make's `-j N` flag may be used to parallelize the compilation, where
  `N` is a positive integer representing the number of jobs requested.
- The following environment variables influence compilation and installation:
  - `CC` - C compiler to use
    - (e.g. `CC=gcc`)
  - `OPTFLAGS` - Optimization flags
    - (e.g. `OPTFLAGS=-O2`)
  - `CFLAGS` - Flags to pass to the C compiler
    - (e.g. `CFLAGS="-Wall -pipe"`)
  - `LDFLAGS` - Flags to pass to the linker
    - (e.g. `LDFLAGS="-L/lib/path -static"`)
  - `V` - Set to enable verbose compilation output
    - (e.g. `V=1`)
  - `DEBUG` - Set to compile a debugging build
    - (e.g. `DEBUG=1`)
  - `LGC` - Set to enable link-time garbage collection
    - (e.g. `LGC=1`)
  - `LTO` - Set to enable link-time optimization
    - (e.g. `LTO=1`)
  - `EXTRA_LIBS` - Extra libraries for linking
    - (e.g. `EXTRA_LIBS=-lmtmalloc`)
  - `PREFIX` - Directory prefix for use with install and uninstall targets
    - (e.g. `PREFIX=/opt/OpenVi`)
- The usual targets (`all`, `clean`, `install`, `uninstall`) are available;
  review the `GNUmakefile` to see all the available targets and options.

## Availability

- [GitHub source repository](https://github.com/johnsonjh/OpenVi)
- [Latest source release](http://github.com/johnsonjh/OpenVi/releases/latest)

## History

- [ChangeLog](/ChangeLog)
- [GitHub release history](http://github.com/johnsonjh/OpenVi/releases/)
- [GitHub commit history](https://github.com/johnsonjh/OpenVi/commits/master)

## License

- See the [LICENSE](/LICENSE) file for important redistribution information.
