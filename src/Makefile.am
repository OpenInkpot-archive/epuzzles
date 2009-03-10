bin_PROGRAMS = epuzzle
noinst_LTLIBRARIES = libgui.la

epuzzle_SOURCES = main.c 
epuzzle_LDADD = libgui.la

libgui_la_SOURCES = util.c

all_includes = -I${top_srcdir} -I${top_srcdir}/src -I${top_srcdir}/src/puzzles

# set the include path found by configure
INCLUDES= $(all_includes) @ECORE_CFLAGS@ @ECORE_FILE_CFLAGS@ @ECORE_EVAS_CFLAGS@ @EDJE_CFLAGS@ @EVAS_CFLAGS@ @EWL_CFLAGS@

# the library search path.
epuzzle_LDFLAGS = $(all_libraries) @ECORE_LIBS@ @ECORE_FILE_LIBS@ @EVAS_LIBS@ @EDJE_LIBS@ @EWL_LIBS@ @ECORE_EVAS_LIBS@
