project('memory', 'c')

malloc_slist_seq = executable(
    'malloc_slist_seq',
    sources: ['malloc_slist_seq.c'],
)

malloc_slist_nseq = executable(
    'malloc_slist_nseq',
    sources: ['malloc_slist_nseq.c'],
)

pool_slist = executable(
    'pool_slist',
    sources: ['pool_slist.c'],
)

chunked_pool_slist_step_5 = executable(
    'chunked_pool_slist_step_5',
    sources: ['chunked_pool_slist_step_5.c'],
)

chunked_pool_slist_step_10 = executable(
    'chunked_pool_slist_step_10',
    sources: ['chunked_pool_slist_step_10.c'],
)

chunked_pool_slist_step_100 = executable(
    'chunked_pool_slist_step_100',
    sources: ['chunked_pool_slist_step_100.c'],
)

chunked_pool_slist_step_1000 = executable(
    'chunked_pool_slist_step_1000',
    sources: ['chunked_pool_slist_step_1000.c'],
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
        malloc_slist_seq,
        malloc_slist_nseq,
        pool_slist,
        chunked_pool_slist_step_5,
        chunked_pool_slist_step_10,
        chunked_pool_slist_step_100,
        chunked_pool_slist_step_1000,
    ],
)
