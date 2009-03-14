AC_INIT(configure.ac)

AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(epuzzle, 0.1)

AC_LANG_C
AC_PROG_CC
AM_PROG_LIBTOOL

PKG_CHECK_MODULES(ECORE,[ecore])
PKG_CHECK_MODULES(ECORE_FILE,[ecore-file])
PKG_CHECK_MODULES(ECORE_EVAS,[ecore-evas])
PKG_CHECK_MODULES(EVAS,[evas])
PKG_CHECK_MODULES(EDJE,[edje])
PKG_CHECK_MODULES(EWL, [ewl])

if test -r "${srcdir}/configure.ac"; then
    AC_CONFIG_SUBDIRS([edrawable])
    SUBDIRS="edrawable ${SUBDIRS}"
    EDRAWABLE_CFLAGS="-I\$(top_srcdir)/edrawable/src"
    EDRAWABLE_LIBS="\$(top_builddir)/edrawable/src/libedrawable.la"
    AC_SUBST([EDRAWABLE_CFLAGS])
    AC_SUBST([EDRAWABLE_LIBS])
else
    PKG_CHECK_MODULES(EDRAWABLE, [edrawable])
fi
SUBDIRS="${SUBDIRS} src"
AC_SUBST([SUBDIRS])
AC_OUTPUT(Makefile src/Makefile src/puzzles/Makefile)
