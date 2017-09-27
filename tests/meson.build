chippy_test_files = files(
    'opcodes.c',
    'test.c'
)

check = dependency('check')

executable(
    'chippy_test',
    chippy_test_files,
    include_directories: inc_dir,
    link_with: [libchippy],
    dependencies: [check]
)
