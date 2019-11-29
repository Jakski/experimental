project('memory', 'c')

main = executable(
    'main',
    sources: ['main.c'],
)

run_target(
    'metrics',
    command: [
        'awk',
        '-f',
        meson.current_source_dir() + '/metrics.awk',
        meson.current_build_dir() + '/metrics.txt',
    ],
    depends: [
        main,
    ],
)
