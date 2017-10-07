/**
 * @file nspire.h
 * @author Adrien "Adriweb" Bertrand
 * @version 1.0
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
 * Header for CE platform
 */
 
#ifndef CE_H
#define CE_H

#ifndef CEBUILD
#define CEBUILD
#endif

// Put headers required for our platform here
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

// Fullscreen definitions

#define NIO_MAX_ROWS 30
#define NIO_MAX_COLS 53

#define SCREEN_WIDTH    gfx_lcdWidth
#define SCREEN_HEIGHT   gfx_lcdHeight

#define idle() 0 // Power saver on the Nspire, useless on the Prizm
void wait_key_pressed(void); // blocks until a key is pressed
bool any_key_pressed(void); // non-blocking, TRUE if any key pressed

#define strerror(errno) "errno"


/** Initializes double buffering.
*/
bool nio_scrbuf_init();

/** Clears the screen buffer.
*/
void nio_scrbuf_clear();

/** Frees the screenbuffer and restores the screen to its original state.
*/
void nio_scrbuf_free();

#endif
