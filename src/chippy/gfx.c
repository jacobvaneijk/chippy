/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "gfx.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "libchippy/chippy.h"

static SDL_Window *window = NULL;

static SDL_Renderer *renderer = NULL;

static SDL_Texture *texture = NULL;

int gfx_init(int scale) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return 1;
    }

    int width = SCREEN_W * PIXEL_SIZE * scale;
    int height = SCREEN_H * PIXEL_SIZE * scale;

    window = SDL_CreateWindow(
        PACKAGE_NAME,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        return 1;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        gfx_destroy();
        return 1;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height);

    if (texture == NULL) {
        gfx_destroy();
        return 1;
    }

    return 0;
}

void gfx_destroy(void) {
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }

    if (texture != NULL) {
        SDL_DestroyTexture(texture);
    }
}

static void render_pixel(int x, int y, int scale) {
    SDL_Rect rect;

    rect.x = x * PIXEL_SIZE * scale;
    rect.y = y * PIXEL_SIZE * scale;
    rect.w = PIXEL_SIZE * scale;
    rect.h = PIXEL_SIZE * scale;

    SDL_RenderFillRect(renderer, &rect);
}

int gfx_render(struct chippy *machine, int scale) {
    (void)machine;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < SCREEN_H; y++) {
        for (int x = 0; x < SCREEN_W; x++) {
            if (machine->gfx[SCREEN_W * y + x]) {
                render_pixel(x, y, scale);
            }
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);

    return 0;
}

int gfx_close_requested(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 1;
        }
    }

    return 0;
}
