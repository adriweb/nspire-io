/**
 * @file ce.c
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
 * Functions for CE platform
 */

#ifndef CEBUILD
#define CEBUILD
#endif

#include "../include/nspireio/platform.h"
#include "../include/nspireio/nspireio.h"

static void* scrbuf = gfx_vram;

bool any_key_pressed()
{
    return kb_AnyKey();
}

void wait_key_pressed(void)
{
    while (!any_key_pressed());
}

bool nio_scrbuf_init()
{
    gfx_Begin();
    return true;
}

void nio_scrbuf_clear()
{
    gfx_ZeroScreen();
}

void nio_scrbuf_free()
{
    scrbuf = NULL;
    gfx_End();
}

uint8_t getPaletteColorIndex(unsigned int color)
{
    if(color < 16)
    {
        // To match the nspireio colo enum
        const uint8_t palette[16] = {
            gfx_black,
            gfx_red,
            gfx_green,
            gfx_yellow,
            gfx_blue,
            gfx_purple, // magenta
            gfx_blue,   // cyan
            gfx_white,  // grey
            gfx_black,  // light black
            gfx_red,    // light red
            gfx_green,  // light green
            gfx_yellow, // light yellow
            gfx_blue,   // light blue
            gfx_purple, // light magenta
            gfx_blue,   // light cyan
            gfx_white
        };
        return palette[color];
    }
    else if(color < 232)
    {
        // todo: fix me
        const int rbtable[6] = {0,6,12,18,24,31};
        const int gtable[6] = {0,12,25,37,50,63};
        unsigned int d = color-16;
        return (uint8_t) ((rbtable[d / 36] << 11) + (gtable[(d / 6) % 6] << 5) + rbtable[d % 6]);
    }
    else if(color < 256)
    {
        // todo: fix me
        unsigned int d = color-232;
        return (uint8_t) (((d + 1) << 11) + ((d * 2 + 2) << 5) + (d + 1));
    }
    return 0;
}

void nio_vram_pixel_set(unsigned int x, unsigned int y, unsigned int color)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
    gfx_SetColor(getPaletteColorIndex(color));
    gfx_SetPixel(x, (uint8_t)y);
}

void nio_vram_fill(const unsigned x, const unsigned y, const unsigned w, const unsigned h, unsigned color)
{
    gfx_SetColor(getPaletteColorIndex(color));
    gfx_FillRectangle_NoClip(x, (uint8_t) y, w, (uint8_t) h);
}

void nio_vram_scroll(const unsigned x, const unsigned y, const unsigned w, const unsigned h, const unsigned scroll, const unsigned color) {
    if (!scroll) return;
    gfx_ShiftUp((uint8_t) scroll);

// TODO: support non-full scroll
/*
    unsigned int r = h - scroll;

    uint16_t *p1 = (uint16_t*)scrbuf+x+SCREEN_WIDTH*y;
    uint16_t *p2 = p1 + SCREEN_WIDTH*scroll;
    size_t n = sizeof(uint16_t)*w;
    for (unsigned int i = 0; i < r; ++i, p1 += SCREEN_WIDTH, p2 += SCREEN_WIDTH)
        memmove(p1, p2, n);
 */

    nio_vram_fill(x, y + h - scroll, w, scroll, color);
}

void nio_vram_draw(void)
{
    // we could do gfx_Blit...
}

unsigned int nio_cursor_clock(void) {
    return (unsigned int) rtc_Time();
}

/*  adaptive_cursor_state
   	 block cursor
	 arrow cursor
	 'A' cursor
	 'a' cursor
	 '#' cursor
 */
char nio_ascii_get(int* adaptive_cursor_state)
{
    bool second, alpha;

    kb_Scan();

    second = (bool) (kb_Data[1] & kb_2nd);
    alpha  = (bool) (kb_Data[2] & kb_Alpha);

    if (alpha && second) {
        *adaptive_cursor_state = 2;
    } else if (alpha) {
        *adaptive_cursor_state = 3;
    } else if (second) {
        *adaptive_cursor_state = 1;
    } else {
        *adaptive_cursor_state = 0;
    }

    if (kb_Data[1] & kb_Mode ) return 0;
    if (kb_Data[6] & kb_Clear) return (char) NIO_KEY_ESC;

    if (kb_Data[4] & kb_DecPnt)return (char) (alpha ? ':' : '.');
    if (kb_Data[5] & kb_Chs  ) return (char) (alpha ? '?' : '-');
    if (kb_Data[3] & kb_0    ) return (char) (alpha ? ' ' : '0');
    if (kb_Data[3] & kb_1    ) return (char) (alpha ? (second ? 'Y' : 'y') : '1');
    if (kb_Data[4] & kb_2    ) return (char) (alpha ? (second ? 'Z' : 'z') : '2');
    if (kb_Data[5] & kb_3    ) return (char) (alpha ? '@' : '3');
    if (kb_Data[3] & kb_4    ) return (char) (alpha ? (second ? 'T' : 't') : '4');
    if (kb_Data[4] & kb_5    ) return (char) (alpha ? (second ? 'U' : 'u') : '5');
    if (kb_Data[5] & kb_6    ) return (char) (alpha ? (second ? 'V' : 'v') : '6');
    if (kb_Data[3] & kb_7    ) return (char) (alpha ? (second ? 'O' : 'o') : '7');
    if (kb_Data[4] & kb_8    ) return (char) (alpha ? (second ? 'P' : 'p') : '8');
    if (kb_Data[5] & kb_9    ) return (char) (alpha ? (second ? 'Q' : 'q') : '9');

    if (kb_Data[2] & kb_Math ) return (char) (alpha ? (second ? 'A' : 'a') :  1 );
    if (kb_Data[3] & kb_Apps ) return (char) (alpha ? (second ? 'B' : 'b') :  1 );
    if (kb_Data[4] & kb_Prgm ) return (char) (alpha ? (second ? 'C' : 'c') :  1 );
    if (kb_Data[2] & kb_Recip) return (char) (alpha ? (second ? 'D' : 'd') : '\'');
    if (kb_Data[3] & kb_Sin  ) return (char) (alpha ? (second ? 'E' : 'e') :  1 );
    if (kb_Data[4] & kb_Cos  ) return (char) (alpha ? (second ? 'F' : 'f') :  1 );
    if (kb_Data[5] & kb_Tan  ) return (char) (alpha ? (second ? 'G' : 'g') :  1 );
    if (kb_Data[2] & kb_Square)return (char) (alpha ? (second ? 'I' : 'i') : ';');
    if (kb_Data[3] & kb_Comma) return (char) (alpha ? (second ? 'J' : 'j') : ',');
    if (kb_Data[4] & kb_LParen)return (char) (alpha ? (second ? 'K' : 'k') : '(');
    if (kb_Data[5] & kb_RParen)return (char) (alpha ? (second ? 'L' : 'l') : ')');
    if (kb_Data[2] & kb_Log  ) return (char) (alpha ? (second ? 'N' : 'n') : '%');
    if (kb_Data[2] & kb_Ln   ) return (char) (alpha ? (second ? 'S' : 's') : '<');
    if (kb_Data[2] & kb_Store) return (char) (alpha ? (second ? 'X' : 'x') : '>');

    if (kb_Data[3] & kb_GraphVar) return 'X'; // why not

    if (kb_Data[6] & kb_Add  ) return (char) (alpha ? '"' : '+');
    if (kb_Data[6] & kb_Sub  ) return (char) (alpha ? (second ? 'W' : 'w') : '-');
    if (kb_Data[6] & kb_Mul  ) return (char) (alpha ? (second ? 'R' : 'r') : '*');
    if (kb_Data[6] & kb_Div  ) return (char) (alpha ? (second ? 'M' : 'm') : '/');
    if (kb_Data[6] & kb_Power) return (char) (alpha ? (second ? 'H' : 'h') : '^');

    // Special chars
    if (kb_Data[5] & kb_Vars ) return '#';
    if (kb_Data[1] & kb_Yequ ) return '=';
    if (kb_Data[1] & kb_Del  ) return '\b';
    if (kb_Data[6] & kb_Enter) return '\n';
    if (kb_Data[7] & kb_Up   ) return (char) NIO_KEY_UP;
    if (kb_Data[7] & kb_Down ) return (char) NIO_KEY_DOWN;

    // no key pressed
    return 1;
}
