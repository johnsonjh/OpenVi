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

Why would you want to use ***OpenVi*** instead of **AnotherVi**?

- Derived from the (extensively audited) *OpenBSD* base system code
  - Focus on readability, simplicity, and correctness of implementation
  - Reduced cyclomatic complexity, correlated with fewer program defects
- Clean and permissive 3-clause BSD license
- Mostly conforming to relevant standards (*POSIX*, *SUS*) where applicable
  - Enhancements, non-standard behaviors, and new features are conservatively
    and sanely implemented with care taken to balance user expectations,
    complexity, and historical accuracy
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
  - No external library dependencies required (beyond `curses`)
    - Suitable for static linking and "rescue" usage
- Various tweaks, fixes, improvements, and clean-ups that have accumulated
  over the past 25 years of development as part of the *OpenBSD* base system

#### Why not?

So, why might you **not** want to use ***OpenVi***, then?

Some of these points might be desirable features, depending on your point of
view.

- Internationalization support is currently lacking
  - No support for Unicode / UTF-8 / wide character display
    - Multi-byte characters are shown as individual bytes, rather than glyphs
  - No support for bidirectional text 
  - No support for regional localization or message translation
- Inefficient handling of extremely large (e.g. multi-GB) files
- No support for syntax highlighting, context-aware code completion, code
  folding, or "*language server*" integrations
- No interactive macro recording and debugging functionality
- No advanced scripting support (no *BASIC* / *VBA*, *COBOL*, *JavaScript*,
  *Lua*, *Perl*, *PHP*, *Python*, *REXX*, *Ruby*, *S-Lang*, *Tcl*, or
  anything else)
- Only curses-based visual-mode and line-based `ex`-mode interfaces available
  - No support for X11/Wayland, OpenGL/Vulkan, Neuralink, augmented / virtual
    reality, or any other graphical user interfaces

## Building

### Prerequisites

#### Required prerequisites

- **POSIX** environment: POSIX shell (`sh`) and utilities, **Awk** (`mawk`,
  `nawk`), etc.
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
  - *Microsoft* **Windows** (*WSL*, *Midipix*)

- The following compilers are fully supported and regularly tested:
  - **Clang** V6+
  - *GNU* **GCC** V4.6+
  - *IBM* **XL C/C++ Compiler** V16.1+
  - *Intel* **C Compiler Classic** V19.1+
  - *Intel* **oneAPI DPC++/C++ Compiler** V2021+
  - *Oracle* **Developer Studio** V12.6+

##### Unsupported platforms

- The following platforms are **not** currently supported, but **support is
  planned** for a future release.
  - *IBM* **AIX** 7.2+
  - **Haiku** Walter
  - *Oracle* **Solaris** 10/11+
  - *illumos* **OpenIndiana** Hipster
  - *Microsoft* **Windows** (*Cygwin*, *MSYS2*)

User contributions to enhance platform support are welcomed.

### Compilation

- Compilation can be performed by invoking GNU Make (usually `gmake` or `make`)
  from the top-level directory of a source release or git checkout.
- GNU Make's `-j N` flag may be used to parallelize the compilation, where `N`
  is a positive integer representing the number of parallel jobs requested.
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

The ***OpenVi*** version number is based on the version of the corresponding
*OpenBSD* release, followed by the ***OpenVi*** release number. The `version`
command can be used to display this information in the format shown below.

```text
Version 7.0.1 (OpenVi) 10/25/2021.
```

This message indicates the editor in use is ***OpenVi***, release **1**,
derived from *OpenBSD* version **7.0**, and is fully synchronized with the
*OpenBSD* versions of ***`vi`***, ***`ex`***, and ***`db`*** as of
**10/25/2021** (*October 25th 2021*).

Changes **not** derived from *OpenBSD* commits do not advance this date.

## History

- ***OpenVi***
  - [`ChangeLog`](/ChangeLog)
  - [Release history](http://github.com/johnsonjh/OpenVi/releases/)
  - [Commit history](https://github.com/johnsonjh/OpenVi/commits/master)
- *OpenBSD* ***`vi`*** / ***`ex`***
  - [Commit history](https://github.com/openbsd/src/commits/master/usr.bin/vi)
- *OpenBSD* ***`db`***
  - [Commit history](https://github.com/openbsd/src/commits/master/lib/libc/db)

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
  is the currently maintained fork of the original (**2BSD**) branch of the
  **`vi`** / **`ex`** editor, derived from *Gunnar Ritter*'s enhanced version
  of the [**traditional** **`vi`**](http://ex-vi.sourceforge.net/) editor.
- [**`Nvi2`**](https://github.com/lichray/nvi2) is the currently maintained
  *feature branch* of the new (**4BSD**) version of the **`nvi`** / **`nex`**
  editor, with a focus on extensibility and new features.
- [**`Nvi1`**](https://repo.or.cz/nvi.git) (*version* *1.8+*) is the
  currently maintained *traditional branch* of the new (**4BSD**) version of
  the **`nvi`** / **`nex`** editor, now developed by *Sven Verdoolaege*.
