# OpenVi

***OpenVi*** — Portable *OpenBSD* **`vi`** / **`ex`**

## Table of Contents

<!-- toc -->

- [Overview](#overview)
  * [Why?](#why)
    + [Why not?](#why-not)
- [Building](#building)
  * [Prerequisites](#prerequisites)
    + [Required prerequisites](#required-prerequisites)
    + [Optional prerequisites](#optional-prerequisites)
    + [Supported platforms](#supported-platforms)
      - [Unsupported platforms](#unsupported-platforms)
  * [Compilation](#compilation)
    + [Platform Specifics](#platform-specifics)
      - [AIX](#aix)
      - [NetBSD](#netbsd)
      - [illumos](#illumos)
      - [Solaris](#solaris)
- [Availability](#availability)
- [Versioning](#versioning)
- [History](#history)
- [License](#license)
- [Acknowledgements](#acknowledgements)
- [Similar Projects](#similar-projects)
- [See Also](#see-also)

<!-- tocstop -->

## Overview

***OpenVi*** is an enhanced and portable implementation of the Berkeley
**`vi`** / **`ex`** text editor, originally developed by *Bill Joy*.

***OpenVi*** is a fork of the **`vi`** / **`ex`** editor included with
*OpenBSD*, which is derived from version 1.79 of the `nvi` editor originally
distributed as part of the *Fourth Berkeley Software Distribution* (**4BSD**).

The **`nvi`** editor was developed by *Keith Bostic* of the *Computer Systems
Research Group* (**CSRG**) at the *University of California, Berkeley*, *Sven
Verdoolaege*, and other contributors. **`Nvi`** itself was derived from *Steve
Kirkendall*'s **`Elvis`** editor.

### Why?

Why would you want to use ***OpenVi*** instead of ***AnotherVi***?

- Derived from the (extensively audited) *OpenBSD* base system code
  - Focus on readability, simplicity, and correctness of implementation
  - Adherence to *OpenBSD*'s standard secure coding practices
    - Uses secure functions (e.g. `strlcpy`, `snprintf`, `mkstemp`, `pledge`)
  - Reduced complexity for hopefully fewer program defects
- Clean source code, distributed under a permissive 3-clause BSD license
  - Some support code is distributed under the (more permissive) ISC license
- Mostly conforming to relevant standards (*POSIX*, *SUS*), where applicable
  - Enhancements, non-standard behaviors, and new features are conservatively
    and sanely implemented with care taken to balance user expectations,
    complexity, and historical accuracy
- Extensions such as `bserase`, `expandtab`, `imctrl`, `visibletab`, etc.
- Build requires only *GNU Make* and standard POSIX utilities
  - Easy integration with embedded, minimal, or iteratively bootstrapped
    environments and distributions (such as *Linux From Scratch* builds)
- No compile-time or build-time configuration options
  - Single standard build configuration with no incompatible variants
  - No configuration-specific bugs resulting from untested combinations or
    rarely exercised code paths
  - Concise and understandable documentation; no subtle platform variations
- Consistent user interface, script, and map behavior across all platforms
- Utilizes *OpenBSD*'s extended *Spencer*-based regular expression engine
  (also adopted by *LLVM*, *Tcl*, etc.) on all supported systems
- Single, compact, self-contained binary
  - No external data files required at run-time
  - No external library dependencies (other than curses)
    - Suitable for static linking and emergency “rescue” usage
- All the various tweaks, fixes, improvements, and clean-ups accumulated
  over 25+ years as part of the *OpenBSD* base system

#### Why not?

So, why might you **not** want to use ***OpenVi***, then?

Some of these points might be desirable features, depending on your point of
view.

- Internationalization support is currently lacking
  - No support for Unicode / UTF-8 / wide character display
    - Multibyte characters are shown as individual bytes, rather than glyphs
    - Multibyte support is planned, but is unfortunately non-trivial, see:
      - Schwarze, I. (2016, September 25). *Keep multibyte character support
        simple* [Conference presentation]. EuroBSDCon 2016 Convention,
        Belgrade, Serbia.
        [[pdf:OpenBSD](https://openbsd.org/papers/eurobsdcon2016-utf8.pdf)]
      - Jun-ichiro itojun Hagino [*KAME Project*] and Yoshitaka Tokugawa
        [*WIDE Project*]. (1999, 6 June). *Multilingual vi clones: past, now
        and the future* [Conference presentation]. In Proceedings of the
        annual conference on USENIX, Annual Technical Conference
        (*USENIX ATEC '99*). USENIX Association, Monterey, CA, USA, Page 45.
        [[doi:10.5555/1268708.1268753](https://dl.acm.org/doi/10.5555/1268708.1268753)],
        [[abstract:USENIX](https://www.usenix.org/conference/1999-usenix-annual-technical-conference/multilingual-vi-clones-past-now-and-future)]
        (*legacy*)
  - No support for bidirectional text
  - No support for regional localization or message translation
- Inefficient handling of extremely large (e.g. multi-GB) files
- No support for syntax highlighting, context-aware code completion, code
  folding, or “*language server*” integrations
- No interactive macro recording and debugging functionality
- No advanced scripting support (no *BASIC*, *COBOL*, *JavaScript*, *Lua*,
  *Perl*, *PHP*, *Python*, *REXX*, *Ruby*, *S-Lang*, *Tcl*, or anything else)
- Only curses-based visual-mode and line-based `ex`-mode interfaces available
  - No support for X11/Wayland, OpenGL/Vulkan, Neuralink, augmented / virtual
    reality, or any other graphical user interfaces

## Building

### Prerequisites

#### Required prerequisites

- **POSIX.1**-**2008** environment: POSIX shell (`sh`) and utilities, **Awk**
  (`mawk`, `nawk`), etc.
- **GNU Make** (version *3.81* or later)
- **C99** compiler (e.g. `xlc`, `suncc`, `clang`, `gcc`, etc.)
- **Curses** (`ncurses`, *NetBSD* `curses` V8+, `PDCurses` V2.8+,
  `PDCursesMod`, etc.)

#### Optional prerequisites

- **Perl** 5+
- **C shell** (`csh`, `tcsh`, etc.)
- `nroff`, `groff`, etc.

#### Supported platforms

- **OpenVi** is easily portable to most platforms with *UNIX*-like operating
  systems that are mostly conforming to the programming interface described by
  *IEEE Std 1003.1-2008* and user environment described by *IEEE Std
  1003.2-2008*, also known as *POSIX.1-2008* and *POSIX.2-2008*, respectively.

- The following operating systems are fully supported and regularly tested
  using ix86/AMD64, ARM/AArch64, m68k, MIPS, POWER, and RISC-V processors:
  - *IBM* **AIX** 7.1+
  - *Apple* **Darwin** (**macOS** / **Mac OS X**) (*ARM64*, *Intel*, *PowerPC*)
  - **FreeBSD** 12.3+
  - **GNU**/**Linux** distributions (*glibc*, *musl*)
  - *illumos* **OpenIndiana** Hipster
  - **NetBSD** 9+
  - **OpenBSD** 6.9+
  - *Oracle* **Solaris** 11+
  - *Microsoft* **Windows** (*Cygwin*, *Midipix*, *MSYS2*, *WSL*)

- The following compilers are fully supported and regularly tested:
  - *LLVM* **Clang** (*BSD*, *Darwin*, *illumos*, *Linux*, *Solaris*,
    *Windows*) V6+
  - *AMD* **Optimizing C**/**C++** (*Linux*) V3+
  - *GNU* **GCC** (*AIX*, *BSD*, *Darwin*, *illumos*, *Linux*, *Solaris*,
    *Windows*) V4.6+
  - *IBM* **Advance Toolchain** (*Linux on POWER*) V14.0+
  - *IBM* **Open XL C**/**C++** (*AIX*) V17.1+
  - *IBM* **XL C**/**C++** (*AIX*, *Linux*) V16.1+
  - *Intel* **oneAPI DPC++**/**C++** (*Linux*) V2021+
  - *Intel* **C Compiler Classic** (*Darwin*, *Linux*) V19.1+
  - *Oracle* **Developer Studio** (*Linux*, *Solaris*) V12.6+

Newer or older operating system and compiler releases, within reason, should
work. The versions listed above are those regularly tested and known working.

##### Unsupported platforms

- The following platforms are **not** currently supported, but **support is
  planned** for a future release:
  - **Haiku** Walter

User contributions to enhance platform support are welcomed.

### Compilation

- Compilation can be performed by invoking GNU Make (usually `gmake` or `make`)
  from the top-level directory of a source release or git checkout.
- GNU Make's `-j N` flag may be used to parallelize the compilation, where `N`
  is a positive integer representing the number of parallel jobs requested.
- The following environment variables influence compilation and installation:
  - `CC` - C compiler to use
    - (e.g. `CC=gcc`)
  - `OPTLEVEL` - Optimization flags
    - (e.g. `OPTLEVEL=-O2`)
  - `CFLAGS` - Flags to pass to the C compiler
    - (e.g. `CFLAGS="-Wall -pipe"`)
  - `LIBS` - Libraries (overriding defaults) to pass to the linker
    - (e.g. `LIBS="-lpdcurses -lflock"`)
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
- The usual targets (`all`, `strip`, `superstrip`, `clean`, `distclean`,
  `install`, `install-strip`, `uninstall`, `upx`, etc.) are available; review
  the `GNUmakefile` to see all the available targets and options.

For example, to compile an aggressively size-optimized build, enabling
link-time optimization and link-time garbage collection, explicitly using
*GCC*:
```sh
env CC=gcc OPTLEVEL=-Os LGC=1 LTO=1 gmake sstrip
```
or, to verbosely compile a debugging build, explicitly using *Clang*:
```sh
env CC=clang DEBUG=1 V=1 gmake
```
For systems with *GNU Make* as `make` (e.g. *GNU/Linux*), basic compilation
should succeed without any options or additional configuration needed:
```sh
make
```
With the appropriate privileges to manipulate files within the chosen `PREFIX`
(using `doas`, `sudo`, `su`, etc.), the compiled executable may be installed -
as-is or stripped - using an invocation such as:
```sh
doas gmake install-strip
```
or
```sh
sudo env PREFIX=/usr/local make install
```

### Platform Specifics

The following sections document ***only*** platform specific differences, and
are not intended to be a general or exhaustive reference. For installation of
prerequisite software packages or other system configuration, consult the
vendor's documentation.

#### AIX

- Before building ***OpenVi*** on **AIX**, install the `ncurses` libraries and
  headers. *IBM* provides the necessary packages, `ncurses` and
  `ncurses-devel`, in RPM format as part of the *AIX Toolbox for Linux and Open
  Source Software*. With the appropriate permissions (e.g. `root`), these
  packages are installable on most systems using the `dnf` or `yum` utilities,
  for example:
  ```sh
  dnf install ncurses ncurses-devel
  ```
  or
  ```sh
  yum install ncurses ncurses-devel
  ```
  The *IBM* **AIX** base system (and **PASE for i**, an integrated runtime
  environment for **AIX** applications on the **IBM i** operating system)
  provides `libxcurses`, an *XPG4*/*XSI* Extended Curses implementation
  derived from *AT&T System V*, which is **not** yet supported for use with
  ***OpenVi***.

- Compilation is supported using *IBM* **XL C**/**C++** V16.1+ (`gxlc` or
  `xlclang`), *IBM* **Open XL C**/**C++** V17.1+ (`ibm-clang`), or *GNU*
  **GCC** (usually `gcc`, `gcc-8`, `gcc-9`, `gcc-10`, `gcc-11`):
  - Link-time optimization (`LTO=1`) requires **Open XL C**/**C++** V17.1+.
    The *IBM* (*AIX Toolbox*) and *Bull*/*Atos* (*Bull Freeware*) **GCC**
    packages, and *IBM* **XL C**/**C++** versions earlier than V17.1 are
    **not** LTO-enabled.
  - Link-time garbage collection (`LGC=1`) is **not** supported on *IBM*
    **AIX**.
  - A 64-bit build is the default on systems operating in 64-bit mode; for a
    32-bit build, set the value of the `MAIXBITS` environment variable to
    `32` (e.g. `export MAIXBITS=32`).
  - The value of the `CC` environment variable must be set to the full path
    of the compiler (e.g. `/opt/freeware/bin/gcc`,
    `/opt/IBM/xlC/16.1.0/bin/gxlc`, `/opt/IBM/openxlC/17.1.0/bin/ibm-clang`,
    etc.) unless the compiler directory is already part of the current `PATH`.

- File locking (via `flock()` as provided by the **AIX** `libbsd` library) is
  non-functional; this will be investigated and corrected in a future release.

- ***OpenVi*** man pages are authored with `mandoc` and require conversion
  before use with the **AIX** `man` software (which is derived from *AT&T
  UNIX System V*.)

#### NetBSD

- On **NetBSD** installations, the default ***OpenVi*** builds use the BSD
  `curses` library provided by the NetBSD base system. To use `ncurses`
  instead, set the values of the `CFLAGS`, `LDFLAGS`, and `CURSESLIB`
  environment variables appropriately (i.e. `CFLAGS=-I/usr/pkg/include`
  `LDFLAGS=-L/usr/pkg/lib` `CURSESLIB=-lncurses`).

- The *LLVM* **LLD** linker is required for link-time optimization (`LTO=1`)
  using **Clang**. It is available as an installable package (i.e. `pkgin
  install lld`).

#### illumos

- Before building ***OpenVi*** on an **illumos** distribution (i.e.
  **OpenIndiana**), install the `ncurses` libraries and headers. The
  **OpenIndiana** distribution provides the necessary `ncurses` package in IPS
  format. With the appropriate permissions (e.g. `root`), the package can be
  installed using the **OpenIndiana** `pkg` utility, for example:
  ```sh
  pkg install ncurses
  ```
  The **OpenIndiana** base system provides `libcurses`, an *XPG4*/*XSI*
  Extended Curses implementation derived from *AT&T System V*, which is **not**
  yet supported for use with ***OpenVi***.

- Link-time garbage collection (`LGC=1`) is **not** supported on
  **OpenIndiana**.

#### Solaris

- Before building ***OpenVi*** on *Oracle* **Solaris** 11, install the
  `ncurses` libraries and headers. *Oracle* provides provides the necessary
  `ncurses` package for **Solaris** 11 in IPS format. With the appropriate
  permissions (e.g. `root`), the package can be installed using the **Solaris**
  `pkg` utility, for example:
  ```sh
  pkg install ncurses
  ```
  The base *Oracle* **Solaris** system provides `libcurses`, an *XPG4*/*XSI*
  Extended Curses implementation derived from *AT&T System V*, which is **not**
  yet supported for use with ***OpenVi***.

- Compilation is supported using *Oracle* **Developer Studio**, **GCC**, and
  **Clang**:
  - When using *Oracle* **Developer Studio**, invoke the compiler as `suncc`
    or set the value of the `_OSLCC` environment variable to `1`.
  - Link-time optimization (`LTO=1`) is currently supported **only** when using
    **GCC** or **Clang**.
  - Link-time garbage collection (`LGC=1`) is **not** supported on **Solaris**.
  - When using the *Oracle* **Developer Studio** (`suncc`) compiler, the
    default is to build a 64-bit binary on systems operating in 64-bit mode; to
    create a 32-bit, build value of the `SUNBITS` environment variable should
    be set to `32`.

- File locking is unavailable, due to the absence of BSD-style `flock()`
  locking on **Solaris**. This will be addressed by supporting *System V*-style
  `fcntl()` locking in a future release.

## Availability

- [GitHub source repository](https://github.com/johnsonjh/OpenVi)
- [Latest source release](http://github.com/johnsonjh/OpenVi/releases/latest)

## Versioning

The ***OpenVi*** version number is based on the version of the corresponding
*OpenBSD* release, followed by the ***OpenVi*** release number. The `version`
command can be used to display this information in the format shown below.

```text
Version 7.0.1 (OpenVi) 10/25/2021.
```

This message indicates the editor in use is ***OpenVi***, release **1**,
derived from *OpenBSD* version **7.0**, and is fully synchronized with the
*OpenBSD* versions of ***`vi`***, ***`ex`***, ***`db`***, and ***`regex`***
as of **10/25/2021** (*October 25th 2021*).

Changes **not** derived from *OpenBSD* commits do not advance this date.
New *OpenBSD* releases do not reset the ***OpenVi*** release number.

## History

- ***OpenVi***
  - [`ChangeLog`](/ChangeLog)
  - [Release history](http://github.com/johnsonjh/OpenVi/releases/)
  - [Commit history](https://github.com/johnsonjh/OpenVi/commits/master)

- *OpenBSD* ***`vi`***
  - *OpenBSD* ***`vi`*** / ***`ex`***
    - [Commit history](https://github.com/openbsd/src/commits/master/usr.bin/vi)
  - *OpenBSD* ***`db`***
    - [Commit history](https://github.com/openbsd/src/commits/master/lib/libc/db)
  - *OpenBSD* ***`regex`***
    - [Commit history](https://github.com/openbsd/src/commits/master/lib/libc/regex)

## License

- ***OpenVi*** is distributed under the terms of a **3-clause BSD** license.
- See the [`LICENSE.md`](/LICENSE.md) file for the full license and
  distribution terms.

## Acknowledgements

- *rqsd* of *Libera.Chat* for the idea that inspired the project and testing.
- [*S. V. Nickolas*](https://github.com/buricco/),
  [*Jason Stevens*](https://virtuallyfun.com/), and the
  [***Virtually Fun*** *Discord*](https://discord.gg/HMwevcN) community, for
  support and feedback.
- From the original **`vi`** acknowledgements (by *Bill Joy* & *Mark Horton*):
  - *Bruce Englar* encouraged the early development of this display editor.
  - *Peter Kessler* helped bring sanity to version 2's command layout.
  - *Bill Joy* wrote version **1**, versions **2.0** through **2.7**, and
    created the framework that users see in the present editor.
  - *Mark Horton* added macros and other features, and made the editor work on
    a large number of terminals and *UNIX* systems.
  - The financial support of *UUNET Communications Services* is gratefully
    acknowledged.

## Similar Projects

- *Martin Guy*'s [**`Xvi`**](http://martinwguy.github.io/xvi/), an enhanced
  fork of *Tim Thompson*'s [**`STEVIE`**](https://timthompson.com/tjt/stevie/)
- *S. V. Nickolas*'
  [**`Sivle`**](https://github.com/buricco/lunaris/tree/main/src/usr.bin/ex), a
  cleaned-up fork of *Steve Kirkendall*'s
  [**`Elvis`**](http://elvis.the-little-red-haired-girl.org/)
- *Andy Valencia*'s [**`Vim57`**](http://sources.vsta.org:7100/vim57/tree), a
  simplified fork of version 5.7 of *Bram Moolenaar*'s
  [**`Vim`**](https://www.vim.org/)

## See Also

- [*Carsten Kunze*'s **`vi`**](https://github.com/n-t-roff/heirloom-ex-vi/)
  is a currently maintained fork of the original (**1BSD**/**2BSD**) branch
  of the **`vi`** / **`ex`** editor, derived from *Gunnar Ritter*'s enhanced
  version of the [**traditional** **`vi`**](http://ex-vi.sourceforge.net/)
  editor.
- [**`Nvi2`**](https://github.com/lichray/nvi2) is a currently maintained
  *feature branch* of the new (**4BSD**) version of the **`nvi`** / **`nex`**
  editor, with a focus on extensibility and new features.
- [**`Nvi1`**](https://repo.or.cz/nvi.git) (*version* *1.8+*) is the
  currently maintained *traditional branch* of the new (**4BSD**) version of
  the **`nvi`** / **`nex`** editor, now developed by *Sven Verdoolaege*.
