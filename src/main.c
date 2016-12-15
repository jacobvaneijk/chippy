/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"

static struct option long_options[] = {
    { "help",    no_argument, 0, 'h' },
    { "version", no_argument, 0, 'v' },
    { 0, 0, 0, 0 }
};

static void display_help(const char *program) {
    printf("Usage: %s [options] file\n"
           "\n"
           "Options:\n"
           " -h, --help    Display this information.\n"
           " -v, --version Display version information.\n"
           "\n"
           "For bug reports, please see <%s>.\n", program, PACKAGE_BUGREPORT);
}

static void display_version(void) {
    printf("%s (%s) %s\nCopyright (c) 2016, Jacob van Eijk\n",
        PACKAGE,
        PACKAGE_NAME,
        PACKAGE_VERSION);
}

int main(int argc, char **argv) {
    int opt = 0;

    while ((opt = getopt_long(argc, argv, "hv", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                display_help(argv[0]);
                return EXIT_SUCCESS;

            case 'v':
                display_version();
                return EXIT_SUCCESS;

            case '?':
                break;

            default:
                break;
        }
    }

    if (optind >= argc) {
        display_help(argv[0]);

        return EXIT_FAILURE;
    }

    chip8 *chippy = chip8_new(argv[optind]);

    return chip8_run(chippy);
}
