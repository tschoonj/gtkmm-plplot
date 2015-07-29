lib_LTLIBRARIES = libgtkmm-plplot-0.1.la
gtkmm_plplotincludedir = ${includedir}/gtkmm-plplot-0.1/gtkmm-plplot
gtkmm_plplotbaseincludedir = ${includedir}/gtkmm-plplot-0.1/gtkmm-plplot

gtkmm_plplotinclude_HEADERS = \
                              canvas.h \
                              exception.h \
                              plotabstract.h \
                              plotdata2d.h \
                              plotdataabstract.h \
                              plot2d.h \
                              plotpolar.h \
                              utils.h \
                              enums.h

gtkmm_plplotbaseinclude_HEADERS = gtkmm-plplot.h

libgtkmm_plplot_0_1_la_SOURCES = $(gtkmm_plplotinclude_HEADERS) \
                                 $(gtkmm_plplotbaseinclude_HEADERS) \
                                 plotdataabstract.cpp \
                                 plotdata2d.cpp \
                                 plotabstract.cpp \
                                 plot2d.cpp \
                                 utils.cpp \
                                 canvas.cpp \
                                 plotpolar.cpp

libgtkmm_plplot_0_1_la_CPPFLAGS = $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS) -I$(top_srcdir)
libgtkmm_plplot_0_1_la_LIBADD = $(gtkmm_LIBS) $(plplotcxx_LIBS)
