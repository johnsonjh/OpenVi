# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 1980, 1986, 1987, 1988, 1989, 1990, 1991, 1992, 1993, 1994
#         The Regents of the University of California
# Copyright (c) 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000
#         Keith Bostic
# Copyright (c) 2021-2024 Jeffrey H. Johnson

BEGIN {
        printf("enum {");
        first = 1;
}
/^\/\* C_[0-9A-Z_]* \*\// {
        printf("%s\n\t%s%s", first ? "" : ",", $2, first ? " = 0" : "");
        first = 0;
        next;
}
END {
        printf("\n};\n");
}
