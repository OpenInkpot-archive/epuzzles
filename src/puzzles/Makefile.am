SUBDIRS = .
noinst_LTLIBRARIES = libcore.la libpuzzles.la

CFLAGS = -g3 -DCOMBINED -DDEBUGGING -DEINK_SCREEN -I${top_srcdir}/src

libcore_la_SOURCES = drawing.c malloc.c dsf.c misc.c divvy.c combi.c random.c \
    midend.c printing.c tree234.c latin.c maxflow.c

libpuzzles_la_SOURCES = fifteen.c twiddle.c pegs.c flip.c bridges.c pegs.c \
    unequal.c solo.c rect.c slant.c
