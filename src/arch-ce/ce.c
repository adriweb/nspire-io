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


#include "../include/nspireio/platform.h"
#include "../include/nspireio/nspireio.h"

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

void nio_set_global_color(unsigned int color)
{
    gfx_SetColor(getPaletteColorIndex(color));
}

void nio_vram_pixel_set(unsigned int x, unsigned int y)
{
    gfx_SetPixel(x, (uint8_t)y);
}

void nio_vram_fill(unsigned x, unsigned y, unsigned w, unsigned h)
{
    gfx_FillRectangle_NoClip(x, (uint8_t) y, w, (uint8_t) h);
}

void nio_vram_scroll(unsigned x, unsigned y, unsigned w, unsigned h, unsigned scroll) {
    if (!scroll) return;
    gfx_ShiftUp((uint8_t) scroll);

// TODO: support non-full scroll
/*
    unsigned int r = h - scroll;

    uint16_t *p1 = (uint16_t*)gfx_vram+x+SCREEN_WIDTH*y;
    uint16_t *p2 = p1 + SCREEN_WIDTH*scroll;
    size_t n = sizeof(uint16_t)*w;
    for (unsigned int i = 0; i < r; ++i, p1 += SCREEN_WIDTH, p2 += SCREEN_WIDTH)
        memmove(p1, p2, n);
 */

    nio_vram_fill(x, y + h - scroll, w, scroll);
}

void nio_vram_draw(void)
{
    // we could do gfx_Blit...
}

unsigned int nio_cursor_clock(void) {
    return (unsigned int) rtc_Time();
}

unsigned nio_time_get() {
    return (unsigned int) rtc_Time();
}

bool second = false, alpha = false;
char nio_ascii_get(int* adaptive_cursor_state)
{
    sk_key_t key = os_GetCSC();

    if (key == sk_2nd) {
        second = (bool)!second;
    }
    if (key == sk_Alpha) {
        alpha = (bool)!alpha;
    }

    if (alpha && second) {
        *adaptive_cursor_state = 2; // uppercase alpha cursor
    } else if (alpha) {
        *adaptive_cursor_state = 3; // lowercase alpha cursor
    } else if (second) {
        *adaptive_cursor_state = 1; // arrow cursor
    } else {
        *adaptive_cursor_state = 0; // block cursor
    }

    switch (key)
    {
        case sk_Mode  : return 0;
        case sk_Clear : return (char) NIO_KEY_ESC;

        case sk_DecPnt: return (char) (alpha ?                 ':'  : '.');
        case sk_Chs   : return (char) (alpha ?                 '?'  : '-');
        case sk_0     : return (char) (alpha ?                 ' '  : '0');
        case sk_1     : return (char) (alpha ? (second ? 'Y' : 'y') : '1');
        case sk_2     : return (char) (alpha ? (second ? 'Z' : 'z') : '2');
        case sk_3     : return (char) (alpha ?                 '@'  : '3');
        case sk_4     : return (char) (alpha ? (second ? 'T' : 't') : '4');
        case sk_5     : return (char) (alpha ? (second ? 'U' : 'u') : '5');
        case sk_6     : return (char) (alpha ? (second ? 'V' : 'v') : '6');
        case sk_7     : return (char) (alpha ? (second ? 'O' : 'o') : '7');
        case sk_8     : return (char) (alpha ? (second ? 'P' : 'p') : '8');
        case sk_9     : return (char) (alpha ? (second ? 'Q' : 'q') : '9');

        case sk_Math  : return (char) (alpha ? (second ? 'A' : 'a') :  1 );
        case sk_Apps  : return (char) (alpha ? (second ? 'B' : 'b') :  1 );
        case sk_Prgm  : return (char) (alpha ? (second ? 'C' : 'c') :  1 );
        case sk_Recip : return (char) (alpha ? (second ? 'D' : 'd') : '\'');
        case sk_Sin   : return (char) (alpha ? (second ? 'E' : 'e') :  1 );
        case sk_Cos   : return (char) (alpha ? (second ? 'F' : 'f') :  1 );
        case sk_Tan   : return (char) (alpha ? (second ? 'G' : 'g') :  1 );
        case sk_Square: return (char) (alpha ? (second ? 'I' : 'i') : ';');
        case sk_Comma : return (char) (alpha ? (second ? 'J' : 'j') : ',');
        case sk_LParen: return (char) (alpha ? (second ? 'K' : 'k') : (second ? '{' : '('));
        case sk_RParen: return (char) (alpha ? (second ? 'L' : 'l') : (second ? '}' : ')'));
        case sk_Log   : return (char) (alpha ? (second ? 'N' : 'n') : '%');
        case sk_Ln    : return (char) (alpha ? (second ? 'S' : 's') : '<');
        case sk_Store : return (char) (alpha ? (second ? 'X' : 'x') : '>');

        case sk_Add   : return (char) (alpha ?                 '"'  : '+');
        case sk_Sub   : return (char) (alpha ? (second ? 'W' : 'w') : (second ? ']' : '-'));
        case sk_Mul   : return (char) (alpha ? (second ? 'R' : 'r') : (second ? '[' : '*'));
        case sk_Div   : return (char) (alpha ? (second ? 'M' : 'm') : '/');
        case sk_Power : return (char) (alpha ? (second ? 'H' : 'h') : '^');

        case sk_Vars  : return '#';
        case sk_Yequ  : return '=';
        case sk_Del   : return '\b';
        case sk_Enter : return '\n';
        case sk_Up    : return (char) NIO_KEY_UP;
        case sk_Down  : return (char) NIO_KEY_DOWN;

        case sk_GraphVar: return 'X'; // why not

        default: return 1; // no key pressed
    }
}
