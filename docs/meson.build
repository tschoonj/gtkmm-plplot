tag_file_modules = [
  'mm-common-libstdc++', # provided by mm-common
  'sigc++-3.0',
  'glibmm-2.68',
  'cairomm-1.16',
  'pangomm-2.48',
  'gtkmm-4.0',
]

doxygen_tagfiles = ''
docinstall_flags = []
foreach module : tag_file_modules
  depmod = dependency(module, required: false)
  if depmod.found()
    doxytagfile = depmod.get_variable(pkgconfig : 'doxytagfile')
    htmlrefpub = depmod.get_variable(pkgconfig: 'htmlrefpub', default_value: '')
    htmlrefdir = depmod.get_variable(pkgconfig: 'htmlrefdir', default_value: '')
    if htmlrefpub == ''
      htmlrefpub = htmlrefdir
    elif htmlrefdir == ''
      htmlrefdir = htmlrefpub
    endif
    doxygen_tagfiles += ' "' + doxytagfile + '=' + htmlrefpub + '"'
    if not htmlrefdir.endswith('/')
      htmlrefdir += '/'
    endif
    docinstall_flags += ['-l', doxytagfile.split('/')[-1] + '@' + htmlrefdir]
  endif
endforeach


doxygen_data = configuration_data()
doxygen_data.set('DOXYGEN_TAGFILES', doxygen_tagfiles)
doxygen_data.set('PACKAGE_VERSION', meson.project_version())
doxygen_data.set('PERL', perl.full_path())
doxygen_data.set('abs_top_builddir', project_build_root)
doxygen_data.set('abs_top_srcdir', project_source_root)
doxygen_data.set('srcdir', meson.current_source_dir())
doxygen_data.set('builddir', meson.current_build_dir())

doxyfile = configure_file(
    input: 'Doxyfile.in',
    output: 'Doxyfile',
    configuration: doxygen_data)

datadir = join_paths(get_option('datadir'), 'doc', 'gtkmm-plplot')

html_target = custom_target('docs',
    input: [doxyfile],
    depend_files: ['DoxygenLayout.xml', 'doxygen_hack.css', 'extra_pages.dox'],
    depends: gtkmm_plplot_library,
    output: 'html',
    command: [doxygen, '@INPUT@'],
    install_dir: datadir,
    install: true)
