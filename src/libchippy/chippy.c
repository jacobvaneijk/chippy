/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "chippy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NNN(opcode) (opcode & 0x0FFF)
#define KK(opcode)  (opcode & 0x00FF)
#define N(opcode)   (opcode & 0x000F)
#define X(opcode)   ((opcode >> 8) & 0x000F)
#define Y(opcode)   ((opcode >> 4) & 0x000F)
#define P(opcode)   (opcode >> 12)

void chippy_init(struct chippy *machine) {
    memcpy(machine->ram, fontset, sizeof(fontset));
    memset(machine->stack, 0, sizeof(machine->stack));
    machine->pc = PROGRAM_START;
    machine->sp = 0;
}

int chippy_load_rom(struct chippy *machine, const char *rom) {
    FILE *f = fopen(rom, "rb");

    if (f == NULL) {
        fclose(f);
        return EXIT_FAILURE;
    }

    fread(machine->ram + PROGRAM_START, 1, RAM_SIZE - PROGRAM_START, f);

    fclose(f);

    return EXIT_SUCCESS;
}

int chippy_step(struct chippy *machine) {
    uint16_t opcode = machine->ram[machine->pc] << 8
                    | machine->ram[machine->pc + 1];

    machine->pc += 2;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0: // CLS: Clears the screen.
                    memset(machine->gfx, 0, sizeof(machine->gfx));
                    machine->pc += 2;
                    break;

                case 0x00EE: // RET: Return from a subroutine.
                    machine->pc = machine->stack[--machine->sp];
                    break;
            }
            break;

        case 0x1000: // JP: Jump to location NNN.
            machine->pc = NNN(opcode);
            break;

        case 0x2000: // CALL: Call subroutine at NNN.
            machine->stack[machine->sp++] = machine->pc;
            machine->pc = NNN(opcode);
            break;

        case 0x3000: // SE: Skip next instruction if VX == KK.
            if (machine->V[X(opcode)] == KK(opcode)) {
                machine->pc += 2;
            }
            break;

        case 0x4000: // SNE: Skip next instruction if VX != KK.
            if (machine->V[X(opcode)] != KK(opcode)) {
                machine->pc += 2;
            }
            break;

        case 0x5000: // SE: Skip next instruction if VX == VY.
            if (machine->V[X(opcode)] == machine->V[Y(opcode)]) {
                machine->pc += 2;
            }
            break;

        case 0x6000: // LD: Set VX = KK.
            machine->V[X(opcode)] = KK(opcode);
            break;

        case 0x7000: // ADD: Set VX = VX + KK.
            machine->V[X(opcode)] += KK(opcode);
            break;

        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0001: // LD: Set VX = VY.
                    machine->V[X(opcode)] = machine->V[Y(opcode)];
                    break;

                case 0x0002: // OR: Set VX = VX | VY.
                    machine->V[X(opcode)] |= machine->V[Y(opcode)];
                    break;

                case 0x0003: // XOR: Set VX = VX ^ VY.
                    machine->V[X(opcode)] ^= machine->V[Y(opcode)];
                    break;

                case 0x0004: // ADD: Set VX = VX + VY, set VF = carry.
                    machine->V[0xF] = (machine->V[X(opcode)] + machine->V[Y(opcode)]) > 255;
                    machine->V[X(opcode)] += machine->V[Y(opcode)];
                    break;

                case 0x0005: // SUB: Set VX = VX - VY, set VF = NOT borrow.
                    machine->V[0xF] = machine->V[X(opcode)] > machine->V[Y(opcode)];
                    machine->V[X(opcode)] -= machine->V[Y(opcode)];
                    break;

                case 0x0006: // SHR: Set VX = VX >> 1, set VF = LSB.
                    machine->V[0xF] = machine->V[X(opcode)] & 1;
                    machine->V[X(opcode)] >>= 1;
                    break;

                case 0x0007: // SUBN: Set VX = VY - VX, set VF = NOT borrow.
                    machine->V[0xF] = machine->V[Y(opcode)] > machine->V[X(opcode)];
                    machine->V[X(opcode)] = machine->V[Y(opcode)] - machine->V[X(opcode)];
                    break;

                case 0x000E: // SHL: Set VX = VX << 1, set VF = MSB.
                    machine->V[0xF] = (machine->V[X(opcode)] & 0x80) != 0;
                    machine->V[X(opcode)] <<= 1;
                    break;
            }
            break;

        default:
            printf("Invalid opcode 0x%X at address 0x%X.\n", opcode, machine->pc - 2);
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void chippy_destroy(struct chippy *machine) {
    free(machine);
}
