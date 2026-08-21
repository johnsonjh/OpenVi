# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 1980, 1986, 1987, 1988, 1989, 1990, 1991, 1992, 1993, 1994
#         The Regents of the University of California
# Copyright (c) 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000
#         Keith Bostic
# Copyright (c) 2021-2026 Jeffrey H. Johnson

BEGIN {
        printf("#include <glob.h>\n");
        printf("enum {\n");
        first = 1;
}

/^#if.*OPT/ {
        print;
        next;
}

/^#el.*OPT/ {
        print;
        next;
}

/^#endif.*OPT/ {
        print;
        next;
}

/^\/\* O_[0-9A-Z_]*/ {
        printf("\t%s%s,\n", $2, first ? " = 0" : "");
        first = 0;
        next;
}

END {
        printf("\tO_OPTIONCOUNT\n};\n");
}
