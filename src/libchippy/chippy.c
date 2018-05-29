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
    machine->wait_key = -1;
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

    if (machine->wait_key != -1) {
        machine->wait_key = -1;
    }

    machine->pc += 2;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0: // CLS: Clears the screen.
                    memset(machine->gfx, 0, sizeof(machine->gfx));
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

        case 0x9000: // SNE: Skip next instruction if VX != VY.
            if (machine->V[X(opcode)] != machine->V[Y(opcode)]) {
                machine->pc += 2;
            }
            break;

        case 0xA000: // LD: Set I = NNN.
            machine->I = NNN(opcode);
            break;

        case 0xB000: // JP: Jump to location NNN + V0.
            machine->pc = NNN(opcode) + machine->V[0];
            break;

        case 0xC000: // RND: Set VX = random byte & KK.
            machine->V[X(opcode)] = rand() & KK(opcode);
            break;

        case 0xD000: // DRW: Display N-byte sprite starting at address I at (VX, VY), set VF = collision.
            for (int y = 0; y < N(opcode); y++) {
                uint8_t sprite = machine->ram[machine->I + y];

                for (int x = 0; x < 8; x++) {
                    int pixel = (sprite & (1 << (7 - x))) != 0;
                    int xpos = machine->V[X(opcode)] + x;
                    int ypos = machine->V[Y(opcode)] + y;
                    int pos = SCREEN_W * ypos + xpos;

                    machine->V[0xF] |= machine->gfx[pos] & pixel;
                    machine->gfx[pos] ^= pixel;
                }
            }
            break;

        case 0xE000:
            switch (opcode & 0x000F) {
                case 0x009E: // SKP: Skip next instruction if key with the value of VX is pressed.
                    break;

                case 0x00A1: // SKNP: Skip next instruction if key with the value of VX is not pressed.
                    break;
            }
            break;

        case 0xF000:
            switch (opcode & 0x000F) {
                case 0x0007: // LD: Set VX = delay timer value.
                    machine->V[X(opcode)] = machine->dt;
                    break;

                case 0x000A: // LD: Wait for a key press, store the value of the key in VX.
                    machine->wait_key = X(opcode);
                    break;

                case 0x0015: // LD: Set delay timer = VX.
                    machine->dt = machine->V[X(opcode)];
                    break;

                case 0x0018: // LD: Set sound timer = VX.
                    machine->st = machine->V[X(opcode)];
                    break;

                case 0x001E: // ADD: Set I = I + VX.
                    machine->I += machine->V[X(opcode)];
                    break;

                case 0x0029: // LD: Set I = location of sprite for digit VX.
                    machine->I = machine->V[X(opcode)] * 5;
                    break;

                case 0x0033: // LD: Store BCD representation of VX in memory locations I, I+1 and I+2.
                    machine->ram[machine->I] = X(opcode) / 100;
                    machine->ram[machine->I + 1] = (X(opcode) / 10) % 10;
                    machine->ram[machine->I + 2] = X(opcode) % 10;
                    break;

                case 0x0055: // LD: Store registers V0 through VX in memory starting at address I.
                    for (int i = 0; i <= X(opcode); i++) {
                        machine->ram[machine->I + i] = machine->V[i];
                    }
                    break;

                case 0x0065: // LD: Read registers V0 through VX from memory starting at address I.
                    for (int i = 0; i <= X(opcode); i++) {
                        machine->V[i] = machine->ram[machine->I + i];
                    }
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
