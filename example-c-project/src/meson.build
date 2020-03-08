project('example-c-project', 'c')

config = configuration_data({
    'version': '0.0.1',
})

config_file = configure_file(
    input: 'config.h.in',
    output: 'config.h',
    configuration: config,
)

libjansson = dependency('jansson')

executable(
    'example-c-project',
    sources: [
        'main.c',
    ],
    dependencies: [
        libjansson,
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
