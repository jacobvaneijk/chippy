AC_INIT(
    [Chippy],
    [0.0.1],
    [https://github.com/jacobvaneijk/chippy/issues],
    [chippy],
    [https://github.com/jacobvaneijk/chippy])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_PREREQ([2.68])

AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CC

PKG_CHECK_MODULES([SDL], [sdl2 >= 2])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
