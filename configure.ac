AC_INIT(configure.ac)

AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(epuzzles, 0.1)

AC_LANG_C
AC_PROG_CC
AM_PROG_LIBTOOL

PKG_CHECK_MODULES(XCB_AUX, [xcb-aux])
PKG_CHECK_MODULES(ECORE,[ecore])
PKG_CHECK_MODULES(ECORE_FILE,[ecore-file])
PKG_CHECK_MODULES(ECORE_EVAS,[ecore-evas])
PKG_CHECK_MODULES(EVAS,[evas])
PKG_CHECK_MODULES(EDJE,[edje])
PKG_CHECK_MODULES(EFREET, [efreet])
PKG_CHECK_MODULES(EWL, [ewl])

if test -r "${srcdir}/edrawable/configure.ac"; then
    AC_CONFIG_SUBDIRS([edrawable])
    SUBDIRS="edrawable ${SUBDIRS}"
    EDRAWABLE_CFLAGS="-I\$(top_srcdir)/edrawable/src"
    EDRAWABLE_LIBS="\$(top_builddir)/edrawable/src/libedrawable.la"
    AC_SUBST([EDRAWABLE_CFLAGS])
    AC_SUBST([EDRAWABLE_LIBS])
else
    PKG_CHECK_MODULES(EDRAWABLE, [edrawable])
fi
SUBDIRS="${SUBDIRS} theme src"
AC_SUBST([SUBDIRS])
AC_OUTPUT(Makefile theme/Makefile src/Makefile src/puzzles/Makefile)
