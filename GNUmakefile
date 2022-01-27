###############################################################################
#                               -  O p e n V i  -                             #
###############################################################################

# Default compiler settings
CC          ?= cc
OPTLEVEL    ?= -O2
DEPFLAGS    ?= -MMD -MP
CFLAGS      += -std=gnu99 -Iinclude -Idb -Icl -Iex -Ivi -Icommon

###############################################################################

# Set DEBUG to enable debugging build
#DEBUG       = 1
DBGFLAGS    ?= -ggdb -g3 -Og

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

# Default libraries to link
LINKLIBS    ?= -lutil -lncurses $(EXTRA_LIBS)

###############################################################################

# Installation directory prefix for install/uninstall
PREFIX      ?= /usr/local

###############################################################################

# Executable prefix and/or suffix (e.g. 'o', '-openbsd') for install/uninstall
BINPREFIX   ?= o
#BINSUFFIX  ?= -openbsd

###############################################################################

# Permissions and user:group to use for installed executables
IPERM        = 755
IUSGR        = root:bin

###############################################################################

ifdef DEBUG
   CFLAGS   += $(DBGFLAGS) -Wall -Wextra -DDEBUG -DSTATISTICS -DHASH_STATISTICS
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

AWK         ?= awk
CHMOD       ?= chmod
CHOWN       ?= chown
CP          ?= cp -f
LN          ?= ln
LNS          = $(LN) -fs
MKDIR       ?= mkdir -p
PAWK         = command -p env PATH="$$(command -p getconf PATH)" $(AWK)
PRINTF      ?= printf
RMDIR       ?= rmdir
RM          ?= rm
RMF          = $(RM) -f
SLEEP       ?= sleep
STRIP       ?= strip
TEST        ?= test
TRUE        ?= true

###############################################################################

ifdef V
	DEBUG = 1
endif # V

###############################################################################

ifdef DEBUG
    VERBOSE = set -ex
else
    VERBOSE = $(TRUE)
endif # DEBUG

###############################################################################

SRCS = cl/basename.c \
	   cl/cl_funcs.c \
	   cl/cl_main.c \
	   cl/cl_read.c \
	   cl/cl_screen.c \
	   cl/cl_term.c \
	   cl/getopt_long.c \
	   cl/getprogname.c \
       cl/pledge.c \
	   cl/regcomp.c \
	   cl/regerror.c \
	   cl/regexec.c \
       cl/regfree.c \
	   cl/strlcpy.c \
	   cl/strtonum.c \
       common/cut.c \
	   common/delete.c \
	   common/exf.c \
       common/key.c \
	   common/line.c \
	   common/log.c \
	   common/main.c \
	   common/mark.c \
       common/msg.c \
	   common/options.c \
	   common/options_f.c \
	   common/put.c \
       common/recover.c \
	   common/screen.c \
	   common/search.c \
	   common/seq.c \
       common/util.c \
	   db/btree/bt_close.c \
       db/btree/bt_conv.c \
	   db/btree/bt_debug.c \
	   db/btree/bt_delete.c \
       db/btree/bt_get.c \
	   db/btree/bt_open.c \
	   db/btree/bt_overflow.c \
       db/btree/bt_page.c \
	   db/btree/bt_put.c \
	   db/btree/bt_search.c \
       db/btree/bt_seq.c \
	   db/btree/bt_split.c \
	   db/btree/bt_utils.c \
	   db/db/db.c \
       db/hash/hash_bigkey.c \
	   db/hash/hash_buf.c \
	   db/hash/hash.c \
       db/hash/hash_func.c \
	   db/hash/hash_log2.c \
	   db/hash/hash_page.c \
       db/hash/ndbm.c \
	   db/mpool/mpool.c \
	   db/recno/rec_close.c \
       db/recno/rec_delete.c \
	   db/recno/rec_get.c \
	   db/recno/rec_open.c \
       db/recno/rec_put.c \
	   db/recno/rec_search.c \
	   db/recno/rec_seq.c \
       db/recno/rec_utils.c \
	   db/sys/issetugid.c \
	   db/sys/open.c \
	   ex/ex_abbrev.c \
	   ex/ex_append.c \
       ex/ex_args.c \
	   ex/ex_argv.c \
	   ex/ex_at.c \
	   ex/ex_bang.c \
	   ex/ex.c \
	   ex/ex_cd.c \
       ex/ex_cmd.c \
	   ex/ex_delete.c \
	   ex/ex_display.c \
	   ex/ex_edit.c \
	   ex/ex_equal.c \
       ex/ex_file.c \
	   ex/ex_filter.c \
	   ex/ex_global.c \
	   ex/ex_init.c \
	   ex/ex_join.c \
       ex/ex_map.c \
	   ex/ex_mark.c \
	   ex/ex_mkexrc.c \
	   ex/ex_move.c \
	   ex/ex_open.c \
       ex/ex_preserve.c \
	   ex/ex_print.c \
	   ex/ex_put.c \
	   ex/ex_quit.c \
	   ex/ex_read.c \
       ex/ex_screen.c \
	   ex/ex_script.c \
	   ex/ex_set.c \
	   ex/ex_shell.c \
	   ex/ex_shift.c \
       ex/ex_source.c \
	   ex/ex_stop.c \
	   ex/ex_subst.c \
	   ex/ex_tag.c \
	   ex/ex_txt.c \
       ex/ex_undo.c \
	   ex/ex_usage.c \
	   ex/ex_util.c \
	   ex/ex_version.c \
	   ex/ex_visual.c \
       ex/ex_write.c \
	   ex/ex_yank.c \
	   ex/ex_z.c \
	   vi/getc.c \
	   vi/v_at.c \
	   vi/v_ch.c \
	   vi/v_cmd.c \
	   vi/v_delete.c \
	   vi/v_ex.c \
       vi/vi.c \
       vi/v_increment.c \
	   vi/v_init.c \
	   vi/v_itxt.c \
	   vi/v_left.c \
	   vi/v_mark.c \
       vi/v_match.c \
	   vi/v_paragraph.c \
	   vi/v_put.c \
	   vi/v_redraw.c \
	   vi/v_replace.c \
       vi/v_right.c \
	   vi/v_screen.c \
	   vi/v_scroll.c \
	   vi/v_search.c \
	   vi/v_section.c \
       vi/v_sentence.c \
	   vi/vs_line.c \
	   vi/vs_msg.c \
	   vi/vs_refresh.c \
	   vi/vs_relative.c \
       vi/vs_smap.c \
	   vi/vs_split.c \
	   vi/v_status.c \
	   vi/v_txt.c \
	   vi/v_ulcase.c \
	   vi/v_undo.c \
       vi/v_util.c \
	   vi/v_word.c \
	   vi/v_xchar.c \
	   vi/v_yank.c \
	   vi/v_z.c \
	   vi/v_zexit.c

###############################################################################

VPATH = build:cl:common:db:ex:include:vi:bin
OBJS := ${SRCS:.c=.o}
DEPS := ${OBJS:.o=.d}

###############################################################################

all: bin/vi bin/ex bin/view docs/USD.doc/vi.man/vi.1

###############################################################################

ex/ex_def.h: ex/ex.awk ex/ex_cmd.c
ifndef DEBUG
	-@$(PRINTF) "\t$(AWK):\t%s\n" "ex/ex.awk"
endif
	@$(VERBOSE); $(RMF) "./ex/ex_def.h"; \
        $(PAWK) -f \
            "./ex/ex.awk" "./ex/ex_cmd.c" \
                > "./ex/ex_def.h" && \
                    $(TEST) -f "./ex/ex_def.h"

###############################################################################

common/options_def.h: common/options.awk common/options.c ex/ex_def.h
ifndef DEBUG
	-@$(PRINTF) "\t$(AWK):\t%s\n" "command/options.awk"
endif
	@$(VERBOSE); $(RMF) "./common/options_def.h"; \
        $(PAWK) -f \
            "./common/options.awk" "./common/options.c" \
                > "./common/options_def.h" && \
                    $(TEST) -f "./common/options_def.h"

###############################################################################

clean distclean realclean mostlyclean maintainer-clean:
ifndef DEBUG
	-@$(PRINTF) '\t%s\t%s\n' "rm:" "common/options_def.h"
endif
	@$(VERBOSE); $(RMF) "./common/options_def.h"
ifndef DEBUG
	-@$(PRINTF) '\t%s\t%s\n' "rm:" "ex/ex_def.h"
endif
	@$(VERBOSE); $(RMF) "./ex/ex_def.h"
ifndef DEBUG
	-@$(PRINTF) '\t%s\t%s\n' "rm:" "objects"
endif
	@$(VERBOSE); $(RMF) $(OBJS)
ifndef DEBUG
	-@$(PRINTF) '\t%s\t%s\n' "rm:" "dependencies"
endif
	@$(VERBOSE); $(RMF) $(DEPS)
ifndef DEBUG
	-@$(PRINTF) '\t%s\t%s\n' "rm:" "bin/vi"
endif
	@$(VERBOSE); $(TEST) -f "./bin/vi"   && $(RMF)   "./bin/vi"   || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) '\t%s\t%s\n' "rm:" "bin/ex"
endif
	@$(VERBOSE); $(TEST) -e "./bin/ex"   && $(RMF)   "./bin/ex"   || $(TRUE)
	@$(VERBOSE); $(TEST) -h "./bin/ex"   && $(RMF)   "./bin/ex"   || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) '\t%s\t%s\n' "rm:" "bin/view"
endif
	@$(VERBOSE); $(TEST) -e "./bin/view" && $(RMF)   "./bin/view" || $(TRUE)
	@$(VERBOSE); $(TEST) -h "./bin/view" && $(RMF)   "./bin/view" || $(TRUE)
ifndef DEBUG
	-@$(PRINTF) '\t%s\t%s\n' "$(RMDIR):" "bin"
endif
	@$(VERBOSE); $(TEST) -d "./bin"      && $(RMDIR)   "./bin"      || $(TRUE)
	-@$(TEST) -d "./bin"    && $(PRINTF) '\t%s\t%s\n' \
        "WARNING:" "Directory './bin' not removed." || $(TRUE)

###############################################################################

%.o: %.c common/options_def.h ex/ex_def.h
ifndef DEBUG
	-@$(PRINTF) "\t$(CC):\t%s\n" "$@"
endif # DEBUG
	@$(VERBOSE); $(CC) $(CFLAGS) $(DEPFLAGS) -c -o "$@" "$<"
-include $(wildcard $(DEPS))

###############################################################################

bin/vi: $(OBJS) common/options_def.h ex/ex_def.h
	@$(TEST) -d "./bin" || $(MKDIR) "./bin"
ifndef DEBUG
	-@$(PRINTF) '\t$(LD):\t%s\n' "$@"
endif # DEBUG
	@$(VERBOSE); $(CC) $(LDFLAGS) -o "$@" $^

.PHONY: vi
vi: bin/vi
	-@$(TRUE)

###############################################################################

bin/ex: bin/vi
ifndef DEBUG
	-@$(PRINTF) "\t$(LN):\t%s\n" "$@"
endif # DEBUG
	@$(VERBOSE); $(LNS) "vi" "./bin/ex"

.PHONY: ex
ex: bin/ex
	-@$(TRUE)

##############################################################################

bin/view: bin/vi
ifndef DEBUG
	-@$(PRINTF) "\t$(LN):\t%s\n" "$@"
endif # DEBUG
	@$(VERBOSE); $(LNS) "vi" "./bin/view"

.PHONY: view
view: bin/view
	-@$(TRUE)

###############################################################################

install: bin/vi bin/ex bin/view scripts/virecover docs/USD.doc/vi.man/vi.1
ifndef DEBUG
	-@$(PRINTF) "\t%s\t%s\n" "mkdir:" "/tmp/vi.recover"
endif
	@$(VERBOSE); $(TEST) -d "/tmp/vi.recover" || { \
        $(MKDIR) "/tmp/vi.recover" && $(TEST) -d "/tmp/vi.recover" && \
            $(CHOWN) "$(IUSGR)" "/tmp/vi.recover" && \
                $(CHMOD) "1777" "/tmp/vi.recover"; }
ifndef DEBUG
	-@$(PRINTF) "\t%s\t%s\n" "mkdir:" "$(PREFIX)/bin"
endif
	@$(VERBOSE); $(TEST) -d "$(PREFIX)/bin" || $(MKDIR) "$(PREFIX)/bin"
ifndef DEBUG
	-@$(PRINTF) "\t%s\t%s\n" "mkdir:" "$(PREFIX)/libexec"
endif
	@$(VERBOSE); $(TEST) -d "$(PREFIX)/libexec" || $(MKDIR) "$(PREFIX)/libexec"
ifndef DEBUG
	-@$(PRINTF) "\t%s\t%s\n" "mkdir:" "$(PREFIX)/man/man1"
endif
	@$(VERBOSE); $(TEST) -d "$(PREFIX)/man/man1" && $(MKDIR) "$(PREFIX)/man/man1"
ifndef DEBUG
	-@$(PRINTF) "\t%s\t%s\n" "cp:" "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"
endif
	@$(VERBOSE); $(CP) "./bin/vi" "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" && \
        $(CHOWN) "$(IUSGR)" "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" && \
            $(CHMOD) "$(IPERM)" "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\t%s\t%s\n" "ln:" "$(PREFIX)/bin/$(BINPREFIX)ex$(BINSUFFIX)"
endif
	@$(VERBOSE); $(TEST) -x "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" && \
        $(LNS) "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" \
            "$(PREFIX)/bin/$(BINPREFIX)ex$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\t%s\t%s\n" "ln:" "$(PREFIX)/bin/$(BINPREFIX)view$(BINSUFFIX)"
endif
	@$(VERBOSE); $(TEST) -x "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" && \
        $(LNS) "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)" \
            "$(PREFIX)/bin/$(BINPREFIX)view$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\t%s\t%s\n" "cp:" "$(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)"
endif
	@$(VERBOSE); $(CP) "./scripts/virecover" \
        "$(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)" && \
            $(CHMOD) "$(IPERM)" \
                "$(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\t%s\t%s\n" "cp:" "$(PREFIX)/man/man1/$(BINPREFIX){vi,ex,view}$(BINSUFFIX).1"
endif
	@$(VERBOSE); $(CP) "docs/USD.doc/vi.man/vi.1" \
        "$(PREFIX)/man/man1/$(BINPREFIX)vi$(BINSUFFIX).1" && \
            $(LNS) "$(PREFIX)/man/man1/$(BINPREFIX)vi$(BINSUFFIX).1" \
                "$(PREFIX)/man/man1/$(BINPREFIX)view$(BINSUFFIX).1" && \
                    $(LNS) "$(PREFIX)/man/man1/$(BINPREFIX)vi$(BINSUFFIX).1" \
                        "$(PREFIX)/man/man1/$(BINPREFIX)ex$(BINSUFFIX).1"

###############################################################################

install-strip: install
ifndef DEBUG
	-@$(PRINTF) "\t$(STRIP):\t%s\n" "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"
endif # DEBUG
	@$(VERBOSE); $(STRIP) "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"

###############################################################################

strip: bin/vi
ifndef DEBUG
	-@$(PRINTF) "\t$(STRIP):\t%s\n" "bin/vi"
endif # DEBUG
	@$(VERBOSE); $(STRIP) "./bin/vi"

###############################################################################

uninstall:
ifndef DEBUG
	-@$(PRINTF) "\t$(RM):\t%s\n" \
        "$(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)"
endif # DEBUG
	-@$(VERBOSE); $(RMF) "$(PREFIX)/libexec/$(BINPREFIX)vi.recover$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\t$(RM):\t%s\n" \
        "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"
endif # DEBUG
	-@$(VERBOSE); $(RMF) "$(PREFIX)/bin/$(BINPREFIX)vi$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\t$(RM):\t%s\n" \
        "$(PREFIX)/bin/$(BINPREFIX)ex$(BINSUFFIX)"
endif # DEBUG
	-@$(VERBOSE); $(RMF) "$(PREFIX)/bin/$(BINPREFIX)ex$(BINSUFFIX)"
ifndef DEBUG
	-@$(PRINTF) "\t$(RM):\t%s\n" \
        "$(PREFIX)/bin/$(BINPREFIX)view$(BINSUFFIX)"
endif # DEBUG
	-@$(VERBOSE); $(RMF) "$(PREFIX)/bin/$(BINPREFIX)view$(BINSUFFIX)"

###############################################################################
