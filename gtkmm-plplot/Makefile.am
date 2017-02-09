NULL =

lib_LTLIBRARIES = libgtkmm-plplot-@GTKMM_PLPLOT_API_VERSION@.la
gtkmm_plplotincludedir = ${includedir}/gtkmm-plplot-@GTKMM_PLPLOT_API_VERSION@/gtkmm-plplot
gtkmm_plplotbaseincludedir = ${includedir}/gtkmm-plplot-@GTKMM_PLPLOT_API_VERSION@

gtkmm_plplotinclude_HEADERS = \
                              canvas.h \
                              exception.h \
                              plot.h \
                              plotdata2d.h \
                              plotdata.h \
                              plot2d.h \
                              plotpolar.h \
                              utils.h \
                              plotdatasurface.h \
                              plotcontour.h \
                              plotcontourshades.h \
                              legend.h \
                              enums.h \
                              regionselection.h \
                              plotdata3d.h \
                              plot3d.h \
                              object.h \
                              plotdatapolar.h \
                              plotdata2derrorx.h \
                              plotdata2derrory.h \
                              plotdata2derrorxy.h \
                              plotdataline.h \
                              plotdatahistogram.h \
                              plotdatahistogramunbinned.h \
                              plotdatahistogrambinned.h \
                              plothistogram.h \
                              $(NULL)

gtkmm_plplotbaseinclude_HEADERS = gtkmm-plplot.h

libgtkmm_plplot_@GTKMM_PLPLOT_API_VERSION@_la_SOURCES = $(gtkmm_plplotinclude_HEADERS) \
                                 $(gtkmm_plplotbaseinclude_HEADERS) \
                                 canvas.cpp \
                                 plot.cpp \
                                 plotdata.cpp \
                                 plotdata2d.cpp \
                                 plot2d.cpp \
                                 utils.cpp \
                                 plotpolar.cpp \
                                 plotdatasurface.cpp \
                                 plotcontour.cpp \
                                 plotcontourshades.cpp \
                                 legend.cpp \
                                 regionselection.cpp \
                                 plotdata3d.cpp \
                                 plot3d.cpp \
                                 plotdatapolar.cpp \
                                 plotdata2derrorx.cpp \
                                 plotdata2derrory.cpp \
                                 plotdata2derrorxy.cpp \
                                 plotdataline.cpp \
                                 plotdatahistogram.cpp \
                                 plothistogram.cpp \
                                 plotdatahistogramunbinned.cpp \
                                 plotdatahistogrambinned.cpp \
                                 $(NULL)

libgtkmm_plplot_@GTKMM_PLPLOT_API_VERSION@_la_CPPFLAGS = $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS) -I$(top_srcdir) $(BOOST_CPPFLAGS)
libgtkmm_plplot_@GTKMM_PLPLOT_API_VERSION@_la_LIBADD = $(gtkmm_LIBS) $(plplotcxx_LIBS) $(BOOST_MULTIARRAY_LIBS)
libgtkmm_plplot_@GTKMM_PLPLOT_API_VERSION@_la_LDFLAGS = $(BOOST_MULTIARRAY_LDFLAGS) -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@

gtkmm_plplotlibincludedir = $(libdir)/gtkmm-plplot-@GTKMM_PLPLOT_API_VERSION@/include
nodist_gtkmm_plplotlibinclude_HEADERS = gtkmm-plplotconfig.h

if OS_WINDOWS
libgtkmm_plplot_@GTKMM_PLPLOT_API_VERSION@_la_LDFLAGS += -no-undefined
endif
