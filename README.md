# OpenVi

**OpenVi** — Portable **OpenBSD** `vi`

<!-- toc -->

- [Overview](#overview)
- [Building](#building)
  * [Prerequisites](#prerequisites)
    + [Required prerequisites](#required-prerequisites)
    + [Optional prerequisites](#optional-prerequisites)
    + [Supported platforms](#supported-platforms)
      - [Unsupported platforms](#unsupported-platforms)
  * [Compilation](#compilation)
- [Availability](#availability)
- [Versioning](#versioning)
- [History](#history)
- [License](#license)
- [Acknowledgements](#acknowledgements)
- [Similar Projects](#similar-projects)
- [See Also](#see-also)

<!-- tocstop -->

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
  - **Darwin** / **macOS** / **Mac OS X** (*PowerPC*, *Intel*, *ARM64*)
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
  - **OS/2** (**eComStation**, **ArcaOS**, etc.)
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

## Versioning

The **OpenVi** version number is based on the version of the corresponding
*OpenBSD* release, followed by the **OpenVi** release number. The `version`
command can be used to display this information in the format shown below.
```text
Version 7.0.1 (OpenVi) 10/25/2021.
```
This message indicates the editor in use is **OpenVi**, release **1**, derived
from *OpenBSD* version **7.0**, and is fully synchronized with *OpenBSD*'s
`vi`, `ex`, and `db` code as of **10/25/2021** (*October 25th 2021*).  Changes
which are **not** derived from *OpenBSD* commits do not advance this date.

## History

- [ChangeLog](/ChangeLog)
- [GitHub release history](http://github.com/johnsonjh/OpenVi/releases/)
- [GitHub commit history](https://github.com/johnsonjh/OpenVi/commits/master)

## License

- See the [LICENSE](/LICENSE) file for important redistribution information.

## Acknowledgements

- From the original `vi` acknowledgements by *Bill Joy* and *Mark Horton*:
  - *Bruce Englar* encouraged the early development of this display editor.
  - *Peter Kessler* helped bring sanity to version 2's command layout.
  - *Bill Joy* wrote version 1, versions 2.0 through 2.7, and created the
    framework that users see in the present editor.
  - *Mark Horton* added macros and other features, and made the editor work
    on a large number of terminals and UNIX systems.

## Similar Projects

- [Xvi](http://martinwguy.github.io/xvi/), an enhanced fork of
  [Stevie](https://timthompson.com/tjt/stevie/)
- [Sivle](https://github.com/buricco/lunaris/tree/main/src/usr.bin/ex), a
  cleaned-up fork of [Elvis](http://elvis.the-little-red-haired-girl.org)
- [Andy's Vim](sources.vsta.org:7100/vim57/tree), a simplified fork of
  [Vim](https://www.vim.org).

## See Also

- [Traditional vi](https://github.com/n-t-roff/heirloom-ex-vi) is the currently
  maintained version of the original (2BSD) branch of the `vi` / `ex` editor.
- [Nvi2](https://github.com/lichray/nvi2) is the currently maintained *feature*
  *branch* of the 4BSD version of the `nvi` / `nex` editor, with a focus on
  extensibility and new features.
