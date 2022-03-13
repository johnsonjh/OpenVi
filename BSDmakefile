###############################################################################
#                               -  O p e n V i  -                             #
###############################################################################
# vi: filetype=make:tabstop=8:tw=79

###############################################################################
# Configuration

.SHELL: name=sh
.MAIN: all
.MAKE.JOBS ?= 1
.NOTPARALLEL: _FAIL all _GMAKE $(.TARGETS)
.PHONY: _FAIL all _GMAKE $(.TARGETS)
$(.TARGETS): _GMAKE

###############################################################################
# Wrapper

_GMAKE:
	@command -v gmake > /dev/null 2>&1 ||				\
	{								\
		printf '\rError: %s\n' "GNU Make is required." 1>&2;	\
		exit 1;							\
	} &&								\
	command gmake 							\
		$$(printf '%s' "$(MAKEFLAGS)"  2> /dev/null |		\
			sed -e 's/-J .* //' 				\
				-e 's/-J.* //' 2> /dev/null)		\
		$(.TARGETS)

###############################################################################
