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
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <cutelog.h>

/* Emojis */
const char *unicorn_emoji = "🦄";
const char *heavy_check_mark_emoji = "✅";
const char *crossmark_emoji = "❌";
const char *lightbulb_emoji = "💡";
const char *stopsign_emoji = "🛑";
const char *warning_emoji = "🔔";
const char *debug_emoji = "💽";
/* --- */

typedef enum {
    progress,
    success,
    fail,
    info,
    fatal,
    warning,
    debug
} print_type_t;

static const char *prog_series = "\\|/-";
static int prog_series_len = 4;

struct _cutelog_t {
    FILE *output;
    cutelog_mode_t mode;
    int prog_series_pos;
    int prev_print_size;
};
#define CUTELOG_SZ (sizeof(struct _cutelog_t))

static char *get_time() {
    time_t current_time;
    struct tm * time_info;
    static char time_string[9] = {0};
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);
    return time_string;
}

static int print(cutelog_t ctx, print_type_t type, const char *fmt, va_list ap) {
    int r = -1;
    char *space = (char*)"";
    if(ctx == NULL || !fmt)
        return r;

    if(ctx->mode == cutelog_non_multiline_mode)
        fflush(ctx->output);

    r += fprintf(ctx->output, " [ %s ] ",
                 get_time());
    switch(type) {
    case progress:
        r += fprintf(ctx->output, "(%s \033[35mProgress\033[0m ", unicorn_emoji);
        break;
    case success:
        r += fprintf(ctx->output, "(%s \033[32mSuccess\033[0m", heavy_check_mark_emoji);
        space = (char*)"  ";
        break;
    case fail:
        r += fprintf(ctx->output, "(%s \033[31mFailed\033[0m", crossmark_emoji);
        space = (char*)"   ";
        break;
    case info:
        r += fprintf(ctx->output, "(%s \033[34mInfo\033[0m", lightbulb_emoji);
        space = (char*)"     ";
        break;
    case fatal:
        r += fprintf(ctx->output, "(%s \033[47;31mFatal\033[0m", stopsign_emoji);
        space = (char*)"    ";
        break;
    case warning:
        r += fprintf(ctx->output, "(%s \033[33mWarning\033[0m", warning_emoji);
        space = (char*)"  ";
        break;
    case debug:
        r += fprintf(ctx->output, "(%s \033[36mDebug\033[0m", debug_emoji);
        space = (char*)"    ";
        break;
    default:
        r += fprintf(ctx->output, "(Unknown");
        break;
    }
    if(ctx->mode == cutelog_non_multiline_mode) {
        if(type == progress) {
            r+= fprintf(ctx->output, "%c",
                        prog_series[ctx->prog_series_pos]);
            ctx->prog_series_pos =
                (ctx->prog_series_pos + 1) % prog_series_len;
        }
    }
    if(type != progress){
	    r += fprintf(ctx->output, " ) %s: ", space);
    }else{
	    r += fprintf(ctx->output, " )%s: ",space);
    }

    r += vfprintf(ctx->output, fmt, ap);

    if(ctx->mode == cutelog_non_multiline_mode) {
        if(ctx->prev_print_size > r) {
            do {
                int fill = ctx->prev_print_size - r + 4;
                while(fill--)
                    putc(' ', ctx->output);
            } while(0);
        }
        r+= fprintf(ctx->output, "\r\r");
    } else
        r+= fprintf(ctx->output, "\n");
    return r + 1;

}

#define CREATE_FUNCTION(name) int cutelog_##name(cutelog_t ctx, const char *fmt, ...){ \
	                         va_list ap; \
	                         int r = 0; \
	                         va_start(ap, fmt); \
	                         r = print(ctx, name , fmt, ap); \
	                         ctx->prev_print_size = r; \
	                         va_end(ap); \
	                         return r; \
	                      }


cutelog_t cutelog_new() {
    return cutelog_new_ex(stdout);
}

cutelog_t cutelog_new_ex(void *out) {
    cutelog_t r = (struct _cutelog_t*)calloc(1, CUTELOG_SZ);
    if(r == NULL)
        return NULL;
    r->output = (!out) ? (FILE*)stdout : (FILE*)out;
    r->mode = cutelog_multiline_mode; /* default */
    return r;
}

void cutelog_free(cutelog_t ctx) {
    if(ctx == NULL)
        return;
    free(ctx);
}


int cutelog_mode(cutelog_t ctx, cutelog_mode_t mode) {
    if(ctx == NULL)
        return -1;

    if(ctx->mode == cutelog_non_multiline_mode &&
            mode == cutelog_multiline_mode) {
        fflush(ctx->output);
        /* avoid overlapping between multiline and non multiline. */
        do { /* code block in c89 version, do not take this away, its not useless. */
            int fill = ctx->prev_print_size + 4;
            while(fill--)
                putc(' ', ctx->output);
            fprintf(ctx->output, "\r\r");
            fflush(ctx->output);
        } while(0);
    } else {
        fflush(ctx->output);
        fprintf(ctx->output, "\r\r");
        fflush(ctx->output);
    }
    ctx->mode = mode;
    return 0;
}

int cutelog_safe_finish(cutelog_t ctx) {
    if(ctx == NULL)
        return -1;

    if(ctx->mode == cutelog_non_multiline_mode)
        fprintf(ctx->output, "\n");
    return 0;
}

CREATE_FUNCTION(progress);
CREATE_FUNCTION(success);
CREATE_FUNCTION(fail);
CREATE_FUNCTION(info);
CREATE_FUNCTION(fatal);
CREATE_FUNCTION(warning);
CREATE_FUNCTION(debug);
