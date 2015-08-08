lib_LTLIBRARIES = libgtkmm-plplot-@GTKMM_PLPLOT_API_VERSION@.la
gtkmm_plplotincludedir = ${includedir}/gtkmm-plplot-@GTKMM_PLPLOT_API_VERSION@/gtkmm-plplot
gtkmm_plplotbaseincludedir = ${includedir}/gtkmm-plplot-@GTKMM_PLPLOT_API_VERSION@/gtkmm-plplot

gtkmm_plplotinclude_HEADERS = \
                              canvas.h \
                              exception.h \
                              plot.h \
                              plotdata2d.h \
                              plotdata.h \
                              plot2d.h \
                              plotpolar.h \
                              utils.h \
                              plotdatacontour.h \
                              plotdatacontourshades.h \
                              plotcontour.h \
                              enums.h

gtkmm_plplotbaseinclude_HEADERS = gtkmm-plplot.h

libgtkmm_plplot_@GTKMM_PLPLOT_API_VERSION@_la_SOURCES = $(gtkmm_plplotinclude_HEADERS) \
                                 $(gtkmm_plplotbaseinclude_HEADERS) \
                                 plotdata.cpp \
                                 plotdata2d.cpp \
                                 plot.cpp \
                                 plot2d.cpp \
                                 utils.cpp \
                                 canvas.cpp \
                                 plotpolar.cpp \
                                 plotdatacontour.cpp \
                                 plotcontour.cpp

libgtkmm_plplot_@GTKMM_PLPLOT_API_VERSION@_la_CPPFLAGS = $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS) -I$(top_srcdir) $(BOOST_CPPFLAGS)
libgtkmm_plplot_@GTKMM_PLPLOT_API_VERSION@_la_LIBADD = $(gtkmm_LIBS) $(plplotcxx_LIBS) $(BOOST_MULTIARRAY_LIBS)
libgtkmm_plplot_@GTKMM_PLPLOT_API_VERSION@_la_LDFLAGS = $(BOOST_MULTIARRAY_LDFLAGS)

gtkmm_plplotlibincludedir = $(libdir)/gtkmm-plplot-@GTKMM_PLPLOT_API_VERSION@/include
nodist_gtkmm_plplotlibinclude_HEADERS = gtkmm-plplotconfig.h
