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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

chip8* chip8_new(const char *rom) {
    chip8 *chippy = malloc(sizeof(chippy));

    FILE *ptr = fopen(rom, "rb");

    if (ptr == NULL) {
        printf("Could not open file \"%s\".\n", rom);

        exit(EIO);
    }

    // Load the file into memory.
    fread(chippy->ram + PROGRAM_START, 1, RAM_SIZE - PROGRAM_START, ptr);

    fclose(ptr);

    // Load the fontset into memory.
    memcpy(chippy->ram, fontset, sizeof(fontset));

    // Set the program counter to the beginning of the program.
    chippy->pc = PROGRAM_START;

    return chippy;
}

void chip8_free(chip8 *chippy) {
    free(chippy);
}

int chip8_cycle(chip8 *chippy) {
    uint16_t opcode  = chippy->ram[chippy->pc] << 8 | chippy->ram[chippy->pc + 1];

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F) {
                case 0x0000: // 00E0: Clears the screen.
                    memset(chippy->gfx, 0, sizeof(chippy->gfx));
                    chippy->pc += 2;
                    break;

                case 0x000E: // 00EE: Returns from a subroutine.
                    chippy->pc = chippy->stack[--chippy->sp] + 2;
                    break;

                default:
                    printf("Invalid opcode 0x%X at address 0x%X.\n", opcode, chippy->pc);
                    return EXIT_FAILURE;
            }
            break;

        default:
            printf("Invalid opcode 0x%X at address 0x%X.\n", opcode, chippy->pc);
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
