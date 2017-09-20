chippy_files = files(
    'main.c'
)

sdl2 = dependency('sdl2')

executable(
    'chippy',
    chippy_files,
    include_directories: inc_dir,
    link_with: [libchippy],
    dependencies: [sdl2]
)
