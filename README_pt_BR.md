<!-- SPDX-License-Identifier: BSD-3-Clause -->
<!-- Copyright (c) 2021-2022 Jeffrey H Johnson <trnsz@pobox.com> -->
# OpenVi

***OpenVi*** — Portable *OpenBSD* **`vi`** / **`ex`**

## Table of Contents

<!-- toc -->

- [Visão geral](#visão-geral)
  * [Porquê?](#porquê)
    + [Por que não?](#por-que-não?)
- [Construindo](#building)
  * [Pré-requisitos](#pré-requisitos)
    + [Pré-requisitos necessários](#pré-requisitos-necessários)
    + [Pré-requisitos opcionais](#pré-requisitos-opcionais)
    + [Plataformas suportadas](#plataformas-suportadas)
      - [Plataformas não suportadas](#plataformas-não-suportadas)
  * [Compilação](#compilação)
    + [Especificações da plataforma](#especificações-da-plataforma)
      - [AIX](#aix)
      - [NetBSD](#netbsd)
      - [illumos](#illumos)
      - [Solaris](#solaris)
      - [Windows](#windows)
        * [Cygwin](#cygwin)
- [Disponibilidade](#availability)
  * [Códgo fonte](#source-code)
  * [Pacotes](#packages)
- [Versionamento](#versioning)
- [histórico](#history)
- [Licençaa](#license)
- [Reconhecimentos](#acknowledgements)
- [Projetos similares](#similar-projects)
- [Veja também](#see-also)

<!-- tocstop -->

## Visão geral

***OpenVi*** é uma implementação aprimorada e portátil do Berkeley
**`vi`** / **`ex`** editor de texto, originalmente desenvolvido por *Bill Joy*.

***OpenVi*** é um fork do editor **`vi`** / **`ex`** incluído no
*OpenBSD*, que é derivado da versão 1.79 do editor `nvi` originalmente
distribuído como parte da *Fourth Berkeley Software Distribution* (**4BSD**).

O editor **`nvi`** foi desenvolvido por *Keith Bostic* da *Computer Systems
Research Group* (**CSRG**) na *Universidade da Califórnia, Berkeley*, *Sven
Verdoolaege* e outros colaboradores. O próprio **`Nvi`** foi derivado de *Steve
O editor do **`Elvis`** de Kirkendall*.

  ### Porquê?

Por que você deveria usar ***OpenVi*** em vez de ***AnotherVi***?

- Derivado do código do sistema base *OpenBSD* (extensivamente auditado)
  - Foco na legibilidade, simplicidade e correção da implementação
  - Aderência às práticas padrão de codificação segura do *OpenBSD*
    - Usa funções seguras (*por exemplo* `strlcpy`, `snprintf`, `mkstemp`, `pledge`)
  - Complexidade reduzida para menos defeitos de programa
- Código-fonte limpo, distribuído sob uma licença BSD permissiva de 3 cláusulas
  - Código de suporte é distribuído sob a licença ISC (mais permissiva)
- Principalmente em conformidade com os padrões relevantes (*POSIX*, *SUS*), quando aplicável
  - Aprimoramentos, comportamentos fora do padrão e novos recursos são conservadoramente
    e implementados de forma sensata com cuidado para equilibrar as expectativas do usuário,
    complexidade e precisão histórica
- Extensões como `bserase`, `expandtab`, `imctrl`, `visibletab`, etc.
- A compilação requer apenas utilitários *GNU Make* e *POSIX* padrão
  - Fácil integração com bootstrap incorporado, mínimo ou iterativo
    ambientes e distribuições (como compilações *Linux From Scratch*)
- Sem opções de configuração em tempo de compilação ou em tempo de compilação
  - Configuração de compilação padrão única sem variantes incompatíveis
  - Nenhum bug específico de configuração resultante de combinações não testadas ou
    caminhos de código raramente exercitados
  - Documentação concisa e compreensível; sem variações sutis de plataforma
- Interface de usuário, script e comportamento de mapa consistentes em todas as plataformas
- Utiliza o mecanismo de expressão regular estendido baseado em *Spencer* do *OpenBSD*
  (também adotado por *LLVM*, *Tcl*, etc.) em todos os sistemas suportados
- Binário único, compacto e independente
  - Não são necessários arquivos de dados externos em tempo de execução
  - Sem dependências de bibliotecas externas (além de curses)
    - Adequado para ligação estática e uso de “resgate” de emergência
- Todos os vários ajustes, correções, melhorias e limpezas acumuladas
  mais de 25 anos como parte do sistema base *OpenBSD*

#### Por que não?

Então, por que você **não** quer usar o ***OpenVi***?

Alguns desses pontos podem ser características desejáveis, dependendo do seu ponto de vista.

- O suporte à internacionalização está faltando no momento
  - Sem suporte para Unicode / UTF-8 / exibição de caracteres amplos
    - Os caracteres multibyte são mostrados como bytes individuais, em vez de glifos
    - O suporte multibyte está planejado, mas infelizmente não é trivial, veja:
      - Schwarze, I. (2016, 25 de setembro). *Mantenha o suporte a caracteres multibyte
        simples* [Apresentação em conferência]. Convenção EuroBSDCon 2016,
        Belgrado, Sérvia.
        [[pdf:OpenBSD](https://openbsd.org/papers/eurobsdcon2016-utf8.pdf)]
      - Jun-ichiro itojun Hagino [*KAME Project*] e Yoshitaka Tokugawa
        [*Projeto WIDE*]. (1999, 6 de junho). *Clones vi multilíngues: passado, agora
        e o futuro* [Apresentação em conferência]. Em Anais do
        conferência anual sobre USENIX, Conferência Técnica Anual
        (*USENIX ATEC '99*). Associação USENIX, Monterey, CA, EUA, página 45.
        [[doi:10.5555/1268708.1268753](https://dl.acm.org/doi/10.5555/1268708.1268753)],
        [[resumo: USENIX](https://www.usenix.org/conference/1999-usenix-annual-technical-conference/multilingual-vi-clones-past-now-and-future)]
        (*legado*)
  - Sem suporte para texto bidirecional
  - Sem suporte para localização regional ou tradução de mensagens
- Manuseio ineficiente de arquivos extremamente grandes (*por exemplo* multi-GB)
- Sem suporte para realce de sintaxe, conclusão de código com reconhecimento de contexto, código
  integrações de dobramento ou “*servidor de idiomas*”
- Nenhuma funcionalidade interativa de gravação e depuração de macro
- Sem suporte de script avançado (sem *BASIC*, *COBOL*, *JavaScript*, *Lua*,
  *Perl*, *PHP*, *Python*, *REXX*, *Ruby*, *S-Lang*, *Tcl* ou qualquer outro)
- Somente interfaces de modo visual baseado em maldições e modo `ex` baseado em linha disponíveis
  - Sem suporte para X11/Wayland, OpenGL/Vulkan, Neuralink, realidade aumentada/virtual ou qualquer outra interface gráfica do usuário

## Construind

### Pré-requisitos

#### Pré-requisitos necessários

- ambiente **POSIX.1**-**2008**: shell *POSIX* (`sh`) e utilitários,
  **Awk** (`mawk`, `nawk`), etc.
- **GNU Make** (versão *3.81* ou posterior)
- Compilador **C99** (*por exemplo* `xlc`, `suncc`, `clang`, `gcc`, etc.)
- **Curses** (`ncurses`, *NetBSD* `curses` V8+, `PDCurses` V2.8+,
  `PDCursesMod`, etc.)

#### Pré-requisitos opcionais

- **Perl** 5+
- **Cshell** (`csh`, `tcsh`, etc.)
- `nroff`, `groff`, etc.

#### Plataformas suportadas

- **OpenVi** é facilmente portátil para a maioria das plataformas com operação semelhante ao *UNIX*
  sistemas que estão em conformidade com a interface de programação descrita por
  *IEEE Std 1003.1-2008* e ambiente do usuário descrito por *IEEE Std
  1003.2-2008*, também conhecido como *POSIX.1-2008* e *POSIX.2-2008*, respectivamente.

- Os seguintes sistemas operacionais são totalmente suportados e testados regularmente
  usando processadores ix86/AMD64, ARM/AArch64, m68k, MIPS, POWER e RISC-V:
  - *IBM* **AIX** 7.1+
  - *Apple* **Darwin** (**macOS** / **Mac OS X**) (*ARM64*, *Intel*, *PowerPC*)
  - **FreeBSD** 12.3+
  - Distribuições **GNU**/**Linux** (*glibc*, *musl*)
  - *illumos* **OpenIndiana** Hipster
  - **NetBSD** 9+
  - **OpenBSD** 6.9+
  - *OracleSolaris** 11+
  - *Microsoft* **Windows** (*Cygwin*, *Midipix*, *MSYS2*, *WSL*)

- Os seguintes compiladores são totalmente suportados e testados regularmente:
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
  - *PCC* **Compilador C Portátil** (*NetBSD*) V1.0.0+

Versões de sistema operacional e compilador mais recentes ou mais antigas, dentro do razoável, devem
trabalhar. As versões listadas acima são aquelas regularmente testadas e comprovadamente funcionando.

##### Plataformas não suportadas

- As seguintes plataformas **não** são suportadas atualmente, mas **suporte é
  planejado** para um lançamento futuro:
  - **Haicai** Walter
  - *SGI* **IRIX**

Contribuições de usuários para melhorar o suporte da plataforma são bem-vindas.

### Compilação

- A compilação pode ser realizada invocando o GNU Make (geralmente `gmake` ou `make`)
  do diretório de nível superior de uma versão de código-fonte ou git checkout.
- O sinalizador `-j N` do GNU Make pode ser usado para paralelizar a compilação, onde `N`
  é um número inteiro positivo que representa o número de tarefas paralelas solicitadas.
- As seguintes variáveis ​​de ambiente influenciam a compilação e instalação:
  - `CC` - Compilador C a ser usado
    - (*ex.* ​​`CC=gcc`)
  - `OPTLEVEL` - sinalizadores de otimização
    - (*por exemplo* `OPTLEVEL=-O2`)
  - `CFLAGS` - Flags para passar para o compilador C
    - (*por exemplo* `CFLAGS="-Wall -pipe"`)
  - `LIBS` - Bibliotecas (substituindo padrões) para passar para o vinculador
    - (*por exemplo* `LIBS="-lpdcurses -lflock"`)
  - `LDFLAGS` - Flags para passar para o linker
    - (*por exemplo* `LDFLAGS="-L/lib/path -static"`)
  - `V` - Definido para ativar a saída de compilação detalhada
    - (*por exemplo* `V=1`)
  - `DEBUG` - Definido para compilar uma compilação de depuração
    - (*ex.* ​​`DEBUG=1`)
  - `LGC` - Definido para habilitar a coleta de lixo de tempo de link
    - (*por exemplo* `LGC=1`)
  - `LTO` - Definido para ativar a otimização de tempo de link
    - (*por exemplo* `LTO=1`)
  - `EXTRA_LIBS` - Bibliotecas extras para vinculação
    - (*ex.* ​​`EXTRA_LIBS=-lmtmalloc`)
  - `PREFIX` - Prefixo de diretório para uso com alvos `install` e `uninstall`
    - (*ex.* ​​`PREFIX=/opt/OpenVi`)
- Os alvos usuais (`all`, `strip`, `superstrip`, `clean`, `distclean`,
  `install`, `install-strip`, `uninstall`, `upx`, etc.) estão disponíveis; Reveja
  o `GNUmakefile` para ver todos os alvos e opções disponíveis.

Por exemplo, para compilar uma compilação de depuração de tamanho agressivamente otimizado, permitindo
otimização de tempo de link e coleta de lixo de tempo de link, usando explicitamente
*CCG*:
```sh
env CC=gcc OPTLEVEL=-Os LGC=1 LTO=1 gmake sstrip
```
ou, para compilar detalhadamente uma compilação de depuração, explicitamente usando *Clang*:
```sh
env CC=clang DEBUG=1 V=1 gmake
```
Para sistemas com *GNU Make* como `make` (*por exemplo* *GNU/Linux*), compilação básica
deve ser bem-sucedido sem nenhuma opção ou configuração adicional necessária:
```sh
faço
```
Com os privilégios apropriados para manipular arquivos dentro do `PREFIX` escolhido
(usando `doas`, `sudo`, `su`, etc.), o executável compilado pode ser instalado —
como está ou despojado — usando uma invocação como:
```sh
doas gmake install-strip
```
ou
```sh
sudo env PREFIX=/usr/local make install
```

#### Especificações da plataforma

As seções a seguir documentam ***apenas*** diferenças específicas de plataforma e
não pretendem ser uma referência geral ou exaustiva. Para instalação de
pacotes de software de pré-requisito ou outra configuração do sistema, consulte a documentação do fornecedor.

##### AIX

- Antes de compilar o ***OpenVi*** no **AIX**, instale as bibliotecas `ncurses` e
  cabeçalhos. *IBM* fornece os pacotes necessários, `ncurses` e
  `ncurses-devel`, no formato *RPM* como parte do *AIX Toolbox for Linux e
  Software livre*. Com as permissões apropriadas (*por exemplo* `root`), estes
  os pacotes são instaláveis ​​na maioria dos sistemas usando os utilitários `dnf` ou `yum`,
  por exemplo:
  ```sh
  dnf install ncurses ncurses-devel
  ```
  ou
  ```sh
  yum instalar ncurses ncurses-devel
  ```
  O sistema base *IBM* **AIX** (e **PASE for i**, um sistema de tempo de execução integrado
  ambiente para aplicativos **AIX** no sistema operacional **IBM i**)
  fornece `libxcurses`, uma implementação *XPG4*/*XSI* Extended Curses
  derivado do *AT&T System V*, que **ainda não** é suportado para uso com
  ***OpenVi***.

- A compilação é suportada usando *IBM* **XL C**/**C++** V16.1+ (`gxlc` ou
  `xlclang`), *IBM* **Open XL C**/**C++** V17.1+ (`ibm-clang`), ou *GNU*
  **GCC** (geralmente `gcc`, `gcc-8`, `gcc-9`, `gcc-10`, `gcc-11`):
  - Otimização de tempo de link (`LTO=1`) requer **Open XL C**/**C++** V17.1+.
    O *IBM* (*AIX Toolbox*) e *Bull*/*Atos* (*Bull Freeware*) **GCC**
    pacotes e versões *IBM* **XL C**/**C++** anteriores à V17.1 são
    **não** habilitado para LTO.
  - A coleta de lixo em tempo de link (`LGC=1`) **não** é suportada no *IBM*
    **AIX**.
  - Uma compilação de 64 bits é o padrão em sistemas operando no modo de 64 bits; para
    compilação de 32 bits, defina o valor da variável de ambiente `MAIXBITS` para
    `32` (*ex.* ​​`exportar MAIXBITS=32`).
  - O valor da variável de ambiente `CC` deve ser definido como o caminho completo
    do compilador (*por exemplo* `/opt/freeware/bin/gcc`,
    `/opt/IBM/xlC/16.1.0/bin/gxlc`, `/opt/IBM/openxlC/17.1.0/bin/ibm-clang`,
    etc.) a menos que o diretório do compilador já faça parte do `PATH` atual.

- Bloqueio de arquivo (via `flock()` conforme fornecido pela biblioteca `libbsd` do **AIX**) é
  não funcional; isso será investigado e corrigido em uma versão futura.

- As páginas man ***OpenVi*** são criadas com `mandoc` e requerem conversão
  antes de usar com o software `man` do **AIX** (que é derivado do *AT&T
  Sistema UNIX V*.)
  
##### NetBSD

- On **NetBSD** installations, the default ***OpenVi*** builds use the BSD
  `curses` library provided by the NetBSD base system. To use `ncurses`
  instead, set the values of the `CFLAGS`, `LDFLAGS`, and `CURSESLIB`
  environment variables appropriately (*i.e.* `CFLAGS=-I/usr/pkg/include`
  `LDFLAGS=-L/usr/pkg/lib` `CURSESLIB=-lncurses`).

- The *LLVM* **LLD** linker is required for link-time optimization (`LTO=1`)
  using **Clang**. It is available as an installable package (*i.e.* `pkgin
  install lld`).

##### illumos

- Before building ***OpenVi*** on an **illumos** distribution (*i.e.*
  **OpenIndiana**), install the `ncurses` libraries and headers. The
  **OpenIndiana** distribution provides the necessary `ncurses` package in
  *IPS* format. With the appropriate permissions (*e.g.* `root`), the package
  can be installed using the **OpenIndiana** `pkg` utility, for example:
  ```sh
  pkg install ncurses
  ```
  The **OpenIndiana** base system provides `libcurses`, an *XPG4*/*XSI*
  Extended Curses implementation derived from *AT&T System V*, which is **not**
  yet supported for use with ***OpenVi***.

- Link-time garbage collection (`LGC=1`) is **not** supported on
  **OpenIndiana**.

##### Solaris

- Before building ***OpenVi*** on *Oracle* **Solaris** 11, install the
  `ncurses` libraries and headers. *Oracle* provides provides the necessary
  `ncurses` package for **Solaris** 11 in *IPS* format. With the appropriate
  permissions (*e.g.* `root`), the package can be installed using the **Solaris**
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
  - When using the *Oracle* **Developer Studio** (`suncc`) compiler, a 64-bit
    build is the default on systems operating in 64-bit mode; for a 32-bit
    build, set the value of the `SUNBITS` environment variable to `32` (*e.g.*
    `export SUNBITS=32`).

- File locking is unavailable due to the absence of `flock()` on **Solaris**.
  This will be addressed by supporting *System V*-style `fcntl()` locking in a
  future release.

##### Windows

- *Microsoft* **Windows** supports various development and runtime
  environments, including *MSVC*, *Cygwin*, *Midipix*, *MSYS2*, *UWIN*, the
  *Git Bash* environment, and others. Care must be taken to avoid mixing
  incompatible libraries and tools.

###### Cygwin

- Compilation problems in the **Cygwin** environment are often caused by
  incomplete or interrupted package installations, or by the installation of
  packages using non-standard tools (*e.g.* `apt-cyg`), which can result in
  missing files and dangling or missing symbolic links.
- **Before** compiling ***OpenVi*** under **Cygwin**, it is *highly*
  recommended to:
  - Update the **Cygwin** `setup.exe` application to the latest available
    version.
  - Update all installed packages using the new **Cygwin** `setup.exe`
    application.
  - Install the required prerequisite packages (*i.e.* `make`, `gcc`, `ncurses`,
    `ncurses-devel`) using the **Cygwin** `setup.exe` application.
  - Invoke the `cygcheck` utility (*i.e.* `cygcheck -cv | grep -v "OK$"`) to
    verify the integrity of all currently installed packages.

## Availability

### Source Code

- [GitHub source repository](https://github.com/johnsonjh/OpenVi)
- [Latest source release](http://github.com/johnsonjh/OpenVi/releases/latest)

### Packages

**OpenVi** is available to Linux and macOS users via the
[Homebrew](https://brew.sh/) package manager.

```sh
brew install openvi
```

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
