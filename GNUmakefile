###############################################################################
#                               -  O p e n V i  -                             #
###############################################################################

# Default C compiler and CFLAGS
CC          ?= cc
CFLAGS      += -std=gnu99 -I./cl -I./include -I. -MD
#CFLAGS     += -DVISIBLE_TAB_CHARS

###############################################################################

# Uncomment to override BDB 1.8.5 emulation detection and/or library name
#DB185       = 1
#DBLIB       = -ldb185

# Uncomment to enable debugging build
#DEBUG       = 1

###############################################################################

# Uncomment to enable link-time garbage collection (for non-debugging builds)
LGC          = 1
LTGC         = -fdata-sections -ffunction-sections
LTGL         = -Wl,--gc-sections

# Uncomment to enable link-time optimization (for non-debugging builds)
LTO          = 1
LTOC         = -flto

###############################################################################

# Optimization flags (for non-debugging builds)
OPTLEVEL    ?= -O2

# Extra debugging flags (only used for debugging builds)
DBGFLAGS    ?= -ggdb

# Default libraries to link
LINKLIBS    ?= -lncurses -lutil $(EXTRA_LIBS)

###############################################################################

# Installation directory prefix for install/uninstall
PREFIX      ?= /usr/local

# Executable prefix and/or suffix (e.g. 'o', '-openbsd') for install/uninstall
BINPREFIX   ?= o
#BINSUFFIX  ?= -openbsd

# Permissions and user:group to use for installed executables
IPERM        = 755
IUSGR        = root:bin

###############################################################################

ifdef DEBUG
   CFLAGS   += $(DBGFLAGS) -Wall -Wextra -DDEBUG -g3 -Og
else
   CFLAGS   += $(OPTLEVEL) -pipe -fomit-frame-pointer
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

LDFLAGS     += $(LINKLIBS)

###############################################################################

AWK         ?= command -p env PATH="$$(command -p getconf PATH)" awk
CHMOD       ?= chmod
CHOWN       ?= chown
CP          ?= cp -f
LNS         ?= ln -fs
MKDIR       ?= mkdir -p
PRINTF      ?= printf
RMDIR       ?= rmdir
RMF         ?= rm -f
SLEEP       ?= sleep
STRIP       ?= strip
TEST        ?= test
TRUE        ?= true

###############################################################################

SRCS = cl/cl_funcs.c cl/cl_main.c cl/cl_read.c cl/cl_screen.c cl/cl_term.c    \
       common/cut.c common/delete.c ex/ex.c ex/ex_abbrev.c ex/ex_append.c     \
       ex/ex_args.c ex/ex_argv.c ex/ex_at.c ex/ex_bang.c ex/ex_cd.c           \
       ex/ex_cmd.c ex/ex_delete.c ex/ex_display.c ex/ex_edit.c ex/ex_equal.c  \
       ex/ex_file.c ex/ex_filter.c ex/ex_global.c ex/ex_init.c ex/ex_join.c   \
       ex/ex_map.c ex/ex_mark.c ex/ex_mkexrc.c ex/ex_move.c ex/ex_open.c      \
       ex/ex_preserve.c ex/ex_print.c ex/ex_put.c ex/ex_quit.c ex/ex_read.c   \
       ex/ex_screen.c ex/ex_script.c ex/ex_set.c ex/ex_shell.c ex/ex_shift.c  \
       ex/ex_source.c ex/ex_stop.c ex/ex_subst.c ex/ex_tag.c ex/ex_txt.c      \
       ex/ex_undo.c ex/ex_usage.c ex/ex_util.c ex/ex_version.c ex/ex_visual.c \
       ex/ex_write.c ex/ex_yank.c ex/ex_z.c common/exf.c vi/getc.c            \
       common/key.c common/line.c common/log.c common/main.c common/mark.c    \
       common/msg.c common/options.c common/options_f.c common/put.c          \
       common/recover.c common/screen.c common/search.c common/seq.c          \
       common/util.c vi/v_at.c vi/v_ch.c vi/v_cmd.c vi/v_delete.c vi/v_ex.c   \
       vi/v_increment.c vi/v_init.c vi/v_itxt.c vi/v_left.c vi/v_mark.c       \
       vi/v_match.c vi/v_paragraph.c vi/v_put.c vi/v_redraw.c vi/v_replace.c  \
       vi/v_right.c vi/v_screen.c vi/v_scroll.c vi/v_search.c vi/v_section.c  \
       vi/v_sentence.c vi/v_status.c vi/v_txt.c vi/v_ulcase.c vi/v_undo.c     \
       vi/v_util.c vi/v_word.c vi/v_xchar.c vi/v_yank.c vi/v_z.c vi/v_zexit.c \
       vi/vi.c vi/vs_line.c vi/vs_msg.c vi/vs_refresh.c vi/vs_relative.c      \
       vi/vs_smap.c vi/vs_split.c cl/basename.c cl/getprogname.c cl/strlcpy.c \
       cl/pledge.c cl/strtonum.c cl/regcomp.c cl/regerror.c cl/regexec.c      \
       cl/regfree.c cl/getopt_long.c common/mkstemp.c

###############################################################################

VPATH = build:cl:common:ex:include:vi:bin
OBJS := ${SRCS:.c=.o}
DEPZ := ${OBJS:.o=.d}

###############################################################################

ifndef DB185
    ifneq (,$(wildcard /usr/include/db_185.h))
        DB185EMU = 1
        ifdef DEBUG
            $(info **** Berkeley DB 1.8.5 emulation enabled [/usr])
        endif # DEBUG
    endif # /usr/include/db_185.h
else
    unexport DB185EMU
    ifdef DEBUG
        $(info **** Berkeley DB 1.8.5 emulation forcefully disabled)
    endif # DEBUG
endif # DB185

ifndef DB185
    ifndef DB185EMU
        ifneq (,$(wildcard /usr/local/include/db_185.h))
            DB185EMU = 1
            CFLAGS  += -I/usr/local/include
            LDFLAGS += -L/usr/local/lib
            ifdef DEBUG
                $(info **** Berkelely DB 1.8.5 emulation enabled [/usr/local])
            endif # DEBUG
        endif # /usr/local/include/db_185.h
    endif # DB185EMU
endif # DB185

ifndef DB185
    ifndef DB185EMU
        ifneq (,$(wildcard /opt/freeware/include/db_185.h))
            DB185EMU = 1
            CFLAGS  += -I/opt/freeware/include
            LDFLAGS += -L/opt/freeware/lib
            ifdef DEBUG
                $(info **** Berkelely DB 1.8.5 emulation enabled [/opt/freeware])
            endif # DEBUG
        endif # /opt/freeware/include/db_185.h
    endif # DB185EMU
endif # DB185

ifndef DB185
    ifneq (,$(wildcard /usr/lib/libdb-5.so))
        DBLIB ?= -ldb-5
    endif # /usr/lib/libdb-5.so

    ifneq (,$(wildcard /usr/lib32/libdb-5.so))
        DBLIB ?= -ldb-5
    endif # /usr/lib32/libdb-5.so

    ifneq (,$(wildcard /usr/lib64/libdb-5.so))
        DBLIB ?= -ldb-5
    endif # /usr/lib64/libdb-5.so

    ifneq (,$(wildcard /usr/local/lib/libdb-5.so))
        DBLIB ?= -ldb-5
    endif # /usr/local/lib/libdb-5.so

    ifneq (,$(wildcard /usr/lib/libdb-4.so))
        DBLIB ?= -ldb-4
    endif # /usr/lib/libdb-4.so

    ifneq (,$(wildcard /usr/lib32/libdb-4.so))
        DBLIB ?= -ldb-4
    endif # /usr/lib32/libdb-4.so

    ifneq (,$(wildcard /usr/lib64/libdb-4.so))
        DBLIB ?= -ldb-4
    endif # /usr/lib64/libdb-4.so

    ifneq (,$(wildcard /usr/local/lib/libdb-4.so))
        DBLIB ?= -ldb-4
    endif # /usr/local/lib/libdb-4.so

    ifneq (,$(wildcard /usr/lib/libdb-3.so))
        DBLIB ?= -ldb-3
    endif # /usr/lib/libdb-3.so

    ifneq (,$(wildcard /usr/lib32/libdb-3.so))
        DBLIB ?= -ldb-3
    endif # /usr/lib32/libdb-3.so

    ifneq (,$(wildcard /usr/lib64/libdb-3.so))
        DBLIB ?= -ldb-3
    endif # /usr/lib64/libdb-3.so

    ifneq (,$(wildcard /usr/local/lib/libdb-3.so))
        DBLIB ?= -ldb-3
    endif # /usr/local/lib/libdb-3.so
endif # DB185

DBLIB ?= -ldb

ifdef DB185EMU
    CFLAGS += -DDB185EMU
endif # DB185EMU

LDFLAGS += $(DBLIB)

###############################################################################

ifneq (,$(wildcard /usr/lib/libmtmalloc.so))
    MALLOC ?= -lmtmalloc
endif # /usr/lib/libmtmalloc.so

ifneq (,$(wildcard /usr/lib/libjemalloc.so))
    MALLOC ?= -ljemalloc
endif # /usr/lib/libjemalloc.so

ifneq (,$(wildcard /usr/lib32/libjemalloc.so))
    MALLOC ?= -ljemalloc
endif # /usr/lib32/libjemalloc.so

ifneq (,$(wildcard /usr/lib64/libjemalloc.so))
    MALLOC ?= -ljemalloc
endif # /usr/lib64/libjemalloc.so

ifdef MALLOC
    LDFLAGS += $(MALLOC)
endif # MALLOC

###############################################################################

ifdef DEBUG
    $(info **** DEBUG is enabled; a debugging build will be produced)
    $(info **** Using CFLAGS: $(CFLAGS))
    $(info **** Using LDFLAGS: $(LDFLAGS))
endif # DEBUG

###############################################################################

all: vi ex view

###############################################################################

ex/ex_def.h: ex/ex.awk ex/ex_cmd.c
	$(RMF) ./ex/ex_def.h; \
        $(AWK) -f ./ex/ex.awk ./ex/ex_cmd.c \
            > ./ex/ex_def.h && $(TEST) -f ./ex/ex_def.h

###############################################################################

common/options_def.h: common/options.awk common/options.c ex/ex_def.h
	$(RMF) ./common/options_def.h; \
        $(AWK) -f ./common/options.awk ./common/options.c \
            > ./common/options_def.h && $(TEST) -f ./common/options_def.h

###############################################################################

clean distclean realclean mostlyclean:
	$(RMF) ./common/options_def.h ./ex/ex_def.h
	$(RMF) $(OBJS)
	$(RMF) $(DEPZ)
	$(TEST) -f ./bin/vi   && $(RMF)   ./bin/vi   || $(TRUE)
	$(TEST) -e ./bin/ex   && $(RMF)   ./bin/ex   || $(TRUE)
	$(TEST) -h ./bin/ex   && $(RMF)   ./bin/ex   || $(TRUE)
	$(TEST) -e ./bin/view && $(RMF)   ./bin/view || $(TRUE)
	$(TEST) -h ./bin/view && $(RMF)   ./bin/view || $(TRUE)
	$(TEST) -d ./bin      && $(RMDIR)   ./bin      || $(TRUE)
	-@$(TEST) -d ./bin    && $(PRINTF) '%s\n' \
        "WARNING: Build directory './bin' could not be removed." || $(TRUE)

###############################################################################

maintainer-clean:
	@$(PRINTF) '%s\n' \
        'WARNING: This command is intended for maintainer use only!'
	@$(PRINTF) '%s\n' \
        'It may delete files that require special tools to rebuild.'
	@$(SLEEP) 5
	$(MAKE) -C "." "distclean"

###############################################################################

%.o: %.c common/options_def.h ex/ex_def.h
	$(CC) -c -o $@ $< $(CFLAGS)

###############################################################################

bin/vi: $(OBJS)
	$(TEST) -d ./bin || $(MKDIR) ./bin
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: vi
vi: bin/vi
	@$(TRUE)

###############################################################################

bin/ex: vi
	$(LNS) vi ./bin/ex

.PHONY: ex
ex: bin/ex
	@$(TRUE)

###############################################################################

bin/view: vi
	$(LNS) vi ./bin/view

.PHONY: view
view: bin/view
	@$(TRUE)

###############################################################################

install: vi ex view virecover
	$(TEST) -d /tmp/vi.recover || \
        $(MKDIR) /tmp/vi.recover
	$(TEST) -d /tmp/vi.recover && \
        $(CHOWN) $(IUSGR) /tmp/vi.recover && \
            $(CHMOD) 1777 /tmp/vi.recover
	$(TEST) -d $(PREFIX)/bin || \
        $(MKDIR) $(PREFIX)/bin
	$(TEST) -d $(PREFIX)/libexec || \
        $(MKDIR) $(PREFIX)/libexec
	$(CP) ./bin/vi $(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX) && \
        $(CHOWN) $(IUSGR) $(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX) && \
        $(CHMOD) $(IPERM) $(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)
	$(TEST) -x $(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX) && \
        $(LNS) $(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX) \
            $(PREFIX)/bin/$(BINPREFIX)ex$(BINSUFFIX)
	$(TEST) -x $(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX) && \
        $(LNS) $(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX) \
            $(PREFIX)/bin/$(BINPREFIX)view$(BINSUFFIX)
	$(CP) ./build/virecover \
        $(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX) && \
        $(CHMOD) $(IPERM) $(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)

###############################################################################

install-strip: install
	$(STRIP) $(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)

###############################################################################

strip: vi
	$(STRIP) ./bin/vi

###############################################################################

uninstall:
	$(RMF) $(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)
	$(RMF) $(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)
	$(RMF) $(PREFIX)/bin/$(BINPREFIX)ex$(BINSUFFIX)
	$(RMF) $(PREFIX)/bin/$(BINPREFIX)view$(BINSUFFIX)

###############################################################################
