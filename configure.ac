AC_INIT([gtkmm-plplot],[2.2],[tom.schoonjans@me.com],,[https://github.com/tschoonj/gtkmm-plplot])
AC_PREREQ([2.60])
AC_CONFIG_SRCDIR([gtkmm-plplot/canvas.cpp])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CANONICAL_HOST
AC_CONFIG_MACRO_DIR([m4])

LIB_CURRENT=1
LIB_REVISION=0
LIB_AGE=0
AC_SUBST(LIB_CURRENT)
AC_SUBST(LIB_REVISION)
AC_SUBST(LIB_AGE)
LIB_CURRENT_MINUS_AGE=`expr $LIB_CURRENT - $LIB_AGE`
AC_SUBST(LIB_CURRENT_MINUS_AGE)

AC_USE_SYSTEM_EXTENSIONS
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

LT_INIT

MM_PREREQ([0.9.0])
MM_INIT_MODULE([gtkmm-plplot-2.0])
MM_CONFIG_DOCTOOL_DIR([docs])

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

AX_CXX_COMPILE_STDCXX_11(noext, mandatory)

boost_found=yes
BOOST_REQUIRE(, [
  boost_found=no
])

if test x$boost_found = xyes ; then
  AC_DEFINE([GTKMM_PLPLOT_BOOST_ENABLED],, [Boost was detected])
  BOOST_MULTIARRAY
fi



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
PKG_PROG_PKG_CONFIG([0.26])


#search for our dependencies
PKG_CHECK_MODULES([gtkmm],gtkmm-3.0 >= 3.0.0)
PKG_CHECK_MODULES([plplotcxx], [plplot-c++ >= 5.11.0],
 AC_SUBST([PLPLOT_CXX], ["plplot-c++ >= 5.11.0"])
 AC_DEFINE([GTKMM_PLPLOT_PLPLOT_5_11_0], [], [PLplot is at least 5.11.0]),
 [PKG_CHECK_MODULES([plplotcxx], [plplotd-c++ >= 5.10.0], AC_SUBST([PLPLOT_CXX], ["plplotd-c++ >= 5.10.0"]))])

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
],[result=no])
AC_MSG_RESULT([$result])
if test x$result = xno ; then
  AC_MSG_ERROR([plplot must be built with the extcairo device!])
fi
AC_LANG_POP
CFLAGS="$ac_save_CFLAGS"

#AC_CHECK_PROGS([DOXYGEN], [doxygen])
#if test -z "$DOXYGEN";
#   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
#fi
#AM_CONDITIONAL([HAVE_DOXYGEN],
#[test -n "$DOXYGEN"])AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([docs/Doxyfile])])

MM_ARG_ENABLE_DOCUMENTATION
MM_ARG_WITH_TAGFILE_DOC([libstdc++.tag], [mm-common-libstdc++])
MM_ARG_WITH_TAGFILE_DOC([libsigc++-2.0.tag], [sigc++-2.0])
MM_ARG_WITH_TAGFILE_DOC([glibmm-2.4.tag], [glibmm-2.4])
MM_ARG_WITH_TAGFILE_DOC([cairomm-1.0.tag], [cairomm-1.0])
MM_ARG_WITH_TAGFILE_DOC([pangomm-1.4.tag], [pangomm-1.4])
MM_ARG_WITH_TAGFILE_DOC([atkmm-1.6.tag], [atkmm-1.6])
MM_ARG_WITH_TAGFILE_DOC([gtkmm-3.0.tag], [gtkmm-3.0])


AC_CONFIG_FILES([
Makefile
gtkmm-plplot/Makefile
tests/Makefile
docs/Makefile
docs/images/Makefile
docs/Doxyfile
${GTKMM_PLPLOT_MODULE_NAME}.pc:gtkmm-plplot.pc.in
gtkmm-plplot.spec
])
AC_CONFIG_HEADERS([config.h gtkmm-plplot/gtkmm-plplotconfig.h])



AC_OUTPUT
