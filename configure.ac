AC_INIT([gtkmm-plplot],[0.1],[tom.schoonjans@me.com],,[https://github.com/tschoonj/gtkmm-plplot])
AC_PREREQ([2.60])
AC_CONFIG_SRCDIR([gtkmm-plplot/plot2ddata.cpp])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CANONICAL_HOST
AC_CONFIG_MACRO_DIRS([m4])


#m4_pattern_allow([AS_TR_SH])

AC_USE_SYSTEM_EXTENSIONS
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

LT_INIT

AC_PROG_CC
if test `AS_BASENAME([$CC])` = $CC ; then
  AC_CHECK_PROG(CC_FULL, [$CC], $CC, [none])
  #this next line may never be reached...
  if test x$CC_FULL = "xnone" ; then
          AC_MSG_ERROR([no C compiler was found on the system.])
  fi
fi
AM_PROG_CC_C_O


AC_PROG_CXX
if test `AS_BASENAME([$CXX])` = $CXX ; then
  AC_CHECK_PROG(CXX_FULL, [$CXX], $CXX, [none])
  #this next line may never be reached...
  if test x$CXX_FULL = "xnone" ; then
          AC_MSG_ERROR([no C++ compiler was found on the system.])
  fi
fi

AX_CXX_COMPILE_STDCXX_11(ext, mandatory)

LDFLAGS_EXTRA=""
OS_WINDOWS=0
OS_WINDOWS_32=0
OS_WINDOWS_64=0

case "$host" in
	i686-*mingw*)
		OS_WINDOWS_32=1
		OS_WINDOWS=1
		;;
	x86_64-*mingw*)
		OS_WINDOWS_64=1
		OS_WINDOWS=1
		;;
esac

AC_SUBST(WINDRES_ARCH)

AC_SUBST(OS_WINDOWS)
AM_CONDITIONAL([OS_WINDOWS],[test x$OS_WINDOWS = x1])
AC_SUBST(OS_WINDOWS_32)
AM_CONDITIONAL([OS_WINDOWS_32],[test x$OS_WINDOWS_32 = x1])
AC_SUBST(OS_WINDOWS_64)
AM_CONDITIONAL([OS_WINDOWS_64],[test x$OS_WINDOWS_64 = x1])

#initialize pkg-config
PKG_PROG_PKG_CONFIG([0.27])


#search for xraylib and other modules
PKG_CHECK_MODULES([gtkmm],gtkmm-3.0 >= 3.12.0)
PKG_CHECK_MODULES([plplotcxx], [plplot-c++], AC_SUBST([PLPLOT_CXX], [plplot-c++]),
 [PKG_CHECK_MODULES([plplotcxx], [plplotd-c++], AC_SUBST([PLPLOT_CXX], [plplotd-c++]))])

#check for the extcairo device
result=
AC_MSG_CHECKING([for plplot extcairo device])
ac_save_CFLAGS="$CFLAGS"
CFLAGS=$plplotcxx_CFLAGS
AC_LANG_PUSH([C])
AC_TRY_COMPILE([
        #include <plDevs.h>
],[
#ifndef PLD_extcairo
  #error
#endif
],[
  result=yes
  AC_DEFINE([HAVE_EXTCAIRO], [], [extcairo found])
],[result=no])
AC_MSG_RESULT([$result])
if test x$result = xno ; then
  AC_MSG_ERROR([plplot must be built with the extcairo device!])
fi
AC_LANG_POP
CFLAGS="$ac_save_CFLAGS"


PKG_INSTALLDIR


AC_CONFIG_FILES([
Makefile
gtkmm-plplot/Makefile
tests/Makefile
gtkmm-plplot-0.1.pc
])
AC_CONFIG_HEADERS([config.h])



AC_OUTPUT
