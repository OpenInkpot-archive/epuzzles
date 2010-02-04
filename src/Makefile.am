noinst_LTLIBRARIES = libgui.la
SUBDIRS = puzzles .
bin_PROGRAMS = epuzzles
AM_CPPFLAGS = -DTHEME_DIR='"$(pkgdatadir)/themes/"' -DCOLOR_CONFIG='"$(pkgdatadir)/epuzzles.rc"'
epuzzles_SOURCES = main.c gamelist.c help.c

libgui_la_SOURCES = util.c gui_util.c timer.c draw.c keyb.c sprites.c custom_drawable.c holder.c
libgui_la_LIBADD = puzzles/libcore.la puzzles/libpuzzles.la
all_libraries = libgui.la

all_includes = -I${top_srcdir} -I${top_srcdir}/src -I${top_srcdir}/src/puzzles

# set the include path found by configure
CFLAGS = -g3 -DCOMBINED -DDEBUGGING -DEINK_SCREEN
INCLUDES= $(all_includes) @ECORE_CFLAGS@ @ECORE_FILE_CFLAGS@ @ECORE_EVAS_CFLAGS@ @EDJE_CFLAGS@ @EVAS_CFLAGS@ @EDRAWABLE_CFLAGS@ @XCB_AUX_CFLAGS@ @EFREET_CFLAGS@ @LIBKEYS_CFLAGS@ @LIBEOI_CFLAGS@ @CHOICEBOX_CFLAGS@

# the library search path.
epuzzles_LDADD = $(all_libraries) @ECORE_LIBS@ @ECORE_FILE_LIBS@ @EVAS_LIBS@ @EDJE_LIBS@ @ECORE_EVAS_LIBS@ @EDRAWABLE_LIBS@ @XCB_AUX_LIBS@ @EFREET_LIBS@ @LIBKEYS_LIBS@ @LIBEOI_LIBS@ @CHOICEBOX_LIBS@
epuzzles_LDFLAGS = -Wl,--as-needed

