/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 2022-2024 Jeffrey H. Johnson <trnsz@pobox.com>
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

int cut(SCR *, CHAR_T *, MARK *, MARK *, int);
int cut_line(SCR *, recno_t, size_t, size_t, CB *);
void cut_close(GS *);
TEXT *text_init(SCR *, const char *, size_t, size_t);
void text_lfree(TEXTH *);
void text_free(TEXT *);
int del(SCR *, MARK *, MARK *, int);
FREF *file_add(SCR *, CHAR_T *);
int file_init(SCR *, FREF *, char *, int);
int file_end(SCR *, EXF *, int);
int file_write(SCR *, MARK *, MARK *, char *, int);
int file_m1(SCR *, int, int);
int file_m2(SCR *, int);
int file_m3(SCR *, int);
int file_aw(SCR *, int);
void set_alt_name(SCR *, char *);
lockr_t file_lock(SCR *, char *, int *, int, int);
int v_key_init(SCR *);
void v_key_ilookup(SCR *);
size_t v_key_len(SCR *, CHAR_T);
CHAR_T *v_key_name(SCR *, CHAR_T);
int v_key_val(SCR *, CHAR_T);
int v_event_push(SCR *, EVENT *, CHAR_T *, size_t, unsigned int);
int v_event_get(SCR *, EVENT *, int, u_int32_t);
void v_event_err(SCR *, EVENT *);
int v_event_flush(SCR *, unsigned int);
int db_eget(SCR *, recno_t, char **, size_t *, int *);
int db_get(SCR *, recno_t, u_int32_t, char **, size_t *);
int db_delete(SCR *, recno_t);
int db_append(SCR *, int, recno_t, char *, size_t);
int db_insert(SCR *, recno_t, char *, size_t);
int db_set(SCR *, recno_t, char *, size_t);
int db_exist(SCR *, recno_t);
int db_last(SCR *, recno_t *);
void db_err(SCR *, recno_t);
int log_init(SCR *, EXF *);
int log_end(SCR *, EXF *);
int log_cursor(SCR *);
int log_line(SCR *, recno_t, unsigned int);
int log_mark(SCR *, LMARK *);
int log_backward(SCR *, MARK *);
int log_setline(SCR *);
int log_forward(SCR *, MARK *);
int editor(GS *, int, char *[]);
void v_end(GS *);
int mark_init(SCR *, EXF *);
int mark_end(SCR *, EXF *);
int mark_get(SCR *, CHAR_T, MARK *, mtype_t);
int mark_set(SCR *, CHAR_T, MARK *, int);
int mark_insdel(SCR *, lnop_t, recno_t);
void msgq(SCR *, mtype_t, const char *, ...);
void msgq_str(SCR *, mtype_t, char *, char *);
void mod_rpt(SCR *);
void msgq_status(SCR *, recno_t, unsigned int);
const char *msg_cmsg(SCR *, cmsg_t, size_t *);
char *msg_print(SCR *, const char *, int *);
int opts_init(SCR *, int *);
int opts_set(SCR *, ARGS *[], char *);
int o_set(SCR *, int, unsigned int, char *, unsigned long);
int opts_empty(SCR *, int, int);
void opts_dump(SCR *, enum optdisp);
int opts_save(SCR *, FILE *);
OPTLIST const *opts_search(char *);
void opts_nomatch(SCR *, char *);
int opts_copy(SCR *, SCR *);
void opts_free(SCR *);
int f_altwerase(SCR *, OPTION *, char *, unsigned long *);
int f_columns(SCR *, OPTION *, char *, unsigned long *);
int f_lines(SCR *, OPTION *, char *, unsigned long *);
int f_paragraph(SCR *, OPTION *, char *, unsigned long *);
int f_print(SCR *, OPTION *, char *, unsigned long *);
int f_readonly(SCR *, OPTION *, char *, unsigned long *);
int f_recompile(SCR *, OPTION *, char *, unsigned long *);
int f_reformat(SCR *, OPTION *, char *, unsigned long *);
int f_section(SCR *, OPTION *, char *, unsigned long *);
int f_secure(SCR *, OPTION *, char *, unsigned long *);
int f_ttywerase(SCR *, OPTION *, char *, unsigned long *);
int f_w300(SCR *, OPTION *, char *, unsigned long *);
int f_w1200(SCR *, OPTION *, char *, unsigned long *);
int f_w9600(SCR *, OPTION *, char *, unsigned long *);
int f_window(SCR *, OPTION *, char *, unsigned long *);
int put(SCR *, CB *, CHAR_T *, MARK *, MARK *, int, int);
int rcv_tmp(SCR *, EXF *, char *);
int rcv_init(SCR *);
int rcv_sync(SCR *, unsigned int);
int rcv_list(SCR *);
int rcv_read(SCR *, FREF *);
int screen_init(GS *, SCR *, SCR **);
int screen_end(SCR *);
SCR *screen_next(SCR *);
int f_search(SCR *, MARK *, MARK *, char *, size_t, char **, unsigned int);
int b_search(SCR *, MARK *, MARK *, char *, size_t, char **, unsigned int);
void search_busy(SCR *, busy_t);
int seq_set(SCR *, CHAR_T *,
size_t, CHAR_T *, size_t, CHAR_T *, size_t, seq_t, int);
int seq_delete(SCR *, CHAR_T *, size_t, seq_t);
int seq_mdel(SEQ *);
SEQ *seq_find
(SCR *, SEQ **, EVENT *, CHAR_T *, size_t, seq_t, int *);
void seq_close(GS *);
int seq_dump(SCR *, seq_t, int);
int seq_save(SCR *, FILE *, char *, seq_t);
int e_memcmp(CHAR_T *, EVENT *, size_t);
void *binc(SCR *, void *, size_t *, size_t);
int nonblank(SCR *, recno_t, size_t *);
CHAR_T *v_strdup(SCR *, const CHAR_T *, size_t);
enum nresult nget_uslong(unsigned long *, const char *, char **, int);
enum nresult nget_slong(long *, const char *, char **, int);
void TRACE(SCR *, const char *, ...);
