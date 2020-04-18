programs = {
    'test1': {
        'src': ['test1.cpp'],
        'cppflags': [],
    },
    'test2': {
        'src': ['test2.cpp'],
        'cppflags': ['-DTEST_CSV="@0@"'.format(join_paths(meson.current_source_dir(), 'srm1155.csv')), ],
    },
    'test3': {
        'src': ['test3.cpp'],
        'cppflags': [],
    },
    'test4': {
        'src': ['test4.cpp'],
        'cppflags': [],
    },
    'test5': {
        'src': ['test5.cpp'],
        'cppflags': [],
    },
    'test6': {
        'src': ['test6.cpp'],
        'cppflags': [],
    },
    'test7': {
        'src': ['test7.cpp'],
        'cppflags': ['-DTEST_CSV="@0@"'.format(join_paths(meson.current_source_dir(), 'contour-test.csv')), ],
    },
    'test8': {
        'src': ['test8.cpp'],
        'cppflags': ['-DTEST_CSV="@0@"'.format(join_paths(meson.current_source_dir(), 'contour-test.csv')), ],
    },
    'test9': {
        'src': ['test9.cpp'],
        'cppflags': [],
    },
    'test10': {
        'src': ['test10.cpp'],
        'cppflags': [],
    },
    'test11': {
        'src': ['test11.cpp'],
        'cppflags': [],
    },
    'test12': {
        'src': ['test12.cpp'],
        'cppflags': [],
    },
    'test13': {
        'src': ['test13.cpp'],
        'cppflags': [],
    },
}

foreach program, data : programs 
    executable(program, data['src'], cpp_args: data['cppflags'], dependencies: gtkmm_plplot_dep, install: false)
endforeach