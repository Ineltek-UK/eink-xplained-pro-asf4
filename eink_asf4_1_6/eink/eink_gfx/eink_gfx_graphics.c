/**
 * \file
 * 
 * \brief Eink Graphics Service
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.6 - July 2020
 * 
 * \author George Sephton
 * 
 * \copyright Copyright (c) 2020 Ineltek Ltd. All rights reserved.
 * 
 * \page eink_xplained_license License
 *
 * \brief Software License and Disclaimer
 *
 * \section software_license Software License
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * For a copy of the GNU General Public License, see
 * see <https://www.gnu.org/licenses/>
 *
 * \subsection software_license_logo Logo Use
 *
 * Use of the Ineltek logo, packaged with this software library, is not subject
 * to the above GPL software license and is property of Ineltek GmbH and it subsidiaries.
 * It may not be used for commercial use and may not be used to endorse or promote products
 * without specific prior written permission.
 *
 * \subsection software_license_conditions Conditions
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    software license, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    software license, this list of conditions and the following disclaimer
 *    in the documentation and/or other materials provided with the distribution.
 *
 * 3. The names, trademarks or logos of Eink, Ineltek or Microchip, and any combination
 *    thereof, may not be used to endorse or promote products derived from this
 *    software without specific prior written permission. They are also subject to their
 *    own copyrights and trademarks that may not be expressed here.
 *
 * \section disclaimer Disclaimer
 *
 * THIS SOFTWARE IS PROVIDED BY INELTEK "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL INELTEK BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#include <eink/eink_gfx/eink_gfx_graphics.h>

/**
 * \brief Fill display buffers with a single color
 *
 * \param fill_value The pixel value to fill the display buffer with.
 */
void gfx_eink_graphics_fill_screen_raw(uint8_t fill_value)
{
    /* Fill entire display buffer. */
    memset(ptr_eink_gfx_config->display_buffer_1_ptr, fill_value, ptr_eink_gfx_config->display_buffer_size);
}

/**
 * \brief Fill display buffers with a single color
 *
 * \param fill_type Selects the color to fill the display buffer with.
 * 
 * \note Selecting FILL_NONE as the color will be ignored.
 */
void gfx_eink_graphics_fill_screen(enum eink_fill_colour fill_color)
{
    volatile eink_coordinate dx, width_fill;
    volatile eink_coordinate dy, height_fill;
    
    width_fill = ptr_eink_gfx_config->display_width;
    height_fill = ptr_eink_gfx_config->display_height;
    
    for(dx = 0; dx < width_fill; dx++) {
        for(dy = 0; dy < height_fill; dy++) {
            if(ptr_eink_gfx_config->gfx_display == GFX_AC057TC1)
            {
                switch (fill_color) {
                    case FILL_BLACK:  gfx_eink_set_pixel(dx, dy, PIXEL_BLACK); break;
                    case FILL_WHITE:  gfx_eink_set_pixel(dx, dy, PIXEL_WHITE); break;
                    case FILL_YELLOW: gfx_eink_set_pixel(dx, dy, PIXEL_YELLOW); break;
                    case FILL_RED:    gfx_eink_set_pixel(dx, dy, PIXEL_RED); break;
                    case FILL_GREEN:  gfx_eink_set_pixel(dx, dy, PIXEL_GREEN); break;
                    case FILL_BLUE:   gfx_eink_set_pixel(dx, dy, PIXEL_BLUE); break;
                    case FILL_ORANGE: gfx_eink_set_pixel(dx, dy, PIXEL_ORANGE); break;
                    case FILL_CLEAN:  gfx_eink_set_pixel(dx, dy, PIXEL_CLEAN); break;
                }
            } else {
                switch (fill_color) {
                    case FILL_BLACK:  gfx_eink_set_pixel(dx, dy, PIXEL_BLACK); break;
                    case FILL_WHITE:  gfx_eink_set_pixel(dx, dy, PIXEL_WHITE); break;
                    case FILL_YELLOW:
                    case FILL_RED:    gfx_eink_set_pixel(dx, dy, PIXEL_RED); break;
                }
            }
        }
    }
}

/**
 * \brief Add a line to the display buffer from (x0,y0) to (x1,y1).
 *
 * \param x0 Starting X coordinate.
 * \param y0 Starting Y coordinate.
 * \param x1 Ending X coordinate.
 * \param y1 Ending Y coordinate.
 * \param line_raw Whether we're drawing with a pixel value or specified color.
 * \param line_colour Color of the line to draw.
 * \param line_value Pixel value of the line to draw.
 */
void _gfx_eink_graphics_draw_line(eink_coordinate x0, eink_coordinate y0, eink_coordinate x1, eink_coordinate y1, uint8_t line_raw, enum eink_pixel_colour line_colour, uint8_t line_value)
{
    uint8_t steep = 0;
    eink_coordinate dx, sx;
    eink_coordinate dy, sy;
    eink_coordinate temp, err, e2;
    
    /* Bresenham's Line Algorithm - fix vertical line issue */
    if(abs(y1 - y0) > abs(x1 - x0)) {
        steep = 1;
        // swap x0 and y0
        temp = x0;
        x0 = y0;
        y0 = temp;
        // swap x1 and y1
        temp = x1;
        x1 = y1;
        y1 = temp;
    }
    
    if(x0 > x1) {
        // swap x0 and x1
        temp = x0;
        x0 = x1;
        x1 = temp;
        // swap y0 and y1
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    
    /* Bresenham's Line Algorithm */
    dx = abs(x1 - x0);
    if (x0 < x1) sx = 1;
    else sx = -1;
    dy = abs(y1 - y0);
    if (y0 < y1) sy = 1;
    else sy = -1;
    err = (dx >= dy ? dx : -dy) /2, e2;
    
    while (1) {
        if(!line_raw)
        {
            if(steep) gfx_eink_set_pixel(y0, x0, line_colour);
            else gfx_eink_set_pixel(x0, y0, line_colour);   
        }else{
            if(steep) gfx_eink_set_pixel_raw(y0, x0, line_value);
            else gfx_eink_set_pixel_raw(x0, y0, line_value);
        }
        
        if ( (x0 == x1) && (y0 == y1) ) break;
        e2 = err;
        if (e2 > (-dx)) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
}

/**
 * \brief Add a line to the display buffer from (x0,y0) to (x1,y1).
 *
 * \param x0 Starting X coordinate.
 * \param y0 Starting Y coordinate.
 * \param x1 Ending X coordinate.
 * \param y1 Ending Y coordinate.
 * \param line_colour Color of the line to draw.
 */
void gfx_eink_graphics_draw_line(eink_coordinate x0, eink_coordinate y0, eink_coordinate x1, eink_coordinate y1, enum eink_pixel_colour line_colour)
{
    _gfx_eink_graphics_draw_line(x0, y0, x1, y1, 0, line_colour, 0);
}

/**
 * \brief Add a line to the display buffer from (x0,y0) to (x1,y1).
 *
 * \param x0 Starting X coordinate.
 * \param y0 Starting Y coordinate.
 * \param x1 Ending X coordinate.
 * \param y1 Ending Y coordinate.
 * \param line_value Pixel value of the line to draw.
 */
void gfx_eink_graphics_draw_line_raw(eink_coordinate x0, eink_coordinate y0, eink_coordinate x1, eink_coordinate y1, uint8_t line_value)
{
    _gfx_eink_graphics_draw_line(x0, y0, x1, y1, 1, 0, line_value);
}

/**
 * \brief Add a rectangle to the display buffer starting at (x0,y0) with set width and height.
 *
 * \param x0 Starting X coordinate.
 * \param y0 Starting Y coordinate.
 * \param width Width of rectangle.
 * \param height Height of rectangle.
 * \param line_colour Color of the rectangle border.
 * \param fill_type Color of the rectangle fill.
 *
 * \note Both the border and the fill can be set to blank should you want a border-less or empty rectangle.
 */
void gfx_eink_graphics_draw_rect(eink_coordinate x0, eink_coordinate y0, eink_coordinate width, eink_coordinate height, enum eink_pixel_colour line_colour, enum eink_fill_colour fill_colour)
{
    eink_coordinate i, j;
    
    /* Draw rectangle borders first */
    for (i = x0; i < (x0+width); i++) {
        gfx_eink_set_pixel(i, y0, line_colour);
        gfx_eink_set_pixel(i, (y0 + height - 1), line_colour);
    }
    for (i = y0; i < (y0+height); i++) {
        gfx_eink_set_pixel(x0, i, line_colour);
        gfx_eink_set_pixel((x0 + width - 1), i, line_colour);
    }
    
    /* Add a fill if necessary */
    if (fill_colour != FILL_NONE) {
        for (i = (x0 + 1); i < (x0 + width - 1); i++) {
            for (j = (y0 + 1); j < (y0 + height - 1); j++) {
                if(ptr_eink_gfx_config->gfx_display == GFX_AC057TC1)
                {
                    switch (fill_colour) {
                        case FILL_BLACK:  gfx_eink_set_pixel(i, j, PIXEL_BLACK); break;
                        case FILL_WHITE:  gfx_eink_set_pixel(i, j, PIXEL_WHITE); break;
                        case FILL_YELLOW: gfx_eink_set_pixel(i, j, PIXEL_YELLOW); break;
                        case FILL_RED:    gfx_eink_set_pixel(i, j, PIXEL_RED); break;
                        case FILL_GREEN:  gfx_eink_set_pixel(i, j, PIXEL_GREEN); break;
                        case FILL_BLUE:   gfx_eink_set_pixel(i, j, PIXEL_BLUE); break;
                        case FILL_ORANGE: gfx_eink_set_pixel(i, j, PIXEL_ORANGE); break;
                        case FILL_CLEAN:  gfx_eink_set_pixel(i, j, PIXEL_CLEAN); break;
                    }
                } else {
                    switch (fill_colour) {
                        case FILL_BLACK:  gfx_eink_set_pixel(i, j, PIXEL_BLACK); break;
                        case FILL_WHITE:  gfx_eink_set_pixel(i, j, PIXEL_WHITE); break;
                        case FILL_YELLOW:
                        case FILL_RED:    gfx_eink_set_pixel(i, j, PIXEL_RED); break;
                    }
                }
            }
        }
    }
}

/**
 * \brief Add a rectangle to the display buffer starting at (x0,y0) with set width and height.
 *
 * \param x0 Starting X coordinate.
 * \param y0 Starting Y coordinate.
 * \param width Width of rectangle.
 * \param height Height of rectangle.
 * \param line_value Pixel value of the rectangle border.
 * \param fill_type Should the rectangle be filled or unfilled.
 * \param fill_value Pixel value of the rectangle fill.
 *
 * \note Both the border and the fill can be set to blank should you want a border-less or empty rectangle.
 */
void gfx_eink_graphics_draw_rect_raw(eink_coordinate x0, eink_coordinate y0, eink_coordinate width, eink_coordinate height, uint8_t line_value, enum eink_fill_type fill_type, uint8_t fill_value)
{
    eink_coordinate i, j;
    
    /* Draw rectangle borders first */
    for (i = x0; i < (x0+width); i++) {
        gfx_eink_set_pixel_raw(i, y0, line_value);
        gfx_eink_set_pixel_raw(i, (y0 + height - 1), line_value);
    }
    for (i = y0; i < (y0+height); i++) {
        gfx_eink_set_pixel_raw(x0, i, line_value);
        gfx_eink_set_pixel_raw((x0 + width - 1), i, line_value);
    }
    
    /* Add a fill if necessary */
    if (fill_type != FILL_NO_FILL) {
        for (i = (x0 + 1); i < (x0 + width - 1); i++) {
            for (j = (y0 + 1); j < (y0 + height - 1); j++) {
                gfx_eink_set_pixel_raw(i, j, fill_value);
            }
        }
    }
}