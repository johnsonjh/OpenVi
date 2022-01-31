# OpenVi

**OpenVi** — Portable **OpenBSD** `vi`

## Overview

**OpenVi** is an enhanced and portable implementation of the Berkeley
`vi`/`ex` text editor, originally developed by *Bill Joy*.

**OpenVi** is a fork of the `vi` editor included with **OpenBSD**,
which is derived from the `nvi` editor originally distributed as
part of the *Fourth Berkeley Software Distribution* (**4BSD**). The
`nvi` was editor developed by *Keith Bostic* of the *Computer
Systems Research Group* (**CSRG**) at the *University of
California, Berkeley*, *Sven Verdoolaege*, and other contributors.
`Nvi` itself was derived from *Steve Kirkendall*'s `elvis` editor.

## Building

### Prerequisites

#### Required prerequisites

- **POSIX** environment: Bourne shell (`sh`), Awk (`mawk, `nawk`), etc.
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
  - Linux (*glibc* and *musl*)
  - FreeBSD
  - OpenBSD
  - Darwin / macOS / MacOS X (*PowerPC*, *Intel*, *ARM64*)

- The following compilers are fully supported and regularly tested:
  - **Clang** 6+
  - **GNU GCC** 7.5+
  - **IBM XL C/C++ Compiler** 16.1+
  - **Intel C Compiler Classic** 19.1+
  - **Intel oneAPI DPC++/C++ Compiler** 2021+
  - **Oracle Developer Studio** 12.6+

### Compilation

- Compilation can be performed by invoking GNU Make (usually `gmake` or
  `make`) from the top-level directory of a source release or git checkout.
- GNU Make's `-j N` flag may be used to parallelize the compilation, where
  `N` is a positive integer representing the number of jobs requested.
- The following environment variables influence compilation and installation:
  - `CC` - C compiler to use for the compilation
  - `CFLAGS` - Flags to pass to the C compiler
  - `LDFLAGS` - Flags to pass to the linker
  - `V` - Set to enable verbose compilation output
  - `DEBUG` - Set to compile a debugging build; also disables optimizations
  - `LGC` - Set to enable link-time garbage collection
  - `LTO` - Set to enable link-time optimization
  - `EXTRA\_LIBS` - Extra libraries requested when linking
  - `PREFIX` - Directory prefix for install and uninstall targets
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
