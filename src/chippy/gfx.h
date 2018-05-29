/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef __GFX_H__
#define __GFX_H__

#include "libchippy/chippy.h"

void set_pixel(int x, int y, int scale);

/**
 * The size of a CHIP-8 pixel.
 */
#define PIXEL_SIZE 10

/**
 * Initializes the graphics context.
 */
int gfx_init(int scale);

/**
 * Destroys the graphics context.
 */
void gfx_destroy(void);

/**
 * Renders the screen buffer from the given machine into the SDL texture.
 */
int gfx_render(struct chippy *machine, int scale);

/**
 * Checks whether the user requested to close the application.
 */
int gfx_close_requested(void);

#endif
