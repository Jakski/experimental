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
        'json.c',
    ],
    dependencies: [
        libjansson,
    ],
)

test(
    'JSON parser',
    executable(
        'test_json_parser',
        sources      : [
            'json.c',
            'tests/json.c',
        ],
        dependencies: [
            libjansson,
        ],
    ),
)
