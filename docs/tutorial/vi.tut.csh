#!/usr/bin/env csh
#       $OpenBSD: vi.tut.csh,v 1.2 2001/01/29 01:58:40 niklas Exp $

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
