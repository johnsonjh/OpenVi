/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 2022-2024 Jeffrey H. Johnson
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the names of the copyright holders nor the names of any
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

int cl_addstr(SCR *, const char *, size_t);
int cl_attr(SCR *, scr_attr_t, int);
int cl_baud(SCR *, unsigned long *);
int cl_bell(SCR *);
int cl_clrtoeol(SCR *);
int cl_cursor(SCR *, size_t *, size_t *);
int cl_deleteln(SCR *);
int cl_ex_adjust(SCR *, exadj_t);
int cl_insertln(SCR *);
int cl_keyval(SCR *, scr_keyval_t, CHAR_T *, int *);
int cl_move(SCR *, size_t, size_t);
int cl_refresh(SCR *, int);
int cl_rename(SCR *, char *, int);
int cl_suspend(SCR *, int *);
void cl_usage(void);
int sig_init(GS *, SCR *);
int cl_event(SCR *, EVENT *, u_int32_t, int);
int cl_screen(SCR *, u_int32_t);
int cl_quit(GS *);
int cl_getcap(SCR *, char *, char **);
int cl_term_init(SCR *);
int cl_term_end(GS *);
int cl_fmap(SCR *, seq_t, CHAR_T *, size_t, CHAR_T *, size_t);
int cl_optchange(SCR *, int, char *, unsigned long *);
int cl_omesg(SCR *, CL_PRIVATE *, int);
int cl_ssize(SCR *, int, size_t *, size_t *, int *);
int cl_putchar(int);
