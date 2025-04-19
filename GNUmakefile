###############################################################################
#                               -  O p e n V i  -                             #
###############################################################################
# vim: filetype=make:tabstop=8:tw=79:noexpandtab:colorcolumn=79:list:
# SPDX-License-Identifier: BSD-3-Clause
###############################################################################

###############################################################################
#
# Copyright (c) 2021-2024 Jeffrey H. Johnson <trnsz@pobox.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered "AS-IS",
# without any warranty.
#
###############################################################################

###############################################################################

# Default compiler settings
CC          ?= cc
DEPFLAGS    ?= -MMD -MP
INCLDS       = -Iinclude -Icommon -Iregex -Iopenbsd
CFLAGS      += -std=gnu99 $(INCLDS)
WFLAGS      ?= -Wall -Wno-pointer-sign -Wno-uninitialized

###############################################################################

# Set DEBUG to enable debugging build
#DEBUG       = 1
DBGFLAGS    ?= -Wextra -ggdb -g3 -O0

###############################################################################

# Set LGC to enable link-time garbage collection (for non-debugging builds)
#LGC         = 1
LTGC         = -fdata-sections -ffunction-sections
LTGL         = -Wl,--gc-sections

###############################################################################

# Set LTO to enable link-time optimization (for non-debugging builds)
#LTO         = 1
LTOC         = -flto

###############################################################################

PKGCFG      ?= pkg-config
TR          ?= tr
UNAME       ?= uname

ifndef OS
    OS=$(shell $(UNAME) -s 2> /dev/null | \
        $(TR) '[:upper:]' '[:lower:]' 2> /dev/null)
endif # OS

ifeq ($(OS),sunos)
    OS=$(shell $(UNAME) -o 2> /dev/null | \
        $(TR) '[:upper:]' '[:lower:]' 2> /dev/null)
    _SUNOS = 1
endif # sunos

ifeq ($(OS),os400)
    _OS400 = 1
        OS = aix
endif # os400

###############################################################################

ifeq ($(OS),solaris)
   ifneq (,$(findstring suncc,$(CC))) # suncc
      OPTLEVEL ?= -O2
      _OSLCC = 1
   endif # suncc
else
   OPTLEVEL    ?= -Os
endif # solaris

###############################################################################

ifeq ($(OS),netbsd)
   ifneq (,$(findstring clang,$(CC))) # clang
      WFLAGS += -Wno-unknown-warning-option  \
                -Wno-system-headers          \
                -Wno-char-subscripts
   endif # clang
endif # netbsd
ifeq ($(OS),illumos)
   WFLAGS += -Wno-unknown-pragmas
endif # illumos
ifeq ($(OS),solaris)
   ifeq ($(_OSLCC),1)
      SUNBITS ?= $$(command -p isainfo -b 2> /dev/null || printf '%s' 32)
       WFLAGS += -erroff=E_EMPTY_DECLARATION            \
                 -erroff=E_STATEMENT_NOT_REACHED        \
                 -erroff=E_ARG_INCOMPATIBLE_WITH_ARG_L  \
                 -erroff=E_ASSIGNMENT_TYPE_MISMATCH     \
                 -erroff=E_ATTRIBUTE_UNKNOWN
       CFLAGS += -m$(SUNBITS)
      LDFLAGS += -m$(SUNBITS)
   endif # suncc
endif # solaris

###############################################################################
# Try to query pkg-config for ncurses flags and libraries

ifndef CURSESLIB
  CFLAGS    += $(shell $(PKGCFG) ncurses --cflags 2> /dev/null \
                     || $(PKGCFG) curses --cflags 2> /dev/null)
  CURSESLIB += $(shell $(PKGCFG) ncurses --libs 2> /dev/null \
                     || $(PKGCFG) curses --libs 2> /dev/null)
endif #!CURSESLIB

###############################################################################

# Default libraries to link
ifeq ($(OS),netbsd)
   CURSESLIB ?= -lcurses -lterminfo
     LDFLAGS += -L"/usr/local/lib" -L"/usr/pkg/lib" -L"/usr/lib" -L"/lib"
     LDFLAGS += -Wl,-R"/lib" -Wl,-R"/usr/lib" -Wl,-R"/usr/pkg/lib" \
                -Wl,-R"/usr/local/bin"
   ifdef LTO
      ifneq (,$(findstring clang,$(CC))) # clang
         LLD     ?= ld.lld
         LDFLAGS += -fuse-ld="$$(command -v $(LLD) || $(PRINTF) '%s' $(LLD))"
      endif # clang
   endif # LTO
else # !netbsd
   CURSESLIB ?= -lncurses
endif # netbsd
ifeq ($(OS),aix) # aix/os400
   MAIXBITS ?= $(shell command -p $(GETCONF) KERNEL_BITMODE 2> /dev/null || \
                    $(PRINTF) '%s' "32")
      ifeq ($(_OS400),1)                 # IBM i (OS/400) PASE
         CFLAGS  += -I/QOpenSys/pkgs/include/ncurses
         LDFLAGS += -lutil -L/QOpenSys/pkgs/lib
      endif
      ifneq (,$(findstring gcc,$(CC)))   # gcc (GNU C)
         CFLAGS  += $(WFLAGS) -maix$(MAIXBITS)
         LDFLAGS += -maix$(MAIXBITS) -Wl,-b$(MAIXBITS)
      endif # gcc
      ifneq (,$(findstring clang,$(CC))) # xlclang / ibm-clang (IBM Open XL)
         CFLAGS  += $(WFLAGS) -m$(MAIXBITS)
         LDFLAGS += -m$(MAIXBITS) -Wl,-b$(MAIXBITS)
      endif # clang
      ifneq (,$(findstring gxlc,$(CC)))  # gxlc (IBM XL C)
         CFLAGS  += -m$(MAIXBITS)
         LDFLAGS += -m$(MAIXBITS) -Wl,-b$(MAIXBITS)
         DEPFLAGS =
      endif # gxlc
   LDFLAGS  += -L/opt/freeware/lib
   CFLAGS   += -I/opt/freeware/include
   LINKLIBS ?= -lbsd $(CURSESLIB) -lcurses
else # !aix/os400
ifeq ($(OS),solaris)
     CFLAGS += -U__EXTENSIONS__ -D_XPG4_2 -D__solaris__ -D_REENTRANT \
               -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_XOPEN_SOURCE=600
endif # solaris
ifeq ($(OS),illumos)
     CFLAGS += -D__illumos__
endif # illumos
ifeq ($(_SUNOS),1)
     CFLAGS += -Du_int32_t=uint32_t -Du_int16_t=uint16_t -Du_int8_t=uint8_t
     CFLAGS += -DBYTE_ORDER=__BYTE_ORDER__
     CFLAGS += -I/usr/include/ncurses
   LINKLIBS ?= $(CURSESLIB)
else # !sunos
   LINKLIBS ?= -lutil $(CURSESLIB)
endif # sunos
     CFLAGS += $(WFLAGS)
endif # aix
LINKLIBS    += $(EXTRA_LIBS)

###############################################################################

CFLAGS += -Ddbm_open=openbsd_dbm_open -Ddbm_close=openbsd_dbm_close          \
          -Ddbm_fetch=openbsd_dbm_fetch -Ddbm_firstkey=openbsd_dbm_firstkey  \
          -Ddbm_nextkey=openbsd_dbm_nextkey -Ddbm_delete=openbsd_dbm_delete  \
          -Ddbm_store=openbsd_dbm_store -Ddbm_error=openbsd_dbm_error        \
          -Ddbm_clearerr=openbsd_dbm_clearerr -Ddbm_rdonly=openbsd_dbm_rdonly

###############################################################################

# Installation directory prefix for install/uninstall
PREFIX      ?= /usr/local
DESTIDIR    ?=

###############################################################################

# Executable prefix and/or suffix (e.g. 'o', '-openbsd') for install/uninstall
BINPREFIX   ?= o

###############################################################################

# Permissions and user:group to use for installed executables
IPERM        = 755
IUSGR        = root:bin

###############################################################################

# Using _FORTIFY_SOURCE=2 grows the binary by about ~2-3 KiB (on AMD64)
ifdef DEBUG
   CFLAGS   += $(DBGFLAGS) -DDEBUG -DSTATISTICS -DHASH_STATISTICS
else # !DEBUG
   CFLAGS   += $(OPTLEVEL) -D_FORTIFY_SOURCE=2
endif # DEBUG

###############################################################################

ifndef DEBUG
    ifdef LTO
        CFLAGS  += $(LTOC)
        LDFLAGS += $(LTOC)
    endif # LTO
    ifdef LGC
        CFLAGS  += $(LTGC)
        LDFLAGS += $(LTGL)
    endif # LGC
endif # DEBUG

###############################################################################

AWK         ?= awk
CHMOD       ?= chmod
CHOWN       ?= chown
CP          ?= cp -f
PENV        := env
GETCONF     ?= getconf
LN          ?= ln
LNS          = $(LN) -fs
MKDIR       ?= mkdir -p
PAWK         = command -p $(PENV) PATH="$$(command -p $(GETCONF) PATH)" $(AWK)
PRINTF      ?= printf
RMDIR       ?= rmdir
RM          ?= rm
RMF          = $(RM) -f
SLEEP       ?= sleep
STRIP       ?= strip
SSTRIP      ?= sstrip
TEST        ?= test
TRUE        ?= true
UPX         ?= upx

###############################################################################

ifdef V
    DEBUG = 1
endif # V

###############################################################################

ifdef DEBUG
    VERBOSE = set -ex
else # !DEBUG
    VERBOSE = $(TRUE)
endif # DEBUG

###############################################################################

XSRC = openbsd/dirname.c       \
       openbsd/err.c           \
       openbsd/errc.c          \
       openbsd/errx.c          \
       openbsd/getopt_long.c   \
       openbsd/getprogname.c   \
       openbsd/issetugid.c     \
       openbsd/minpwcache.c    \
       openbsd/reallocarray.c  \
       openbsd/setmode.c       \
       openbsd/strlcat.c       \
       openbsd/strlcpy.c       \
       openbsd/strtonum.c      \
       openbsd/verr.c          \
       openbsd/verrc.c         \
       openbsd/verrx.c         \
       openbsd/vwarn.c         \
       openbsd/vwarnc.c        \
       openbsd/vwarnx.c        \
       openbsd/warn.c          \
       openbsd/warnc.c         \
       openbsd/warnx.c         \
       xinstall/xinstall.c

SRCS = cl/cl_funcs.c           \
       cl/cl_main.c            \
       cl/cl_read.c            \
       cl/cl_screen.c          \
       cl/cl_term.c            \
       common/cut.c            \
       common/delete.c         \
       common/exf.c            \
       common/key.c            \
       common/line.c           \
       common/log.c            \
       common/main.c           \
       common/mark.c           \
       common/msg.c            \
       common/options.c        \
       common/options_f.c      \
       common/put.c            \
       common/recover.c        \
       common/screen.c         \
       common/search.c         \
       common/seq.c            \
       common/util.c           \
       db/btree/bt_close.c     \
       db/btree/bt_conv.c      \
       db/btree/bt_debug.c     \
       db/btree/bt_delete.c    \
       db/btree/bt_get.c       \
       db/btree/bt_open.c      \
       db/btree/bt_overflow.c  \
       db/btree/bt_page.c      \
       db/btree/bt_put.c       \
       db/btree/bt_search.c    \
       db/btree/bt_seq.c       \
       db/btree/bt_split.c     \
       db/btree/bt_utils.c     \
       db/db/db.c              \
       db/hash/hash_bigkey.c   \
       db/hash/hash_buf.c      \
       db/hash/hash.c          \
       db/hash/hash_func.c     \
       db/hash/hash_log2.c     \
       db/hash/hash_page.c     \
       db/hash/ndbm.c          \
       db/mpool/mpool.c        \
       db/recno/rec_close.c    \
       db/recno/rec_delete.c   \
       db/recno/rec_get.c      \
       db/recno/rec_open.c     \
       db/recno/rec_put.c      \
       db/recno/rec_search.c   \
       db/recno/rec_seq.c      \
       db/recno/rec_utils.c    \
       ex/ex_abbrev.c          \
       ex/ex_append.c          \
       ex/ex_args.c            \
       ex/ex_argv.c            \
       ex/ex_at.c              \
       ex/ex_bang.c            \
       ex/ex.c                 \
       ex/ex_cd.c              \
       ex/ex_cmd.c             \
       ex/ex_delete.c          \
       ex/ex_display.c         \
       ex/ex_edit.c            \
       ex/ex_equal.c           \
       ex/ex_file.c            \
       ex/ex_filter.c          \
       ex/ex_global.c          \
       ex/ex_init.c            \
       ex/ex_join.c            \
       ex/ex_map.c             \
       ex/ex_mark.c            \
       ex/ex_mkexrc.c          \
       ex/ex_move.c            \
       ex/ex_open.c            \
       ex/ex_preserve.c        \
       ex/ex_print.c           \
       ex/ex_put.c             \
       ex/ex_quit.c            \
       ex/ex_read.c            \
       ex/ex_screen.c          \
       ex/ex_script.c          \
       ex/ex_set.c             \
       ex/ex_shell.c           \
       ex/ex_shift.c           \
       ex/ex_source.c          \
       ex/ex_stop.c            \
       ex/ex_subst.c           \
       ex/ex_tag.c             \
       ex/ex_txt.c             \
       ex/ex_undo.c            \
       ex/ex_usage.c           \
       ex/ex_util.c            \
       ex/ex_version.c         \
       ex/ex_visual.c          \
       ex/ex_write.c           \
       ex/ex_yank.c            \
       ex/ex_z.c               \
       openbsd/basename.c      \
       openbsd/err.c           \
       openbsd/errx.c          \
       openbsd/getopt_long.c   \
       openbsd/getprogname.c   \
       openbsd/issetugid.c     \
       openbsd/open.c          \
       openbsd/pledge.c        \
       openbsd/reallocarray.c  \
       openbsd/strlcpy.c       \
       openbsd/strtonum.c      \
       openbsd/verr.c          \
       openbsd/verrx.c         \
       openbsd/vwarn.c         \
       openbsd/vwarnx.c        \
       openbsd/warn.c          \
       openbsd/warnx.c         \
       regex/regcomp.c         \
       regex/regerror.c        \
       regex/regexec.c         \
       regex/regfree.c         \
       vi/getc.c               \
       vi/v_at.c               \
       vi/v_ch.c               \
       vi/v_cmd.c              \
       vi/v_delete.c           \
       vi/v_ex.c               \
       vi/vi.c                 \
       vi/v_increment.c        \
       vi/v_init.c             \
       vi/v_itxt.c             \
       vi/v_left.c             \
       vi/v_mark.c             \
       vi/v_match.c            \
       vi/v_paragraph.c        \
       vi/v_put.c              \
       vi/v_redraw.c           \
       vi/v_replace.c          \
       vi/v_right.c            \
       vi/v_screen.c           \
       vi/v_scroll.c           \
       vi/v_search.c           \
       vi/v_section.c          \
       vi/v_sentence.c         \
       vi/vs_line.c            \
       vi/vs_msg.c             \
       vi/vs_refresh.c         \
       vi/vs_relative.c        \
       vi/vs_smap.c            \
       vi/vs_split.c           \
       vi/v_status.c           \
       vi/v_txt.c              \
       vi/v_ulcase.c           \
       vi/v_undo.c             \
       vi/v_util.c             \
       vi/v_word.c             \
       vi/v_xchar.c            \
       vi/v_yank.c             \
       vi/v_z.c                \
       vi/v_zexit.c

###############################################################################

VPATH = build:cl:common:db:ex:include:vi:regex:openbsd:bin
OBJS := ${SRCS:.c=.o}
XOBJ := ${XSRC:.c=.o}
DEPS := ${OBJS:.o=.d}
XDEP := ${XOBJ:.o=.d}

###############################################################################

.PHONY: all
all: bin/vi                    \
     bin/ex                    \
     bin/view                  \
     bin/xinstall              \
     docs/USD.doc/vi.man/vi.1  \
     scripts/virecover.8

###############################################################################

ex/ex_def.h: ex/ex.awk ex/ex_cmd.c
ifndef DEBUG
	-@$(PRINTF) "\r\t$(AWK):\t%42s\n" "ex/ex.awk"
endif # DEBUG
	@$(VERBOSE); $(RMF) "./ex/ex_def.h";  \
        $(PAWK) -f                            \
            "./ex/ex.awk" "./ex/ex_cmd.c"     \
                > "./ex/ex_def.h" &&          \
                    $(TEST) -f                \
                       "./ex/ex_def.h"

###############################################################################

common/options_def.h: common/options.awk common/options.c ex/ex_def.h
ifndef DEBUG
	-@$(PRINTF) "\r\t$(AWK):\t%42s\n" "command/options.awk"
endif # DEBUG
	@$(VERBOSE); $(RMF) "./common/options_def.h";    \
        $(PAWK) -f                                       \
            "./common/options.awk" "./common/options.c"  \
                > "./common/options_def.h" &&            \
                    $(TEST) -f "./common/options_def.h"

###############################################################################

.PHONY: clean distclean realclean mostlyclean maintainer-clean
ifneq (,$(findstring clean,$(MAKECMDGOALS)))
.NOTPARALLEL: clean distclean realclean mostlyclean maintainer-clean
endif # (,$(findstring clean,$(MAKECMDGOALS)))
clean distclean realclean mostlyclean maintainer-clean:
ifndef DEBUG
	-@$(PRINTF) '\r\t%s\t%42s\n' "rm:" "common/options_def.h"
endif # DEBUG
	@$(VERBOSE); $(RMF) "./common/options_def.h"
ifndef DEBUG
	-@$(PRINTF) '\r\t%s\t%42s\n' "rm:" "ex/ex_def.h"
endif # DEBUG
	@$(VERBOSE); $(RMF) "./ex/ex_def.h"
ifndef DEBUG
	-@$(PRINTF) '\r\t%s\t%42s\n' "rm:" "objects"
endif # DEBUG
	@$(VERBOSE); $(RMF) $(OBJS) $(XOBJ)
ifndef DEBUG
	-@$(PRINTF) '\r\t%s\t%42s\n' "rm:" "dependencies"
endif # DEBUG
	@$(VERBOSE); $(RMF) $(DEPS) $(XDEP)
ifndef DEBUG
	-@$(PRINTF) '\r\t%s\t%42s\n' "rm:" "bin/vi"
endif # DEBUG
	@$(VERBOSE); $(TEST) -f "./bin/vi" && $(RMF) "./bin/vi" || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) '\r\t%s\t%42s\n' "rm:" "bin/ex"
endif # DEBUG
	@$(VERBOSE); $(TEST) -e "./bin/ex" && $(RMF) "./bin/ex" || $(TRUE)
	@$(VERBOSE); $(TEST) -h "./bin/ex" && $(RMF) "./bin/ex" || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) '\r\t%s\t%42s\n' "rm:" "bin/view"
endif # DEBUG
	@$(VERBOSE); $(TEST) -e "./bin/view" && $(RMF) "./bin/view" || $(TRUE)
	@$(VERBOSE); $(TEST) -h "./bin/view" && $(RMF) "./bin/view" || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) '\r\t%s\t%42s\n' "rm:" "bin/xinstall"
endif # DEBUG
	@$(VERBOSE); $(TEST) -f "./bin/xinstall" && \
            $(RMF) "./bin/xinstall" || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) '\r\t%s\t%42s\n' "$(RMDIR):" "bin"
endif # DEBUG
	@$(VERBOSE); $(TEST) -d "./bin" && $(RMDIR) "./bin" || $(TRUE)
	-@$(TEST) -d "./bin" && $(PRINTF) '\r\t%s\t%42s\n' \
            "WARNING:" "Directory './bin' not removed." || $(TRUE)

###############################################################################

%.o: %.c common/options_def.h ex/ex_def.h
ifndef DEBUG
	-@$(PRINTF) "\r\t$(CC):\t%42s\n" "$@"
endif # DEBUG
	@$(VERBOSE); $(CC) $(CFLAGS) $(DEPFLAGS) -c -o "$@" "$<"
-include $(wildcard $(DEPS))
-include $(wildcaed $(XDEP))

###############################################################################

bin/xinstall: $(XOBJ)
	@$(TEST) -d "./bin" || $(MKDIR) "./bin"
ifndef DEBUG
	-@$(PRINTF) '\r\t$(LD):\t%42s\n' "$@"
endif # DEBUG
	@$(VERBOSE); $(CC) -o "$@" $^ $(LDFLAGS) $(EXTRA_LIBS)

.PHONY: xinstall
xinstall: bin/xinstall
	-@$(TRUE)

###############################################################################

bin/vi: $(OBJS)
	@$(TEST) -d "./bin" || $(MKDIR) "./bin"
ifndef DEBUG
	-@$(PRINTF) '\r\t$(LD):\t%42s\n' "$@"
endif # DEBUG
	@$(VERBOSE); $(CC) -o "$@" $^ $(LDFLAGS) $(LINKLIBS)

.PHONY: vi
vi: bin/vi
	-@$(TRUE)

###############################################################################

bin/ex: bin/vi
ifndef DEBUG
	-@$(PRINTF) "\r\t$(LN):\t%42s\n" "$@"
endif # DEBUG
	@$(VERBOSE); $(LNS) "vi" "./bin/ex"

.PHONY: ex
ex: bin/ex
	-@$(TRUE)

##############################################################################

bin/view: bin/vi
ifndef DEBUG
	-@$(PRINTF) "\r\t$(LN):\t%42s\n" "$@"
endif # DEBUG
	@$(VERBOSE); $(LNS) "vi" "./bin/view"

.PHONY: view
view: bin/view
	-@$(TRUE)

###############################################################################

.PHONY: install
ifneq (,$(findstring install,$(MAKECMDGOALS)))
.NOTPARALLEL: install
endif # (,$(findstring install,$(MAKECMDGOALS)))
install: bin/vi bin/ex bin/view docs/USD.doc/vi.man/vi.1  \
         scripts/virecover scripts/virecover.8
ifndef DEBUG
	-@$(PRINTF) "\r\t%s\t%42s\n" "mkdir:" "$(DESTDIR)$(PREFIX)/bin"
endif # DEBUG
	@$(VERBOSE); $(TEST) -d "$(DESTDIR)$(PREFIX)/bin" || \
            $(MKDIR) "$(DESTDIR)$(PREFIX)/bin"
ifndef DEBUG
	-@$(PRINTF) "\r\t%s\t%42s\n" "mkdir:" "$(DESTDIR)$(PREFIX)/libexec"
endif # DEBUG
	@$(VERBOSE); $(TEST) -d "$(DESTDIR)$(PREFIX)/libexec" || \
            $(MKDIR) "$(DESTDIR)$(PREFIX)/libexec"
ifndef DEBUG
	@$(PRINTF) "\r\t%s\t%42s\n" \
            "mkdir:" "$(DESTDIR)$(PREFIX)/share/man/man1"
endif # DEBUG
	@$(VERBOSE); $(TEST) -d "$(DESTDIR)$(PREFIX)/share/man/man1" || \
            $(MKDIR) "$(DESTDIR)$(PREFIX)/share/man/man1"
ifndef DEBUG
	-@$(PRINTF) "\r\t%s\t%42s\n" \
            "mkdir:" "$(DESTDIR)$(PREFIX)/share/man/man8"
endif # DEBUG
	@$(VERBOSE); $(TEST) -d "$(DESTDIR)$(PREFIX)/share/man/man8" || \
            $(MKDIR) "$(DESTDIR)$(PREFIX)/share/man/man8"
ifndef DEBUG
	-@$(PRINTF) "\r\t%s\t%42s\n" \
        "cp:" "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"
endif # DEBUG
	@$(VERBOSE); $(CP) ./bin/vi                                 \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" && \
        $(CHOWN) "$(IUSGR)"                                         \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" && \
        $(CHMOD) "$(IPERM)"                                         \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"
ifndef DEBUG
	@$(PRINTF) "\r\t%s\t%42s\n" \
        "ln:" "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)ex$(BINSUFFIX)"
endif # DEBUG
	@$(VERBOSE); $(TEST) -x                                     \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" && \
        $(LNS) "$(BINPREFIX)vi$(BINSUFFIX)"                         \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)ex$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\r\t%s\t%42s\n" \
        "ln:" "$(PREFIX)/bin/$(BINPREFIX)view$(BINSUFFIX)"
endif # DEBUG
	@$(VERBOSE); $(TEST) -x                                     \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" && \
        $(LNS) "$(BINPREFIX)vi$(BINSUFFIX)"                         \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)view$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\r\t%s\t%42s\n" \
        "cp:" "$(DESTDIR)$(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)"
endif # DEBUG
	@$(VERBOSE); $(CP) "./scripts/virecover"                             \
         "$(DESTDIR)$(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)" && \
        $(CHMOD) "$(IPERM)"                                                  \
         "$(DESTDIR)$(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\r\t%s\t%42s\n" \
        "cp:" \
    "$(DESTDIR)$(PREFIX)/share/man/man1/$(BINPREFIX){vi,ex,view}$(BINSUFFIX).1"
endif # DEBUG
	@$(VERBOSE); $(CP) "docs/USD.doc/vi.man/vi.1"                       \
        "$(DESTDIR)$(PREFIX)/share/man/man1/$(BINPREFIX)vi$(BINSUFFIX).1"   \
         && $(LNS)                                                          \
        "$(PREFIX)/share/man/man1/$(BINPREFIX)vi$(BINSUFFIX).1"   \
        "$(DESTDIR)$(PREFIX)/share/man/man1/$(BINPREFIX)view$(BINSUFFIX).1" \
         && $(LNS)                                                          \
        "$(PREFIX)/share/man/man1/$(BINPREFIX)vi$(BINSUFFIX).1"   \
        "$(DESTDIR)$(PREFIX)/share/man/man1/$(BINPREFIX)ex$(BINSUFFIX).1"
ifndef DEBUG
	-@$(PRINTF) "\r\t%s\t%42s\n" \
          "cp:" \
      "$(DESTDIR)$(PREFIX)/share/man/man8/$(BINPREFIX)vi.recover$(BINSUFFIX).8"
endif # DEBUG
	@$(VERBOSE); $(CP) "scripts/virecover.8" \
      "$(DESTDIR)$(PREFIX)/share/man/man8/$(BINPREFIX)vi.recover$(BINSUFFIX).8"

###############################################################################

.PHONY: install-strip installstrip
ifneq (,$(findstring install-strip,$(MAKECMDGOALS)))
.NOTPARALLEL: install-strip installstrip install
endif # (,$(findstring install-strip,$(MAKECMDGOALS)))
install-strip installstrip: install
ifndef DEBUG
	-@$(PRINTF) "\r\t$(STRIP):\t%42s\n" \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"
endif # DEBUG
	-@$(VERBOSE); $(PENV) OBJECT_MODE=$(MAIXBITS)                        \
            $(STRIP) "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" || \
              $(TRUE)

###############################################################################

.PHONY: strip
ifneq (,$(findstring strip,$(MAKECMDGOALS)))
.NOTPARALLEL: strip
endif # (,$(findstring strip,$(MAKECMDGOALS)))
strip: bin/vi bin/ex bin/view
ifndef DEBUG
	-@$(PRINTF) "\r\t$(STRIP):\t%42s\n" "bin/vi"
endif # DEBUG
	-@$(VERBOSE); $(PENV) OBJECT_MODE=$(MAIXBITS) \
            $(STRIP) "./bin/vi" || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) "\r\t$(STRIP):\t%42s\n" "bin/xinstall"
endif # DEBUG
	-@$(VERBOSE); $(PENV) OBJECT_MODE=$(MAIXBITS) \
            $(STRIP) "./bin/xinstall" || $(TRUE)

###############################################################################

.PHONY: superstrip sstrip
ifneq (,$(findstring strip,$(MAKECMDGOALS)))
.NOTPARALLEL: superstrip sstrip
endif # (,$(findstring strip,$(MAKECMDGOALS)))
ifneq ($(OS),freebsd)
    STRIP_VERS=-R '.gnu.version'
else # freebsd
    STRIP_VERS=
endif # !freebsd
ifneq ($(OS),netbsd)
    STRIP_NOTE=-R '.note.*'
else # netbsd
    STRIP_NOTE=-R '.SUNW_ctf' -R '.jcr' -R '.ident'  \
               -R '.note.netbsd.mcmodel'             \
               -R '.note.netbsd.pax' -R '.gnu.hash'
endif # !netbsd
superstrip sstrip: strip bin/vi bin/ex bin/view
ifndef DEBUG
	-@$(PRINTF) "\r\t$(STRIP):\t%42s\n" "bin/vi"
endif # DEBUG
	-@$(VERBOSE); $(PENV) OBJECT_MODE=$(MAIXBITS)  \
            $(STRIP) --strip-all                       \
                -R '.gnu.build.attributes'             \
                -R '.eh_frame'                         \
                -R '.eh_frame*'                        \
                -R '.comment'   $(STRIP_NOTE)          \
                -R '.comment.*' $(STRIP_VERS)          \
                    "./bin/vi"                         \
                      2> /dev/null || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) "\r\t$(SSTRIP):\t%42s\n" "bin/vi"
endif # DEBUG
	-@$(VERBOSE); $(SSTRIP) -z "./bin/vi" \
           2> /dev/null || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) "\r\t$(STRIP):\t%42s\n" "bin/xinstall"
endif # DEBUG
	-@$(VERBOSE); $(PENV) OBJECT_MODE=$(MAIXBITS)  \
            $(STRIP) --strip-all                       \
                -R '.gnu.build.attributes'             \
                -R '.eh_frame'                         \
                -R '.eh_frame*'                        \
                -R '.comment'   $(STRIP_NOTE)          \
                -R '.comment.*' $(STRIP_VERS)          \
                    "./bin/xinstall"                   \
                      2> /dev/null || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) "\r\t$(SSTRIP):\t%42s\n" "bin/xinstall"
endif # DEBUG
	-@$(VERBOSE); $(SSTRIP) -z "./bin/xinstall" \
            2> /dev/null || $(TRUE)

###############################################################################

.PHONY: upx
ifneq (,$(findstring upx,$(MAKECMDGOALS)))
.NOTPARALLEL: upx
endif # (,$(findstring upx,$(MAKECMDGOALS)))
upx: sstrip
ifndef DEBUG
	-@$(PRINTF) "\r\t$(UPX):\t%42s\n" "bin/vi"
endif # DEBUG
	-@$(VERBOSE); $(UPX) -qqq9 --exact "./bin/vi" 2> /dev/null || $(TRUE)
	-@$(VERBOSE); $(UPX) -qqq9         "./bin/vi" 2> /dev/null || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) "\r\t$(SSTRIP):\t%42s\n" "bin/vi"
endif # DEBUG
	-@$(VERBOSE); $(SSTRIP) -z "./bin/vi" 2> /dev/null || $(TRUE)

###############################################################################

.PHONY: uninstall
ifneq (,$(findstring uninstall,$(MAKECMDGOALS)))
.NOTPARALLEL: uninstall
endif # (,$(findstring uninstall,$(MAKECMDGOALS)))
uninstall:
ifndef DEBUG
	-@$(PRINTF) "\r\trm:\t%42s\n" \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"
endif # DEBUG
	-@$(VERBOSE); $(RMF) \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\r\trm:\t%42s\n" \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)ex$(BINSUFFIX)"
endif # DEBUG
	-@$(VERBOSE); $(RMF) \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)ex$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\r\trm:\t%42s\n" \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)view$(BINSUFFIX)"
endif # DEBUG
	-@$(VERBOSE); $(RMF) \
            "$(DESTDIR)$(PREFIX)/bin/$(BINPREFIX)view$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\r\trm:\t%42s\n" \
            "$(DESTDIR)$(PREFIX)/share/man/man1/$(BINPREFIX)vi$(BINSUFFIX).1"
endif # DEBUG
	-@$(VERBOSE); $(RMF) \
            "$(DESTDIR)$(PREFIX)/share/man/man1/$(BINPREFIX)vi$(BINSUFFIX).1"
ifndef DEBUG
	-@$(PRINTF) "\r\trm:\t%42s\n" \
            "$(DESTDIR)$(PREFIX)/share/man/man1/$(BINPREFIX)ex$(BINSUFFIX).1"
endif # DEBUG
	-@$(VERBOSE); $(RMF) \
            "$(DESTDIR)$(PREFIX)/share/man/man1/$(BINPREFIX)ex$(BINSUFFIX).1"
ifndef DEBUG
	-@$(PRINTF) "\r\trm:\t%42s\n" \
            "$(DESTDIR)$(PREFIX)/share/man/man1/$(BINPREFIX)view$(BINSUFFIX).1"
endif # DEBUG
	-@$(VERBOSE); $(RMF) \
            "$(DESTDIR)$(PREFIX)/share/man/man1/$(BINPREFIX)view$(BINSUFFIX).1"
ifndef DEBUG
	-@$(PRINTF) "\r\trm:\t%42s\n" \
      "$(DESTDIR)$(PREFIX)/share/man/man8/$(BINPREFIX)vi.recover$(BINSUFFIX).8"
endif # DEBUG
	-@$(VERBOSE); $(RMF) \
      "$(DESTDIR)$(PREFIX)/share/man/man8/$(BINPREFIX)vi.recover$(BINSUFFIX).8"
ifndef DEBUG
	-@$(PRINTF) "\r\trm:\t%42s\n" \
            "$(DESTDIR)$(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)"
endif # DEBUG
	-@$(VERBOSE); $(RMF) \
            "$(DESTDIR)$(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)"

###############################################################################

# Local Variables:
# mode: make
# tab-width: 8
# End:
