# OpenVi

**_OpenVi_** — Portable _OpenBSD_ **`vi`** / **`ex`**

## Table of Contents

<!-- toc -->

- [Overview](#overview)
  - [Why?](#why)
    - [Why not?](#why-not)
- [Building](#building)
  - [Prerequisites](#prerequisites)
    - [Required prerequisites](#required-prerequisites)
    - [Optional prerequisites](#optional-prerequisites)
    - [Supported platforms](#supported-platforms)
      - [Unsupported platforms](#unsupported-platforms)
  - [Compilation](#compilation)
- [Availability](#availability)
- [Versioning](#versioning)
- [History](#history)
- [License](#license)
- [Acknowledgements](#acknowledgements)
- [Similar Projects](#similar-projects)
- [See Also](#see-also)

<!-- tocstop -->

## Overview

**_OpenVi_** is an enhanced and portable implementation of the Berkeley
**`vi`** / **`ex`** text editor, originally developed by _Bill Joy_.

**_OpenVi_** is a fork of the **`vi`** / **`ex`** editor included with
_OpenBSD_, which is derived from version 1.79 of the `nvi` editor originally
distributed as part of the _Fourth Berkeley Software Distribution_ (**4BSD**).

The **`nvi`** editor was developed by _Keith Bostic_ of the _Computer Systems
Research Group_ (**CSRG**) at the _University of California, Berkeley_, _Sven
Verdoolaege_, and other contributors. **`Nvi`** itself was derived from _Steve
Kirkendall_'s **`Elvis`** editor.

### Why?

Why would you want to use **_OpenVi_** instead of **_AnotherVi_**?

- Derived from the (extensively audited) _OpenBSD_ base system code
  - Focus on readability, simplicity, and correctness of implementation
  - Adherence to _OpenBSD_'s standard secure coding practices
    - Uses secure functions (e.g. `strlcpy`, `snprintf`, `mkstemp`, `pledge`)
  - Reduced complexity for hopefully fewer program defects
- Clean source code, distributed under a permissive 3-clause BSD license
  - Some support code is distributed under the (more permissive) ISC license
- Mostly conforming to relevant standards (_POSIX_, _SUS_), where applicable
  - Enhancements, non-standard behaviors, and new features are conservatively
    and sanely implemented with care taken to balance user expectations,
    complexity, and historical accuracy
- Extensions such as `bserase`, `expandtab`, `imctrl`, `visibletab`, etc.
- Build requires only _GNU Make_ and standard POSIX utilities
  - Easy integration with embedded, minimal, or iteratively bootstrapped
    environments and distributions (such as _Linux From Scratch_ builds)
- No compile-time or build-time configuration options
  - Single standard build configuration with no incompatible variants
  - No configuration-specific bugs resulting from untested combinations or
    rarely exercised code paths
  - Concise and understandable documentation; no subtle platform variations
- Consistent user interface, script, and map behavior across all platforms
- Utilizes _OpenBSD_'s extended _Spencer_-based regular expression engine
  (also adopted by _LLVM_, _Tcl_, etc.) on all supported systems
- Single, compact, self-contained binary
  - No external data files required at run-time
  - No external library dependencies (other than curses)
    - Suitable for static linking and emergency “rescue” usage
- All the various tweaks, fixes, improvements, and clean-ups accumulated
  over 25+ years as part of the _OpenBSD_ base system

#### Why not?

So, why might you **not** want to use **_OpenVi_**, then?

Some of these points might be desirable features, depending on your point of
view.

- Internationalization support is currently lacking
  - No support for Unicode / UTF-8 / wide character display
    - Multibyte characters are shown as individual bytes, rather than glyphs
    - Multibyte support is planned, but is unfortunately non-trivial, see:
      - Schwarze, I. (2016, September 25). _Keep multibyte character support
        simple_ [Conference presentation]. EuroBSDCon 2016 Convention,
        Belgrade, Serbia.
        [[pdf:OpenBSD](https://openbsd.org/papers/eurobsdcon2016-utf8.pdf)]
      - Jun-ichiro itojun Hagino [*KAME Project*] and Yoshitaka Tokugawa
        [*WIDE Project*]. (1999, 6 June). _Multilingual vi clones: past, now
        and the future_ [Conference presentation]. In Proceedings of the
        annual conference on USENIX, Annual Technical Conference
        (_USENIX ATEC '99_). USENIX Association, Monterey, CA, USA, Page 45.
        [[doi:10.5555/1268708.1268753](https://dl.acm.org/doi/10.5555/1268708.1268753)],
        [[abstract:USENIX](https://www.usenix.org/conference/1999-usenix-annual-technical-conference/multilingual-vi-clones-past-now-and-future)]
        (_legacy_)
  - No support for bidirectional text
  - No support for regional localization or message translation
- Inefficient handling of extremely large (e.g. multi-GB) files
- No support for syntax highlighting, context-aware code completion, code
  folding, or “_language server_” integrations
- No interactive macro recording and debugging functionality
- No advanced scripting support (no _BASIC_, _COBOL_, _JavaScript_, _Lua_,
  _Perl_, _PHP_, _Python_, _REXX_, _Ruby_, _S-Lang_, _Tcl_, or anything else)
- Only curses-based visual-mode and line-based `ex`-mode interfaces available
  - No support for X11/Wayland, OpenGL/Vulkan, Neuralink, augmented / virtual
    reality, or any other graphical user interfaces

## Building

### Prerequisites

#### Required prerequisites

- **POSIX.1**-**2008** environment: POSIX shell (`sh`) and utilities, **Awk**
  (`mawk`, `nawk`), etc.
- **GNU Make** (version _3.81_ or later)
- **C99** compiler (e.g. `xlc`, `suncc`, `clang`, `gcc`, etc.)
- **Curses** (_BSD curses_, `ncurses`, `PDCurses`, `PDCursesMod`, etc.)

#### Optional prerequisites

- **Perl** 5+
- **C shell** (`csh`, `tcsh`, etc.)
- `nroff`, `groff`, etc.

#### Supported platforms

- **OpenVi** is easily portable to most platforms with _UNIX_-like operating
  systems that are mostly conforming to the programming interface described by
  _IEEE Std 1003.1-2008_ and user environment described by _IEEE Std
  1003.2-2008_, also known as _POSIX.1-2008_ and _POSIX.2-2008_, respectively.

- The following operating systems are fully supported and regularly tested
  using ix86/AMD64, ARM/AArch64, m68k, MIPS, POWER, and RISC-V processors:

  - **GNU**/**Linux** (_glibc_ and _musl_)
  - _IBM_ **AIX** 7.2+
  - **FreeBSD**
  - **OpenBSD**
  - **Darwin** / **macOS** / **Mac OS X** (_PowerPC_, _Intel_, _ARM64_)
  - _Microsoft_ **Windows** (_Cygwin_, _Midipix_, _MSYS2_, _WSL_)

- The following compilers are fully supported and regularly tested:
  - **Clang** V6+
  - _GNU_ **GCC** V4.6+
  - _IBM_ **XL C/C++ Compiler** V16.1+
  - _IBM_ **Open XL C/C++ Compiler** V17.1+
  - _Intel_ **C Compiler Classic** V19.1+
  - _Intel_ **oneAPI DPC++/C++ Compiler** V2021+
  - _Oracle_ **Developer Studio** V12.6+

##### Unsupported platforms

- The following platforms are **not** currently supported, but **support is
  planned** for a future release.
  - **Haiku** Walter
  - **NetBSD** 7+
  - _Oracle_ **Solaris** 10/11+
  - _illumos_ **OpenIndiana** Hipster

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
_GCC_:

```sh
env CC=gcc OPTLEVEL=-Os LGC=1 LTO=1 gmake sstrip
```

or, to verbosely compile a debugging build, explicitly using _Clang_:

```sh
env CC=clang DEBUG=1 V=1 gmake
```

For systems with _GNU Make_ as `make` (e.g. _GNU/Linux_), basic compilation
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

## Availability

- [GitHub source repository](https://github.com/johnsonjh/OpenVi)
- [Latest source release](http://github.com/johnsonjh/OpenVi/releases/latest)

## Versioning

The **_OpenVi_** version number is based on the version of the corresponding
_OpenBSD_ release, followed by the **_OpenVi_** release number. The `version`
command can be used to display this information in the format shown below.

```text
Version 7.0.1 (OpenVi) 10/25/2021.
```

This message indicates the editor in use is **_OpenVi_**, release **1**,
derived from _OpenBSD_ version **7.0**, and is fully synchronized with the
_OpenBSD_ versions of **_`vi`_**, **_`ex`_**, **_`db`_**, and **_`regex`_**
as of **10/25/2021** (_October 25th 2021_).

Changes **not** derived from _OpenBSD_ commits do not advance this date.
New _OpenBSD_ releases do not reset the **_OpenVi_** release number.

## History

- **_OpenVi_**

  - [`ChangeLog`](/ChangeLog)
  - [Release history](http://github.com/johnsonjh/OpenVi/releases/)
  - [Commit history](https://github.com/johnsonjh/OpenVi/commits/master)

- _OpenBSD_ **_`vi`_**
  - _OpenBSD_ **_`vi`_** / **_`ex`_**
    - [Commit history](https://github.com/openbsd/src/commits/master/usr.bin/vi)
  - _OpenBSD_ **_`db`_**
    - [Commit history](https://github.com/openbsd/src/commits/master/lib/libc/db)
  - _OpenBSD_ **_`regex`_**
    - [Commit history](https://github.com/openbsd/src/commits/master/lib/libc/regex)

## License

- **_OpenVi_** is distributed under the terms of a **3-clause BSD** license.
- See the [`LICENSE.md`](/LICENSE.md) file for the full license and
  distribution terms.

## Acknowledgements

- _rqsd_ of _Libera.Chat_ for the idea that inspired the project and testing.
- [_S. V. Nickolas_](https://github.com/buricco/),
  [_Jason Stevens_](https://virtuallyfun.com/), and the
  [**_Virtually Fun_** _Discord_](https://discord.gg/HMwevcN) community, for
  support and feedback.
- From the original **`vi`** acknowledgements (by _Bill Joy_ & _Mark Horton_):
  - _Bruce Englar_ encouraged the early development of this display editor.
  - _Peter Kessler_ helped bring sanity to version 2's command layout.
  - _Bill Joy_ wrote version **1**, versions **2.0** through **2.7**, and
    created the framework that users see in the present editor.
  - _Mark Horton_ added macros and other features, and made the editor work on
    a large number of terminals and _UNIX_ systems.
  - The financial support of _UUNET Communications Services_ is gratefully
    acknowledged.

## Similar Projects

- _Martin Guy_'s [**`Xvi`**](http://martinwguy.github.io/xvi/), an enhanced
  fork of _Tim Thompson_'s [**`STEVIE`**](https://timthompson.com/tjt/stevie/)
- _S. V. Nickolas_'
  [**`Sivle`**](https://github.com/buricco/lunaris/tree/main/src/usr.bin/ex), a
  cleaned-up fork of _Steve Kirkendall_'s
  [**`Elvis`**](http://elvis.the-little-red-haired-girl.org/)
- _Andy Valencia_'s [**`Vim57`**](http://sources.vsta.org:7100/vim57/tree), a
  simplified fork of version 5.7 of _Bram Moolenaar_'s
  [**`Vim`**](https://www.vim.org/)

## See Also

- [_Carsten Kunze_'s **`vi`**](https://github.com/n-t-roff/heirloom-ex-vi/)
  is a currently maintained fork of the original (**1BSD**/**2BSD**) branch
  of the **`vi`** / **`ex`** editor, derived from _Gunnar Ritter_'s enhanced
  version of the [**traditional** **`vi`**](http://ex-vi.sourceforge.net/)
  editor.
- [**`Nvi2`**](https://github.com/lichray/nvi2) is a currently maintained
  _feature branch_ of the new (**4BSD**) version of the **`nvi`** / **`nex`**
  editor, with a focus on extensibility and new features.
- [**`Nvi1`**](https://repo.or.cz/nvi.git) (_version_ _1.8+_) is the
  currently maintained _traditional branch_ of the new (**4BSD**) version of
  the **`nvi`** / **`nex`** editor, now developed by _Sven Verdoolaege_.
