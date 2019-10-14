project('linker-test', 'c')
a = shared_library(
    'a',
    [
        'a.c',
    ],
    soversion: '0',
    version: '0.1.0',
    link_args: [
        '-Wl,--version-script=' + meson.current_source_dir() + '/a.sym',
    ],
)
b = shared_library(
    'b',
    [
        'b.c',
    ],
    soversion: '1',
    version: '1.2.0',
    link_args: [
        '-Wl,--version-script=' + meson.current_source_dir() + '/b.sym',
    ],
)
a_wrapper = shared_library(
    'a_wrapper',
    [
        'a_wrapper.c',
    ],
    soversion: '0',
    version: '0.1.0',
    link_with: [
        a,
    ]
)
b_wrapper = shared_library(
    'b_wrapper',
    [
        'b_wrapper.c',
    ],
    soversion: '1',
    version: '1.2.0',
    link_with: [
        b,
    ]
)
executable(
    'linker-test',
    [
        'main.c',
    ],
    link_with: [
        a_wrapper,
        b_wrapper,
    ],
)
