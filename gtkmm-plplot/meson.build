install_includeconfigdir = join_paths(install_libdir, gtkmm_plplot_pcname, 'include')

gtkmm_plplotconfig_h = configure_file(
  input: 'gtkmm-plplotconfig.h.meson',
  output: 'gtkmm-plplotconfig.h',
  configuration: pkg_conf_data,
  install_dir: install_includeconfigdir,
)

headers = files(
    'canvas.h',
    'exception.h',
    'plot.h',
    'plotdata2d.h',
    'plotdata.h',
    'plot2d.h',
    'plotpolar.h',
    'utils.h',
    'plotdatasurface.h',
    'plotcontour.h',
    'plotcontourshades.h',
    'legend.h',
    'enums.h',
    'regionselection.h',
    'plotdata3d.h',
    'plot3d.h',
    'object.h',
    'plotdatapolar.h',
    'plotdata2derrorx.h',
    'plotdata2derrory.h',
    'plotdata2derrorxy.h',
    'plotdataline.h',
    'plotdatahistogram.h',
    'plotdatahistogramunbinned.h',
    'plotdatahistogrambinned.h',
    'plothistogram.h',
    'plotobject.h',
    'plotobject2d.h',
    'plotobject2dline.h',
    'plotobject2dtext.h',
)

files = files(
    'canvas.cpp',
    'plot.cpp',
    'plotdata2d.cpp',
    'plotdata.cpp',
    'plot2d.cpp',
    'plotpolar.cpp',
    'utils.cpp',
    'plotdatasurface.cpp',
    'plotcontour.cpp',
    'plotcontourshades.cpp',
    'legend.cpp',
    'regionselection.cpp',
    'plotdata3d.cpp',
    'plot3d.cpp',
    'plotdatapolar.cpp',
    'plotdata2derrorx.cpp',
    'plotdata2derrory.cpp',
    'plotdata2derrorxy.cpp',
    'plotdataline.cpp',
    'plotdatahistogram.cpp',
    'plotdatahistogramunbinned.cpp',
    'plotdatahistogrambinned.cpp',
    'plothistogram.cpp',
    'plotobject.cpp',
    'plotobject2d.cpp',
    'plotobject2dline.cpp',
    'plotobject2dtext.cpp',
)

install_headers('gtkmm-plplot.h', subdir: gtkmm_plplot_pcname)
install_headers(headers, subdir: join_paths(gtkmm_plplot_pcname, 'gtkmm-plplot'))

gtkmm_plplot_cpp_args = [ '-DG_LOG_DOMAIN="Gtkmm::PLplot"' ]

extra_include_dirs = include_directories('..', '.')

gtkmm_plplot_library = shared_library(gtkmm_plplot_pcname, headers, files, 
    darwin_versions: darwin_versions,
    cpp_args: gtkmm_plplot_cpp_args,
    version: gtkmm_plplot_libversion,
    include_directories: extra_include_dirs,
    dependencies: gtkmm_plplot_build_dep,
    install: true)

gtkmm_plplot_dep = declare_dependency(
    link_with: gtkmm_plplot_library,
    include_directories: extra_include_dirs,
    dependencies: gtkmm_plplot_build_dep
)