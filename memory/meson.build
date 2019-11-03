project('memory', 'c')

debug_link_flags = []

if get_option('buildtype') == 'debug'
    debug_link_flags += [
        '-fsanitize=address',
        '-O1',
        '-fno-omit-frame-pointer',
    ]
endif

executable(
    'malloc_slist_seq',
    sources: ['malloc_slist_seq.c'],
    link_args: debug_link_flags,
)

executable(
    'malloc_slist_nseq',
    sources: ['malloc_slist_nseq.c'],
    link_args: debug_link_flags,
)
