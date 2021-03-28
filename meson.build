project('gtkmm-plplot', 'cpp',
  version: '2.5',
  license: 'LGPLv2.1+',
  default_options: [
    'cpp_std=c++11'
  ],
  meson_version: '>= 0.48.0',
)

gtkmm_plplot_api_version = '2.0'
gtkmm_plplot_pcname = meson.project_name() + '-' + gtkmm_plplot_api_version

gtkmm_plplot_version_array = meson.project_version().split('.')
gtkmm_plplot_major_version = gtkmm_plplot_version_array[0].to_int()
gtkmm_plplot_minor_version = gtkmm_plplot_version_array[1].to_int()

# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
# The relation between libtool's current:revison:age interface versioning
# and the .so filename, .so.x.y.z, is
# x = current - age
# y = age
# z = revision
# If libtool_soversion is updated as described in libtool's documentation,
# x.y.z will usually *not* be equal to meson.project_version().
libtool_soversion = [3, 1, 0]
gtkmm_plplot_libversion = '@0@.@1@.@2@'.format(
  libtool_soversion[0] - libtool_soversion[2],
  libtool_soversion[2],
  libtool_soversion[1])

lib_current = libtool_soversion[0] + 1
lib_revision =  libtool_soversion[1]
darwin_versions = [lib_current, '@0@.@1@'.format(lib_current , lib_revision)]

# Use these instead of meson.source_root() and meson.build_root() in subdirectories.
# source_root() and build_root() are not useful, if this is a subproject.
project_source_root = meson.current_source_dir()
project_build_root = meson.current_build_dir()

cpp_compiler = meson.get_compiler('cpp')

# Installation directories are relative to {prefix}.
install_prefix = get_option('prefix')
install_includedir = get_option('includedir')
install_libdir = get_option('libdir')
install_datadir = get_option('datadir')
install_pkgconfigdir = join_paths(install_libdir, 'pkgconfig')

# Configuration data
pkg_conf_data = configuration_data()
pkg_conf_data.set('prefix', install_prefix)
pkg_conf_data.set('exec_prefix', '${prefix}')
pkg_conf_data.set('libdir', join_paths('${prefix}', install_libdir))
pkg_conf_data.set('datarootdir', join_paths('${prefix}', install_datadir))
pkg_conf_data.set('datadir', '${datarootdir}')
pkg_conf_data.set('includedir', join_paths('${prefix}', install_includedir))
pkg_conf_data.set('PACKAGE_TARNAME', meson.project_name())
pkg_conf_data.set('PACKAGE_VERSION', meson.project_version())
pkg_conf_data.set('VERSION', meson.project_version())
pkg_conf_data.set('GTKMM_PLPLOT_MODULE_NAME', gtkmm_plplot_pcname)
pkg_conf_data.set('GTKMM_PLPLOT_API_VERSION', gtkmm_plplot_api_version)

pkg_conf_data.set('GTKMM_PLPLOT_MAJOR_VERSION', gtkmm_plplot_major_version)
pkg_conf_data.set('GTKMM_PLPLOT_MINOR_VERSION', gtkmm_plplot_minor_version)

# Dependencies
gtkmm_dep = dependency('gtkmm-3.0', version: '>=3.0.0')
plplotcxx_dep = dependency('plplot-c++', version: '>= 5.11.0')
plplot_dep = dependency('plplot', version: '>= 5.11.0')
pkg_conf_data.set('PLPLOT_CXX', 'plplot-c++ >= 5.11.0')
pkg_conf_data.set('GTKMM_PLPLOT_PLPLOT_5_11_0', 1)
gtkmm_plplot_build_dep = [gtkmm_dep, plplot_dep, plplotcxx_dep]
gtkmm_plplot_pkg_config_requires = [gtkmm_dep, ]
gtkmm_plplot_pkg_config_requires_private = [plplotcxx_dep, ]
boost_dep = dependency('boost', required : false)
if boost_dep.found()
    pkg_conf_data.set('GTKMM_PLPLOT_BOOST_ENABLED', 1)
    gtkmm_plplot_build_dep += boost_dep
endif

# Ensure that the extcairo device is supported by this PLplot build
code = '''
#include <plDevs.h>
#ifndef PLD_extcairo
  #error
#endif
'''

if not cpp_compiler.compiles(code, name: 'test for extcairo device', dependencies: plplot_dep)
    error('PLplot was compiled without extcairo device, which is required for gtkmm-plplot')
endif

configure_file(
    input: 'gtkmm-plplot.spec.in',
    output: 'gtkmm-plplot.spec',
    configuration: pkg_conf_data,
)


subdir('gtkmm-plplot')
subdir('tests')

if not get_option('build-documentation').disabled()
  doxygen = find_program('doxygen', required : get_option('build-documentation'))
  dot = find_program('dot', required : get_option('build-documentation'))
  perl = find_program('perl', required : get_option('build-documentation'))
  if doxygen.found() and dot.found() and perl.found()
    subdir('docs')
  endif
endif

pkgconfig = import('pkgconfig')

pkgconfig.generate(
  gtkmm_plplot_library,
  filebase: gtkmm_plplot_pcname,
  name: 'Gtkmm-PLplot',
  description: 'a scientific plotting widget collection for Gtkmm',
  version: meson.project_version(),
  libraries: boost_dep,
  subdirs: gtkmm_plplot_pcname,
  requires: gtkmm_plplot_pkg_config_requires,
  requires_private: gtkmm_plplot_pkg_config_requires_private,
  extra_cflags: '-I${libdir}/@0@/include'.format(gtkmm_plplot_pcname),
)