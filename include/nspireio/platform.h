/**
 * @file platform.h
 * @author Julian Mackeben aka compu <compujuckel@googlemail.com>
 * @version 3.1
 *
 * @section LICENSE
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 *
 * @section DESCRIPTION
 *
 * Decide which header to use...
 */

// Includes for the different platforms here.

#ifndef PLATFORM_H
#define PLATFORM_H

#define NIO_CHAR_WIDTH 6
#define NIO_CHAR_HEIGHT 8

#include <stdbool.h>

#include "platform-ce.h"


// These functions are the same on all platforms...

void nio_set_global_color(unsigned int color);
void nio_vram_pixel_set(unsigned int x, unsigned int y);
void nio_vram_fill(unsigned x, unsigned y, unsigned w, unsigned h);
void nio_vram_draw(void);
void nio_vram_scroll(unsigned x, unsigned y, unsigned w, unsigned h, unsigned scroll);
unsigned int nio_cursor_clock(void);
char nio_ascii_get(int* adaptive_cursor_state);

#endif
