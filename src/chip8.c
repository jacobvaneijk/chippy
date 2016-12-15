/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "chip8.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

chip8* chip8_new(const char *rom) {
    chip8 *chippy = malloc(sizeof(chippy));

    FILE *ptr = fopen(rom, "rb");

    if (ptr == NULL) {
        printf("Could not open file \"%s\".\n", rom);

        exit(EIO);
    }

    chippy->rom = ptr;

    return chippy;
}

static void chip8_free(chip8 *chippy) {
    if (chippy->rom != NULL) {
        fclose(chippy->rom);
    }

    free(chippy);
}

int chip8_run(chip8 *chippy) {
    printf("Starting chippy...\n");

    free(chippy);

    return EXIT_SUCCESS;
}
