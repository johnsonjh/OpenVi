#!/usr/bin/env csh
# $OpenBSD: vi.tut.csh,v 1.2 2001/01/29 01:58:40 niklas Exp $
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 1980, 1986, 1987, 1988, 1989, 1990, 1991, 1992, 1993, 1994
#         The Regents of the University of California
# Copyright (c) 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000
#         Keith Bostic
# Copyright (c) 2021-2024 Jeffrey H. Johnson <trnsz@pobox.com>

# XXX I don't know what happens if there is .exrc file!
# XXX Make sure that user is using a 24 line window!!!

if ($1 != "beginner" && $1 != "advanced") then
        echo Usage: $0 beginner or $0 advanced
        exit
endif # ($1 != "beginner" && $1 != "advanced")

if ($?EXINIT) then
        set oexinit="$EXINIT"
        setenv EXINIT 'se ts=4 wm=8 sw=4'
endif # $?EXINIT

vi vi.{$1}

onintr:

if ($?oexinit) then
                setenv EXINIT "$oexinit"
endif # $?oexinit
