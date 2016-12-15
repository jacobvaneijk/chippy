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

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    FILE *rom;
} chip8;

chip8* chip8_new(const char *rom);
int chip8_run(chip8 *chippy);

#endif
