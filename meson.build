project(
    'chippy',
    'c',
    license: 'MIT',
    version: '0.0.1'
)

add_project_arguments(
    '-Wall',
    '-Wextra',
    '-Werror',
    '-std=c99',
    '-Wno-unused-variable',
    language: 'c'
)

add_project_arguments(
    '-DPACKAGE_NAME="@0@"'.format(meson.project_name()),
    language: 'c'
)

add_project_arguments(
    '-DPACKAGE_VERSION="@0@"'.format(meson.project_version()),
    language: 'c'
)

add_project_arguments(
    '-DPACKAGE_BUGREPORT="@0@"'.format('https://github.com/jacobvaneijk/chippy/issues'),
    language: 'c'
)

inc_dir = include_directories('src')

subdir('src/libchippy')
subdir('src/chippy')
subdir('tests')
