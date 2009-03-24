SUBDIRS = puzzles .
bin_PROGRAMS = epuzzle
noinst_LTLIBRARIES = libgui.la

epuzzle_SOURCES = main.c gamelist.c

libgui_la_SOURCES = util.c gui_util.c timer.c draw.c keyb.c
libgui_la_LIBADD = puzzles/libcore.la puzzles/libpuzzles.la
all_libraries = libgui.la

all_includes = -I${top_srcdir} -I${top_srcdir}/src -I${top_srcdir}/src/puzzles

# set the include path found by configure
CFLAGS = -DCOMBINED -DDEBUGGING
INCLUDES= $(all_includes) @ECORE_CFLAGS@ @ECORE_FILE_CFLAGS@ @ECORE_EVAS_CFLAGS@ @EDJE_CFLAGS@ @EVAS_CFLAGS@ @EWL_CFLAGS@ @EDRAWABLE_CFLAGS@

# the library search path.
epuzzle_LDFLAGS = $(all_libraries) @ECORE_LIBS@ @ECORE_FILE_LIBS@ @EVAS_LIBS@ @EDJE_LIBS@ @EWL_LIBS@ @ECORE_EVAS_LIBS@ @EDRAWABLE_LIBS@
