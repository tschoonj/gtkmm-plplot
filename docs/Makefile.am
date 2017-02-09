SUBDIRS = images
NULL =

if ENABLE_DOCUMENTATION

doxyfile_deps = $(top_srcdir)/gtkmm-plplot/canvas.h \
								$(top_srcdir)/gtkmm-plplot/exception.h \
								$(top_srcdir)/gtkmm-plplot/plot2d.h \
								$(top_srcdir)/gtkmm-plplot/plot3d.h \
								$(top_srcdir)/gtkmm-plplot/plotdata2d.h \
								$(top_srcdir)/gtkmm-plplot/enums.h \
								$(top_srcdir)/gtkmm-plplot/plot.h \
								$(top_srcdir)/gtkmm-plplot/plotdata.h \
								$(top_srcdir)/gtkmm-plplot/utils.h \
								$(top_srcdir)/gtkmm-plplot/plotdatasurface.h \
								$(top_srcdir)/gtkmm-plplot/plotcontour.h \
								$(top_srcdir)/gtkmm-plplot/plotcontourshades.h \
								$(top_srcdir)/gtkmm-plplot/plotpolar.h \
								$(top_srcdir)/gtkmm-plplot/legend.h \
								$(top_srcdir)/gtkmm-plplot/regionselection.h \
								$(top_srcdir)/gtkmm-plplot/plotdata3d.h \
								$(top_srcdir)/gtkmm-plplot/plotdatapolar.h \
								$(top_srcdir)/gtkmm-plplot/plotdataline.h \
								$(top_srcdir)/gtkmm-plplot/plotdatahistogram.h \
								$(top_srcdir)/gtkmm-plplot/plotdatahistogramunbinned.h \
								$(top_srcdir)/gtkmm-plplot/plotdatahistogrambinned.h \
								$(NULL)


doxyfile.stamp: $(doxyfile_deps) Doxyfile extra_pages.dox doxygen_hack.css DoxygenLayout.xml
	$(DOXYGEN) Doxyfile
	echo Timestamp > doxyfile.stamp

gtkmm_plplot_docdir = $(datadir)/doc/gtkmm-plplot/html


install-data-local:
	$(MKDIR_P) $(DESTDIR)$(gtkmm_plplot_docdir)
	$(MKDIR_P) $(DESTDIR)$(gtkmm_plplot_docdir)/search
	$(INSTALL_DATA) $(builddir)/html/*.css $(DESTDIR)$(gtkmm_plplot_docdir)
	$(INSTALL_DATA) $(builddir)/html/*.html $(DESTDIR)$(gtkmm_plplot_docdir)
	$(INSTALL_DATA) $(builddir)/html/*.js $(DESTDIR)$(gtkmm_plplot_docdir)
	$(INSTALL_DATA) $(builddir)/html/*.map $(DESTDIR)$(gtkmm_plplot_docdir)
	$(INSTALL_DATA) $(builddir)/html/*.md5 $(DESTDIR)$(gtkmm_plplot_docdir)
	$(INSTALL_DATA) $(builddir)/html/*.png $(DESTDIR)$(gtkmm_plplot_docdir)
	$(INSTALL_DATA) $(builddir)/html/search/*.css $(DESTDIR)$(gtkmm_plplot_docdir)/search
	$(INSTALL_DATA) $(builddir)/html/search/*.html $(DESTDIR)$(gtkmm_plplot_docdir)/search
	$(INSTALL_DATA) $(builddir)/html/search/*.js $(DESTDIR)$(gtkmm_plplot_docdir)/search
	$(INSTALL_DATA) $(builddir)/html/search/*.png $(DESTDIR)$(gtkmm_plplot_docdir)/search
dist_noinst_DATA = \
	doc-install.pl \
	doc-postprocess.pl


CLEANFILES = doxyfile.stamp

all-local: doxyfile.stamp
clean-local:
	rm -rf html
uninstall-local:
	rm -rf $(DESTDIR)$(gtkmm_plplot_docdir)
endif
EXTRA_DIST = extra_pages.dox doxygen_hack.css DoxygenLayout.xml
