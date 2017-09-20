/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef __CHIPPY_H__
#define __CHIPPY_H__

#include <stdint.h>

/**
 * The location where CHIP-8 programs start. The first 512 bytes (0x000 to
 * 0x1FF) are where the original interpreter was located, and should not be used
 * by  programs.
 */
#define PROGRAM_START 0x200

/**
 * This is the maximum amount of addressable memory by the machine. The CHIP-8
 * language is capable of accessing up to 4kB of RAM.
 */
#define RAM_SIZE 0x1000

/**
 * The original implementation of the CHIP-8 language used a 64x32-pixel
 * monochrome display.
 */
#define SCREEN_W 64
#define SCREEN_H 32

/**
 * The fontset is a group of sprites reprisenting the hexadecimal digits 0
 * through F. These sprites are 5 bytes long, or 8x5 pixels.
 *
 * The fontset should be stored in the interpreter area of the RAM (0x000 to
 * 0x1FF).
 */
static uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/**
 * This is the main data structure for holding information and state about the
 * machine.
 */
struct chippy {
    uint8_t ram[RAM_SIZE];              // Memory (4kB)
    uint8_t V[16];                      // 16 general purpose 8-bit registers

    uint8_t delay_timer;                // Delay timer
    uint8_t sound_timer;                // Sound timer

    uint16_t pc;                        // Program counter
    uint16_t I;                         // Index register

    uint16_t sp;                        // Stack pointer
    uint16_t stack[16];                 // Stack

    uint8_t gfx[SCREEN_W * SCREEN_H];   // Graphics buffer
    uint8_t key[16];                    // Keypad
};

/**
 * Initializes a machine data structure. This function can
 * also be called to reinitialize the machine.
 *
 * @param machine The machine data structure to be initialized.
 */
void chippy_init(struct chippy* machine);

/**
 * Performs exactly one instruction cycle.
 *
 * @param machine The machine data structure to step.
 *
 * @return Returns 0 on success, otherwise 1.
 */
int chippy_step(struct chippy* machine);

/**
 * Frees the memory allocated for the machine.
 *
 * @param machine The machine data structure to destroy.
 */
void chippy_destroy(struct chippy* machine);

#endif
