lib_LTLIBRARIES = libgtkmm-plplot-0.1.la
gtkmm_plplotincludedir = ${includedir}/gtkmm-plplot-0.1/gtkmm-plplot

gtkmm_plplotinclude_HEADERS = \
                              canvas.h \
                              color.h \
                              exception.h \
                              plot2ddata.h \
                              plot2d.h \
                              utils.h \
                              linestyle.h

libgtkmm_plplot_0_1_la_SOURCES = $(gtkmm_plplotinclude_HEADERS) \
                                 plot2ddata.cpp \
                                 plot2d.cpp \
                                 canvas.cpp

libgtkmm_plplot_0_1_la_CPPFLAGS = $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS) -I$(top_srcdir)
libgtkmm_plplot_0_1_la_LIBADD = $(gtkmm_LIBS) $(plplotcxx_LIBS)
