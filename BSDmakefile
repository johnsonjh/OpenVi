###############################################################################
#                               -  O p e n V i  -                             #
###############################################################################
# vim: filetype=make:tabstop=8:tw=79:noexpandtab:colorcolumn=79
# SPDX-License-Identifier: BSD-3-Clause
###############################################################################

###############################################################################
#
# Copyright (c) 2021-2023 Jeffrey H. Johnson <trnsz@pobox.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered "AS-IS",
# without any warranty.
#
###############################################################################

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
