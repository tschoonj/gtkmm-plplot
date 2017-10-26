SUBDIRS = images
NULL =

if ENABLE_DOCUMENTATION

include $(top_srcdir)/gtkmm-plplot/filelist.am

doxyfile_deps = $(addprefix $(top_srcdir)/gtkmm-plplot/,$(headers))

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
