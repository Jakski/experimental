project('chat-server', 'c')

libuv = dependency('libuv', version: '>=1.0.0')
debug_link_flags = []

if get_option('buildtype') == 'debug'
    debug_link_flags += [
        '-fsanitize=address',
        '-O1',
        '-fno-omit-frame-pointer',
    ]
endif

config = configuration_data({
    'version'            : '0.0.1',
    'connection_backlog' : 128,
    'port'               : 8081,
    'address'            : '127.0.0.1',
})

config_file = configure_file(
    input         : 'config.h.in',
    output        : 'config.h',
    configuration : config,
)

executable(
    'chat-server',
    sources      : [
        'main.c',
    ],
    link_args    : debug_link_flags,
    dependencies : [
        libuv,
    ],
)

#test(
    #'connection buffer',
    #executable(
        #'test_connection_buffer',
        #sources      : [
            #'connection.c',
            #'tests/test_connection_buffer.c',
        #],
        #link_args    : debug_link_flags,
    #),
#)
