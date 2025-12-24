<!-- SPDX-License-Identifier: BSD-3-Clause -->
<!-- Copyright (c) 2021-2024 Jeffrey H Johnson -->
[An English version of this README is available.](README.md)

# OpenVi

***OpenVi*** — Portable *OpenBSD* **`vi`** / **`ex`**

## Índice

<!-- toc -->

- [Visão geral](#visão-geral)
  * [Porquê?](#porquê)
    + [Por que não?](#por-que-não)
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
- [Disponibilidade](#disponibilidade)
  * [Códgo fonte](#código-fonte)
  * [Pacotes](#pacotes)
- [Versionamento](#versionamento)
- [Histórico](#histórico)
- [Licença](#licença)
- [Agradecimentos](#agradecimentos)
- [Projetos similares](#projetos-similares)
- [Veja também](#veja-também)

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

- **pkg-config**
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
  - **Managarm**

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
  - *PCC* **Portable C Compiler** (*NetBSD*) V1.0.0+

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
make
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
  yum install ncurses ncurses-devel
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
    `32` (*ex.* ​​`export MAIXBITS=32`).
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

- Nas instalações do **NetBSD**, as compilações padrão do ***OpenVi*** usam a biblioteca BSD `curses`
  fornecida pelo sistema base do NetBSD. Para usar `ncurses`, defina
  os valores de `CFLAGS`, `LDFLAGS` e `CURSESLIB`
  variáveis ​​de ambiente apropriadamente (*isto é* `CFLAGS=-I/usr/pkg/include`
  `LDFLAGS=-L/usr/pkg/lib` `CURSESLIB=-lncurses`).

- O linker *LLVM* **LLD** é necessário para otimização de tempo de link (`LTO=1`)
  usando **Clang**. Está disponível como um pacote instalável (*i.e.* `pkgin
  instalar lld`).

##### illumos

- Antes de compilar o ***OpenVi*** em uma distribuição **illumos** (*i.e.*
  **OpenIndiana**), instale as bibliotecas e cabeçalhos `ncurses`.
  A distribuição **OpenIndiana** fornece o pacote `ncurses` necessário no formato *IPS*.
  Com as permissões apropriadas (*por exemplo*       `root`), o pacote
  pode ser instalado usando o utilitário `pkg` **OpenIndiana**, por exemplo:
  ```sh
  pkg install ncurses
  ```
  O sistema base **OpenIndiana** fornece `libcurses`, um *XPG4*/*XSI*
  Implementação Extended Curses derivada do *AT&T System V*, que **não**
  ainda suportado para uso com ***OpenVi***.

- A coleta de lixo em tempo de link (`LGC=1`) **não** é suportada em
  **OpenIndiana**.

##### Solaris

- Antes de compilar o ***OpenVi*** no *Oracle* **Solaris** 11, instale as
  bibliotecas e cabeçalhos `ncurses`. *Oracle* fornece o pacote
  `ncurses` necessário para **Solaris** 11 no formato *IPS*. Com as permissões
  apropriadas (*por exemplo* `root`), o pacote pode ser instalado usando o utilitário
  **Solaris** `pkg`, por exemplo:
  ```sh
  pkg install ncurses
  ```
  O sistema base *Oracle* **Solaris** fornece `libcurses`, uma *XPG4*/*XSI*
  Implementação Extended Curses derivada do *AT&T System V*, que ainda **não**
  não é suportado para uso com ***OpenVi***.

- A compilação é suportada usando *Oracle* **Developer Studio**, **GCC** e
  **Clang**:
  - Ao usar *Oracle* **Developer Studio**, invoque o compilador como `suncc`
    ou defina o valor da variável de ambiente `_OSLCC` para `1`.
  - A otimização de tempo de link (`LTO=1`) é atualmente suportada **apenas** ao usar
    **GCC** ou **Clang**.
  - A coleta de lixo em tempo de link (`LGC=1`) **não** é suportada no **Solaris**.
  - Ao usar o compilador *Oracle* **Developer Studio** (`suncc`), uma build 64-bit
    é o padrão em sistemas operando no modo de 64 bits; para uma build em 32 bits, defina
    o valor da variável de ambiente `SUNBITS` para `32` (*ex.*
    `export SUNBITS=32`).

- O bloqueio de arquivo não está disponível devido à ausência de `flock()` no **Solaris**.
  Isso será resolvido com o suporte ao bloqueio `fcntl()` estilo *System V* em um
  lançamento futuro.

##### Windows

- *Microsoft* **Windows** suporta vários desenvolvimentos e tempo de execução, incluindo
  *MSVC*, *Cygwin*, *Midipix*, *MSYS2*, *UWIN*, o
  ambiente *Git Bash* e outros. Deve-se tomar cuidado para evitar a mistura de
  bibliotecas e ferramentas incompatíveis.

###### Cygwin

- Problemas de compilação no ambiente **Cygwin** geralmente são causados ​​por
  instalações de pacotes incompletos ou interrompidos, ou pela instalação de
  pacotes usando ferramentas não padrão (*por exemplo* `apt-cyg`), o que pode resultar em
  arquivos ausentes e links simbólicos pendentes ou ausentes.
- **Antes** de compilar ***OpenVi*** em **Cygwin**, é *altamente*
  recomendado que:
  - Atualize o aplicativo **Cygwin** `setup.exe` para a última versão
    disponível.
  - Atualize todos os pacotes instalados usando a nova aplicação **Cygwin** `setup.exe`
  - Instale os pacotes de pré-requisito necessários (*ou seja,* `make`, `gcc`, `ncurses`,
    `ncurses-devel`) usando o aplicativo **Cygwin** `setup.exe`.
  - Invoque o utilitário `cygcheck` (*i.e.* `cygcheck -cv | grep -v "OK$"`) para
    verificar a integridade de todos os pacotes atualmente instalados.

## Disponibilidade

### Código fonte

- [Repositório de origem do GitHub](https://github.com/johnsonjh/OpenVi)
- [Lançamento da fonte mais recente](http://github.com/johnsonjh/OpenVi/releases/latest)

### Pacotes

**OpenVi** está disponível para usuários de Linux e macOS por meio do
Gerenciador de pacotes [Homebrew](https://formulae.brew.sh/formula/openvi).

[![Homebrew](https://repology.org/badge/version-for-repo/homebrew/openvi.svg)](https://repology.org/project/openvi/versions)

```sh
brew install openvi
```

Outros pacotes de distribuição (não oficiais) podem estar disponíveis.

[![Status da embalagem](https://repology.org/badge/vertical-allrepos/openvi.svg)](https://repology.org/project/openvi/versions)

## Versionamento

O número da versão do ***OpenVi*** é baseado na versão do lançamento do *OpenBSD* correspondente,
seguido pelo número do lançamento do ***OpenVi***. O comando `version` pode ser usado para exibir esta
informação no formato mostrado abaixo.

```text
Version 7.0.1 (OpenVi) 10/25/2021.
```

Esta mensagem indica que o editor em uso é ***OpenVi***, versão **1**,
derivado do *OpenBSD* versão **7.0**, e é totalmente sincronizado com as versões *OpenBSD* de
***`vi`***, ***`ex`***, ***`db`***, e ***`regex`***
a partir de **10/25/2021** (*25 de outubro de 2021*).

Alterações **não** derivadas de commits do *OpenBSD* não avançam nesta data.
Novos lançamentos do *OpenBSD* não redefinem o número do lançamento do ***OpenVi***.

## Histórico

- ***OpenVi***
  - [`ChangeLog`](/ChangeLog)
  - [Histórico de lançamento](http://github.com/johnsonjh/OpenVi/releases/)
  - [Histórico de commits](https://github.com/johnsonjh/OpenVi/commits/master)

- *OpenBSD* ***`vi`***
  - *OpenBSD* ***`vi`*** / ***`ex`***
    - [Histórico de commits](https://github.com/openbsd/src/commits/master/usr.bin/vi)
  - *OpenBSD* ***`db`***
    - [Histórico de commits](https://github.com/openbsd/src/commits/master/lib/libc/db)
  - *OpenBSD* ***`regex`***
    - [Histórico de commits](https://github.com/openbsd/src/commits/master/lib/libc/regex)

## Licença

- ***OpenVi*** é distribuído sob os termos de uma licença **3-clause BSD**.
- Veja o arquivo [`LICENSE.md`](/LICENSE.md) para a licença completa e
  termos de distribuição.

## Agradecimentos

- *rqsd* de *Libera.Chat* pela ideia que inspirou o projeto e os testes.
- [*S. V. Nickolas*](https://github.com/buricco/),
  [*Jason Stevens*](https://virtuallyfun.com/), e o
  [***Virtually Fun*** *Discord*](https://discord.gg/HMwevcN) comunidade, pelo
  suporte e feedback.
- Dos agradecimentos originais do **`vi`** (por *Bill Joy* e *Mark Horton*):
  - *Bruce Englar* encorajou o desenvolvimento inicial deste editor de exibição.
  - *Peter Kessler* ajudou a trazer sanidade ao layout de comando da versão 2.
  - *Bill Joy* escreveu a versão **1**, versões **2.0** até **2.7** e
    criou a estrutura que os usuários veem no editor atual.
  - *Mark Horton* adicionou macros e outros recursos e fez o editor funcionar
    em um grande número de terminais e sistemas *UNIX*.
  - Agradecemos o apoio financeiro dos *UUNET Communications Services*.

## Projetos Similares

- [**`Xvi`**](http://martinwguy.github.io/xvi/) de *Martin Guy*,
  uma versão melhorada de [**`STEVIE`**]( de *Tim Thompson* https://timthompson.com/tjt/stevie/)
- *S. V. Nickolas*'
  [**`Sivle`**](https://github.com/buricco/lunaris/tree/main/src/usr.bin/ex), um fork limpo de *Steve Kirkendall*
  [**`Elvis`**](http://elvis.the-little-red-haired-girl.org/)
- *Andy Valencia*'s [**`Vim57`**](https://sources.vsta.org:7100/vim57/tree), um
  fork simplificado da versão 5.7 de *Bram Moolenaar*
  [**`Vim`**](https://www.vim.org/)

## Veja também

- [*Carsten Kunze*'s **`vi`**](https://github.com/n-t-roff/heirloom-ex-vi/)
  é um fork atualmente mantido do ramo original (**1BSD**/**2BSD**) do editor **`vi`** / **`ex`**,
  derivado do editor aprimorado de *Gunnar Ritter* versão do [**tradicional** do editor **`vi`**](http://ex-vi.sourceforge.net/).
- [**`Nvi2`**](https://github.com/lichray/nvi2) é um *branch de recursos* atualmente mantido da nova versão (**4BSD**) do
  editor **`nvi`* * / **`nex`**, com foco em extensibilidade e novas funcionalidades.
- [**`Nvi1`**](https://repo.or.cz/nvi.git) (*versão* *1.8+*) é o *branch tradicional* atualmente mantido do novo (**4BSD* *) versão do
  editor **`nvi`** / **`nex`**, agora desenvolvido por *Sven Verdoolaege*.
