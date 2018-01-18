dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.60)
AC_INIT(mg-samples,3.0.13)
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

dnl ========================================================================
dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl ========================================================================
dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_HEADER_TIME
AC_CHECK_HEADERS(sys/time.h unistd.h)

dnl ========================================================================
dnl check for libminigui
have_libminigui="no"
AC_CHECK_HEADERS(minigui/common.h, have_libminigui=yes, foo=bar)

dnl ========================================================================
dnl check for non-UNIX system
unix="yes"
AC_CHECK_DECLS(__NOUNIX__, unix="no", foo=bar)

dnl ========================================================================
dnl check for run-time mode of MiniGUI
dnl ========================================================================
threads_version="no"
AC_CHECK_DECLS(_MGRM_THREADS, threads_version="yes", foo=bar, [#include <minigui/common.h>])

custom_version="no"
AC_CHECK_DECLS(_MGIAL_CUSTOM, custom_version="yes", foo=bar, [#include <minigui/common.h>])

procs_version="no"
AC_CHECK_DECLS(_MGRM_PROCESSES, procs_version="yes", foo=bar, [#include <minigui/common.h>])

standalone_version="no"
AC_CHECK_DECLS(_MGRM_STANDALONE, standalone_version="yes", foo=bar, [#include <minigui/common.h>])

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

dnl ========================================================================
dnl Write Output

if test "$ac_cv_prog_gcc" = "yes"; then
    CFLAGS="$CFLAGS -Wall -Wstrict-prototypes -pipe"
fi

if test "x$threads_version" = "xyes"; then
    LIBS="$LIBS -lminigui_ths -lpthread"
else
    if test "x$standalone_version" = "xyes"; then
        CFLAGS="$CFLAGS -D_REENTRANT"
        LIBS="$LIBS -lminigui_sa"
    else
        LIBS="$LIBS -lminigui_procs"
    fi
    lib_pthread="yes"
    AC_CHECK_DECLS(_MGGAL_SHADOW, LIBS="$LIBS -lpthread", lib_pthread="no", [#include <minigui/common.h>])
    if test "x$lib_pthread" = "xno"; then
        AC_CHECK_DECLS(_MGGAL_MLSHADOW, LIBS="$LIBS -lpthread", no_pthread="no", [#include <minigui/common.h>])
    fi
fi

AM_CONDITIONAL(MGRM_PROCESSES, test "x$procs_version" = "xyes")
AC_CHECK_DECLS(_HAVE_MATH_LIB, LIBS="$LIBS -lm", foo=bar, [#include <minigui/common.h>])
AC_CHECK_DECLS(_MGIAL_DLCUSTOM, LIBS="$LIBS -ldl", foo=bar, [#include <minigui/common.h>])
AC_CHECK_DECLS(_MGIMAGE_PNG, LIBS="$LIBS -lpng", foo=bar, [#include <minigui/common.h>])
AC_CHECK_DECLS(_MGIMAGE_JPG, LIBS="$LIBS -ljpeg", foo=bar, [#include <minigui/common.h>])
AC_CHECK_DECLS(_MGFONT_TTF, LIBS="$LIBS -lttf", foo=bar, [#include <minigui/common.h>])
AC_CHECK_DECLS(_MGFONT_FT2, LIBS="$LIBS -lfreetype", foo=bar, [#include <minigui/common.h>])
AC_CHECK_DECLS(_MGFONT_SEF, LIBS="$LIBS -lscripteasy", foo=bar, [#include <minigui/common.h>])

if test "x$devel_mode" = "xyes"; then
    CFLAGS="$CFLAGS -Werror"
fi

AC_OUTPUT(
Makefile
same/Makefile
same/res/Makefile
graphics/Makefile
graphics/res/Makefile
housekeeper/Makefile
housekeeper/res/Makefile
minesweeper/Makefile
minesweeper/res/Makefile
ctrl_dlg/Makefile
ctrl_dlg/res/Makefile
lf_skin/Makefile
lf_skin/res/Makefile
lf_skin/res/skin_res/Makefile
lf_skin/res/systemres/Makefile
look_feel/Makefile
look_feel/res/Makefile
font_text/Makefile
dbuff/Makefile
dbuff/res/Makefile
mginit/Makefile
mginit/res/Makefile
src/Makefile
src/res/Makefile
)

if test "x$have_libminigui" != "xyes"; then
    AC_MSG_WARN([
        MiniGUI is not properly installed on the system. You need 
        MiniGUI Ver 3.0.6 or later for building this package. 
        Please configure and install MiniGUI Ver 3.0.6 first.
    ])
fi

