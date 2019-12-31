dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.60)
AC_INIT(mg-samples,4.1.0)
AC_CONFIG_SRCDIR(src/helloworld.c)

dnl ========================================================================
dnl Need for cross-compiling
AC_CANONICAL_SYSTEM

dnl ========================================================================
dnl Init automake
AM_INIT_AUTOMAKE

dnl ========================================================================
dnl Checks for programs.
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LD
AC_PROG_RANLIB
AC_PROG_LIBTOOL

dnl ========================================================================
dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl ========================================================================
dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_HEADER_TIME
AC_CHECK_HEADERS(sys/time.h unistd.h)

AC_ARG_ENABLE(develmode,
[  --enable-develmode       developer mode <default=no>],
devel_mode=$enableval)

AC_ARG_WITH(lang,
        [  --with-lang=[en/zhcn/zhtw]])

case "$with_lang" in
  zhcn)
    AC_DEFINE(_LANG_ZHCN, 1,
            [Use text strings for locale zh_CN in GB2312])
    ;;
  zhtw)
    AC_DEFINE(_LANG_ZHTW, 1,
            [Use text strings for locale zh_TW in BIG5])
    ;;
  *)
    AC_DEFINE(_LANG_EN, 1,
            [Use text strings for locale en_US in ISO8859-1])
    ;;
esac

if test "x$devel_mode" = "xyes"; then
    CPPFLAGS="$CPPFLAGS -DDEBUG -D_DEBUG"
    if test "$ac_cv_prog_gcc" = "yes"; then
        CPPFLAGS="$CPPFLAGS -Wall -Werror"
    fi
fi

if test "$ac_cv_prog_gcc" = "yes"; then
    CFLAGS="$CFLAGS -Wstrict-prototypes -pipe"
fi

dnl ========================================================================
dnl check for installation of MiniGUI
dnl ========================================================================
PKG_CHECK_MODULES([MINIGUI], [minigui >= 4.0.0])
PKG_CHECK_MODULES([MGUTILS], [mgutils >= 1.2.2])
PKG_CHECK_MODULES([MGPLUS], [mgplus >= 1.4.0])
PKG_CHECK_MODULES([MGEFF], [mgeff >= 1.2.2])
PKG_CHECK_MODULES([MGNCS], [mgncs >= 1.2.4])

LIBS="$LIBS $MINIGUI_LIBS"

dnl ========================================================================
dnl Write Output

AC_CHECK_DECLS(_MGRM_PROCESSES, minigui_runmode="procs", foo=bar, [#include <minigui/common.h>])
AM_CONDITIONAL(MGRM_PROCESSES, test "x$minigui_runmode" = "xprocs")

AC_OUTPUT(
    Makefile
    m4/Makefile
    mginit/Makefile
    mginit/res/Makefile
    src/Makefile
    src/res/Makefile
    graphics/Makefile
    graphics/res/Makefile
    dbuff/Makefile
    dbuff/res/Makefile
    ctrl_dlg/Makefile
    ctrl_dlg/res/Makefile
    lf_skin/Makefile
    lf_skin/res/Makefile
    lf_skin/res/skin_res/Makefile
    lf_skin/res/systemres/Makefile
    look_feel/Makefile
    look_feel/res/Makefile
    font_text/Makefile
    same/Makefile
    same/res/Makefile
    housekeeper/Makefile
    housekeeper/res/Makefile
    minesweeper/Makefile
    minesweeper/res/Makefile
    incore-data/Makefile
    complexscripts/Makefile
    mgutils/Makefile
    mgutils/templates/Makefile
    mgutils/mywins/Makefile
    mgutils/skin/Makefile
    mgutils/notebook/Makefile
    mgutils/notebook/res/Makefile
    mgutils/painter/Makefile
    mgutils/picview/Makefile
    mgutils/picview/res/Makefile
    mgutils/vcongui/Makefile
    mgplus/Makefile
    mgplus/animate/Makefile
    mgplus/animate/common_animates/Makefile
    mgplus/src/Makefile
    mgplus/src/res/Makefile
    mgplus/pen/Makefile
    mgplus/path/Makefile
    mgplus/brush/Makefile
    mgplus/colorcomposite/Makefile
    mgplus/glyphoutline/Makefile
    mgplus/worldtransform/Makefile
    mgeff/Makefile
    mgeff/animation/Makefile
    mgeff/ball/Makefile
    mgeff/banner/Makefile
    mgeff/btnshow/Makefile
    mgeff/curve/Makefile
    mgeff/curve/res/Makefile
    mgeff/custom-effector/Makefile
    mgeff/custom-effector/res/Makefile
    mgeff/effector/Makefile
    mgeff/effector/res/Makefile
    mgeff/image-box/Makefile
    mgeff/image-box/res/Makefile
    mgeff/particle-sys/Makefile
    mgncs/Makefile
    mgncs/res/Makefile
)
