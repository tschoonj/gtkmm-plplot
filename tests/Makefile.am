NULL =

noinst_PROGRAMS = \
  test1 \
  test2 \
  test3 \
  test4 \
  test5 \
  test6 \
  test7 \
  test8 \
  test9 \
  test10 \
  test11 \
  test12 \
  $(NULL)

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/gtkmm-plplot -I$(top_builddir) $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS)
AM_LDADD = ../gtkmm-plplot/libgtkmm-plplot-@GTKMM_PLPLOT_API_VERSION@.la $(gtkmm_LIBS) $(plplotcxx_LIBS)

test1_SOURCES = test1.cpp
test1_CPPFLAGS = $(AM_CPPFLAGS)
test1_LDADD = $(AM_LDADD)

test2_SOURCES = test2.cpp
test2_CPPFLAGS = $(AM_CPPFLAGS) \
                 -DTEST_CSV=\"$(abs_srcdir)/srm1155.csv\"
test2_LDADD = $(AM_LDADD)

test3_SOURCES = test3.cpp
test3_CPPFLAGS = $(AM_CPPFLAGS)
test3_LDADD = $(AM_LDADD)

test4_SOURCES = test4.cpp
test4_CPPFLAGS = $(AM_CPPFLAGS)
test4_LDADD = $(AM_LDADD)

test5_SOURCES = test5.cpp
test5_CPPFLAGS = $(AM_CPPFLAGS)
test5_LDADD = $(AM_LDADD)

test6_SOURCES = test6.cpp
test6_CPPFLAGS = $(AM_CPPFLAGS)
test6_LDADD = $(AM_LDADD)

test7_SOURCES = test7.cpp
test7_CPPFLAGS = $(AM_CPPFLAGS) \
                 -DTEST_CSV=\"$(abs_srcdir)/contour-test.csv\"
test7_LDADD = $(AM_LDADD)

test8_SOURCES = test8.cpp
test8_CPPFLAGS = $(AM_CPPFLAGS) \
                 -DTEST_CSV=\"$(abs_srcdir)/contour-test.csv\"
test8_LDADD = $(AM_LDADD)

test9_SOURCES = test9.cpp
test9_CPPFLAGS = $(AM_CPPFLAGS)
test9_LDADD = $(AM_LDADD)

test10_SOURCES = test10.cpp
test10_CPPFLAGS = $(AM_CPPFLAGS)
test10_LDADD = $(AM_LDADD)

test11_SOURCES = test11.cpp
test11_CPPFLAGS = $(AM_CPPFLAGS)
test11_LDADD = $(AM_LDADD)

test12_SOURCES = test12.cpp
test12_CPPFLAGS = $(AM_CPPFLAGS)
test12_LDADD = $(AM_LDADD)

EXTRA_DIST = srm1155.csv contour-test.csv
