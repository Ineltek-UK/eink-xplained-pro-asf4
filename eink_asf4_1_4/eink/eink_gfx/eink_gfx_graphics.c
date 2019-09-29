/**
 * \file
 * 
 * \brief Eink Graphics Service
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.4 - October 2019
 * 
 * \author George Sephton
 * 
 * \copyright Copyright (c) 2019 Ineltek Ltd. All rights reserved.
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
 * \param fill_type Selects the color to fill the display buffer with.
 * 
 * \note Selecting FILL_NONE as the color will be ignored.
 */
void gfx_eink_graphics_fill_screen(enum eink_fill_type fill_color)
{
    volatile eink_x_coordinate dx, width_fill;
    volatile eink_y_coordinate dy, height_fill;
    
    width_fill = ptr_eink_gfx_config->display_width;
    height_fill = ptr_eink_gfx_config->display_height;
    
    for(dx = 0; dx < width_fill; dx++) {
        for(dy = 0; dy < height_fill; dy++) {
            switch (fill_color) {
                case FILL_BLACK: gfx_eink_set_pixel(dx, dy, PIXEL_BLACK); break;
                case FILL_WHITE: gfx_eink_set_pixel(dx, dy, PIXEL_WHITE); break;
                case FILL_YELLOW:
                case FILL_RED:   gfx_eink_set_pixel(dx, dy, PIXEL_RED); break;
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
 * \param line_colour Color of the line to draw.
 */
void gfx_eink_graphics_draw_line(eink_x_coordinate x0, eink_y_coordinate y0, eink_x_coordinate x1, eink_y_coordinate y1, enum eink_pixel_colour line_colour)
{
    uint8_t steep = 0;
    eink_x_coordinate dx, sx;
    eink_y_coordinate dy, sy;
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
        if(steep) gfx_eink_set_pixel(y0, x0, line_colour);
        else gfx_eink_set_pixel(x0, y0, line_colour);
        
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
void gfx_eink_graphics_draw_rect(eink_x_coordinate x0, eink_y_coordinate y0, eink_x_coordinate width, eink_y_coordinate height, enum eink_pixel_colour line_colour, enum eink_fill_type fill_type)
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
    if (fill_type != FILL_NONE) {
        for (i = (x0 + 1); i < (x0 + width - 1); i++) {
            for (j = (y0 + 1); j < (y0 + height - 1); j++) {
                switch (fill_type) {
                    case FILL_BLACK: gfx_eink_set_pixel(i, j, PIXEL_BLACK); break;
                    case FILL_WHITE: gfx_eink_set_pixel(i, j, PIXEL_WHITE); break;
                    case FILL_YELLOW:
                    case FILL_RED:   gfx_eink_set_pixel(i, j, PIXEL_RED); break;  
                }
            }
        }
    }
}