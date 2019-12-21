project('tcp-server', 'c')

libuv = dependency('libuv', version: '>=1.0.0')

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
    'tcp-server',
    sources      : [
        'main.c',
    ],
    dependencies : [
        libuv,
    ],
)
