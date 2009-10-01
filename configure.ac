AC_INIT(configure.ac)

AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(epuzzles, 0.1)

AC_LANG_C
AC_PROG_CC
AM_PROG_LIBTOOL

PKG_CHECK_MODULES(XCB_AUX, [xcb-aux])
PKG_CHECK_MODULES(EVAS,[evas])
PKG_CHECK_MODULES(EDJE,[edje])
PKG_CHECK_MODULES(ECORE,[ecore])
PKG_CHECK_MODULES(ECORE_FILE,[ecore-file])
PKG_CHECK_MODULES(ECORE_EVAS,[ecore-evas])
PKG_CHECK_MODULES(EFREET, [efreet])
PKG_CHECK_MODULES(EDRAWABLE, [libedrawable])
PKG_CHECK_MODULES(LIBKEYS, [libkeys])
PKG_CHECK_MODULES(CHOICEBOX, [libchoicebox])
PKG_CHECK_MODULES(LIBEOI, [libeoi])

AC_OUTPUT([
    Makefile theme/Makefile src/Makefile src/puzzles/Makefile 
    desktops/Makefile data/Makefile
    theme/sprites/fifteen/Makefile
])

