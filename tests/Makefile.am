noinst_PROGRAMS = test1 test2 test3 test4 test5 test6

test1_SOURCES = test1.cpp
test1_CPPFLAGS = -I$(top_srcdir) $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS)
test1_LDADD = ../gtkmm-plplot/libgtkmm-plplot-0.1.la $(gtkmm_LIBS) $(plplotcxx_LIBS)

test2_SOURCES = test2.cpp
test2_CPPFLAGS = -I$(top_srcdir) $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS) \
                 -DTEST_CSV=\"$(srcdir)/srm1155.csv\"
test2_LDADD = ../gtkmm-plplot/libgtkmm-plplot-0.1.la $(gtkmm_LIBS) $(plplotcxx_LIBS)

test3_SOURCES = test3.cpp
test3_CPPFLAGS = -I$(top_srcdir) $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS)
test3_LDADD = ../gtkmm-plplot/libgtkmm-plplot-0.1.la $(gtkmm_LIBS) $(plplotcxx_LIBS)

test4_SOURCES = test4.cpp
test4_CPPFLAGS = -I$(top_srcdir) $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS)
test4_LDADD = ../gtkmm-plplot/libgtkmm-plplot-0.1.la $(gtkmm_LIBS) $(plplotcxx_LIBS)

test5_SOURCES = test5.cpp
test5_CPPFLAGS = -I$(top_srcdir) $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS)
test5_LDADD = ../gtkmm-plplot/libgtkmm-plplot-0.1.la $(gtkmm_LIBS) $(plplotcxx_LIBS)

test6_SOURCES = test6.cpp
test6_CPPFLAGS = -I$(top_srcdir) $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS)
test6_LDADD = ../gtkmm-plplot/libgtkmm-plplot-0.1.la $(gtkmm_LIBS) $(plplotcxx_LIBS)

EXTRA_DIST = srm1155.csv
