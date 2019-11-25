/*
 * MIT License
 * 
 * Copyright (c) 2019 Antony jr
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
#ifndef CUTELOG_H_INCLUDED
#define CUTELOG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

struct _cutelog_t;
typedef struct _cutelog_t* cutelog_t;

typedef enum {
    cutelog_no_mode,
    cutelog_multiline_mode,
    cutelog_non_multiline_mode
} cutelog_mode_t;

cutelog_t cutelog_new();
cutelog_t cutelog_new_ex(void*);
void cutelog_free(cutelog_t);

int cutelog_mode(cutelog_t, cutelog_mode_t);
int cutelog_safe_finish(cutelog_t);

int cutelog_progress(cutelog_t, const char*, ...);
int cutelog_success(cutelog_t, const char*, ...);
int cutelog_fail(cutelog_t, const char*, ...);
int cutelog_info(cutelog_t, const char*, ...);
int cutelog_fatal(cutelog_t, const char*, ...);
int cutelog_warning(cutelog_t, const char*, ...);
int cutelog_debug(cutelog_t, const char*, ...);

#ifdef __cplusplus
}
#endif

#endif /* CUTELOG_H_INCLUDED */
