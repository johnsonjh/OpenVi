###############################################################################

# Uncomment to override BDB 1.8.5 emulation detection
#DB185 = 1

# Uncomment to override BDB library name detection
#DBLIB = -ldb185

# Uncomment to enable debugging build
#DEBUG_VI = 1

# Uncomment to link-time garbage collection for non-debugging builds
LTGC = -fdata-sections -ffunction-sections -Wl,--gc-sections

# Uncomment to enable link-time optimization for non-debugging builds
LTOC = -flto

# Uncomment to enable some verbose Makefile debugging
#MKVERBOSE = 1

# Default libraries to use
CURSESLIB = -lncurses -lutil

# Installation prefix
PREFIX ?= /usr/local

# Apparently needed for SH4 to avoid miscompilation
#CFLAGS += -fno-tree-dominator-opts

###############################################################################

# Default CFLAGS
CFLAGS += -pipe -std=gnu99 -I./cl -I./include -I. -MD

# CFLAGS and LDFLAGS
ifdef DEBUG_VI
   # Debugging
   CFLAGS  += -DDEBUG=1 -g3 -ggdb -Og
else
   # Optimizing
   CFLAGS  += -O2 -fomit-frame-pointer
   LDFLAGS += -s
endif

ifndef DEBUG_VI
    CFLAGS  += $(LTGC) $(LTOC)
    LDFLAGS += $(LTOC)
endif

LDFLAGS += $(CURSESLIB)

###############################################################################

# Default PREFIX to /usr/local if unset
ifndef PREFIX
    PREFIX = /usr/local
endif

###############################################################################

# Required POSIX tools
AWK   ?= awk
RMF   ?= rm -f
LNS   ?= ln -fs
MKDIR ?= mkdir -p
CHMOD ?= chmod
RMDIR ?= rmdir
TEST  ?= test
TRUE  ?= true
CP    ?= cp -f
CHOWN ?= chown

###############################################################################

VPATH = build:cl:common:ex:include:vi:bin

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
       cl/regfree.c cl/getopt_long.c

OBJS := ${SRCS:.c=.o}

DEPZ := ${OBJS:.o=.d}

###############################################################################

ifndef DB185
    unexport DB185EMU
    ifneq (,$(wildcard /usr/include/db_185.h))
        DB185EMU=1
        ifdef MKVERBOSE
            $(info **** Berkely DB 1.8.5 emulation enabled [/usr])
        endif
    endif
else
    ifdef MKVERBOSE
        $(info **** Berkeley DB 1.8.5 emulation forcefully disabled)
    endif
endif

ifndef DB185
    ifndef DB185EMU
        ifneq (,$(wildcard /usr/local/include/db_185.h))
            DB185EMU = 1
            CFLAGS  += -I/usr/local/include
            LDFLAGS += -L/usr/local/lib
            ifdef MKVERBOSE
                $(info **** Berkelely DB 1.8.5 emulation enabled [/usr/local])
            endif
        endif
    endif
endif

ifndef DB185
    ifneq (,$(wildcard /usr/lib/libdb-5.so))
        DBLIB ?= -ldb-5
    endif

    ifneq (,$(wildcard /usr/lib32/libdb-5.so))
        DBLIB ?= -ldb-5
    endif

    ifneq (,$(wildcard /usr/lib64/libdb-5.so))
        DBLIB ?= -ldb-5
    endif

    ifneq (,$(wildcard /usr/local/lib/libdb-5.so))
        DBLIB ?= -ldb-5
    endif

    ifneq (,$(wildcard /usr/lib/libdb-4.so))
        DBLIB ?= -ldb-4
    endif

    ifneq (,$(wildcard /usr/lib32/libdb-4.so))
        DBLIB ?= -ldb-4
    endif

    ifneq (,$(wildcard /usr/lib64/libdb-4.so))
        DBLIB ?= -ldb-4
    endif

    ifneq (,$(wildcard /usr/local/lib/libdb-4.so))
        DBLIB ?= -ldb-4
    endif

    ifneq (,$(wildcard /usr/lib/libdb-3.so))
        DBLIB ?= -ldb-3
    endif

    ifneq (,$(wildcard /usr/lib32/libdb-3.so))
        DBLIB ?= -ldb-3
    endif

    ifneq (,$(wildcard /usr/lib64/libdb-3.so))
        DBLIB ?= -ldb-3
    endif

    ifneq (,$(wildcard /usr/local/lib/libdb-3.so))
        DBLIB ?= -ldb-3
    endif
endif

DBLIB ?= -ldb

ifdef DB185EMU
    CFLAGS  += -DDB185EMU=1
endif

LDFLAGS += $(DBLIB)

###############################################################################

ifneq (,$(wildcard /usr/lib/libmtmalloc.so))
    MALLOC ?= -lmtmalloc
endif

ifneq (,$(wildcard /usr/lib/libjemalloc.so))
    MALLOC ?= -ljemalloc
endif

ifneq (,$(wildcard /usr/lib32/libjemalloc.so))
    MALLOC ?= -ljemalloc
endif

ifneq (,$(wildcard /usr/lib64/libjemalloc.so))
    MALLOC ?= -ljemalloc
endif

ifdef MALLOC
    LDFLAGS += $(MALLOC)
endif

###############################################################################

ifdef MKVERBOSE
    $(info **** Using CFLAGS: $(CFLAGS))
    $(info **** Using LDFLAGS: $(LDFLAGS))
endif

###############################################################################

all: vi ex view

ex/ex_def.h: ex/ex.awk ex/ex_cmd.c
	$(RMF) ./ex/ex_def.h; \
        $(AWK) -f ./ex/ex.awk ./ex/ex_cmd.c \
            > ./ex/ex_def.h && $(TEST) -f ./ex/ex_def.h

common/options_def.h: common/options.awk common/options.c ex/ex_def.h
	$(RMF) ./common/options_def.h; \
        $(AWK) -f ./common/options.awk ./common/options.c \
            > ./common/options_def.h && $(TEST) -f ./common/options_def.h

###############################################################################

clean:
	$(RMF) ./common/options_def.h ./ex/ex_def.h
	$(RMF) $(OBJS)
	$(RMF) $(DEPZ)
	$(RMF) ./bin/vi ./bin/ex ./bin/view
	$(RMDIR) ./bin

###############################################################################

%.o: %.c common/options_def.h ex/ex_def.h
	$(CC) -c -o $@ $< $(CFLAGS)

###############################################################################

bin/vi: common/options_def.h ex/ex_def.h $(OBJS)
	$(TEST) -d ./bin || $(MKDIR) ./bin
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: vi
vi: bin/vi
	@$(TRUE)

bin/ex: vi
	$(TEST) -d ./bin || $(MKDIR) ./bin
	$(LNS) vi ./bin/ex

.PHONY: ex
ex: bin/ex
	@$(TRUE); $(TRUE)

bin/view: vi
	$(TEST) -d ./bin || $(MKDIR) ./bin
	$(LNS) vi ./bin/view

.PHONY: view
view: bin/view
	@$(TRUE); $(TRUE); $(TRUE)

###############################################################################

install: vi ex view virecover
	$(TEST) -d /tmp/vi.recover || \
        $(MKDIR) /tmp/vi.recover
	$(TEST) -d /tmp/vi.recover && \
        $(CHOWN) root:root /tmp/vi.recover && \
        $(CHMOD) 1777 /tmp/vi.recover
	$(TEST) -d $(PREFIX)/bin || \
        $(MKDIR) $(PREFIX)/bin
	$(TEST) -d $(PREFIX)/libexec || \
        $(MKDIR) $(PREFIX)/libexec
	$(CP) ./bin/vi $(PREFIX)/bin/vi && \
        $(CHOWN) root:root $(PREFIX)/bin/vi && \
        $(CHMOD) 755 $(PREFIX)/bin/vi
	$(TEST) -x $(PREFIX)/bin/vi && \
        $(LNS) $(PREFIX)/bin/vi $(PREFIX)/bin/ex
	$(TEST) -x $(PREFIX)/bin/vi && \
        $(LNS) $(PREFIX)/bin/vi $(PREFIX)/bin/view
	$(CP) ./build/virecover $(PREFIX)/libexec/vi.recover && \
        $(CHMOD) 755 $(PREFIX)/libexec/vi.recover

###############################################################################

uninstall:
	$(RMF) $(PREFIX)/libexec/vi.recover
	$(RMF) $(PREFIX)/bin/vi
	$(RMF) $(PREFIX)/bin/ex
	$(RMF) $(PREFIX)/bin/view

###############################################################################

# EOF
