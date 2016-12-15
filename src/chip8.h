/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef __CHIP8_H__
#define __CHIP8_H__

#include <stdint.h>

#define PROGRAM_START 0x200
#define RAM_SIZE 0x1000
#define SCREEN_W 64
#define SCREEN_H 32

static uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

typedef struct {
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
} chip8;

chip8* chip8_new(const char *rom);
void chip8_free(chip8 *chippy);
int chip8_cycle(chip8 *chippy);

#endif
