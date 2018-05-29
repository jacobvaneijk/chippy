/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libchippy/chippy.h"
#include "gfx.h"

static int hidpi = 0;

static struct option long_options[] = {
    { "help",    no_argument, 0,      'h' },
    { "version", no_argument, 0,      'v' },
    { "hidpi",   no_argument, &hidpi,  1  },
    { 0, 0, 0, 0 }
};

static void display_help(const char *program) {
    printf("Usage: %s [options] file\n"
           "\n"
           "Options:\n"
           " -h, --help    Display this information.\n"
           " -v, --version Display version information.\n"
           "     --hidpi   Scale for HiDPI screens.\n"
           "\n"
           "For bug reports, please see <%s>.\n", program, PACKAGE_BUGREPORT);
}

static void display_version(void) {
    printf("%s %s\nCopyright (c) 2017, Jacob van Eijk\n",
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

    if (gfx_init(hidpi ? 2 : 1)) {
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    struct chippy *machine = malloc(sizeof(struct chippy));

    chippy_init(machine);

    if (chippy_load_rom(machine, argv[optind]) != 0) {
        return EXIT_FAILURE;
    }

    while (chippy_step(machine) != EXIT_FAILURE) {
        if (gfx_close_requested() != 0) {
            break;
        }

        gfx_render(machine, hidpi ? 2 : 1);
    }

    gfx_destroy();

    return EXIT_FAILURE;
}
