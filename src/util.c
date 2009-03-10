/*
 * gtk.c: GTK front end for my puzzle collection.
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <sys/time.h>

#include "puzzles.h"

#ifdef DEBUGGING
static FILE *debug_fp = NULL;

void dputs(char *buf)
{
/*    if (!debug_fp) {
        debug_fp = fopen("debug.log", "w");
    } */

    fputs(buf, stderr);

    if (debug_fp) {
        fputs(buf, debug_fp);
        fflush(debug_fp);
    }
}

void debug_printf(char *fmt, ...)
{
    char buf[4096];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    dputs(buf);
    va_end(ap);
}
#endif

/* ----------------------------------------------------------------------
 * Error reporting functions used elsewhere.
 */

void fatal(char *fmt, ...)
{
    va_list ap;

    fprintf(stderr, "fatal error: ");

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    fprintf(stderr, "\n");
    exit(1);
}

