/*
 * Copyright (C) 2020  Iulian Gheorghiu (morgoth@mail.devboard.tech)
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


/*
 * This file is custom made for each design.
 */


#ifndef DEF_H_
#define DEF_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdint.h>

#define MAX_PASSWORD_LENGTH				20
#define MIN_PASSWORD_LENGTH				4
#define PASSWORD_BUF_LENGTH				65

//#define BACK_TO_TABLES

//#define KBD_USE_INIT
#define KBD_USE_TICK

//#define USE_ST7735
//#define USE_ST7789
#define USE_SSD1306

#ifdef USE_ST7735
#define DISPLAY_DRIVER_FILE				"device/st7735.h"
#endif
#ifdef USE_ST7789
#define DISPLAY_DRIVER_FILE				"device/st7789.h"
#endif
#ifdef USE_SSD1306
#define DISPLAY_DRIVER_FILE				"device/ssd1306.h"
#endif

#define IMPLEMENTED
#define GUI_EXPLORER

#define MAX_DISK_NR						1
//#define SSD1306_USE_NO_BUF
//#define ST7735S_128x128
#define ST7735_BW_MODE

#define MAX_ALLOWED_PATH_LEN_BUF		64
#define MAX_ALLOWED_FILE_NAME_LEN_BUF	32

#include "def_plat.h"

/*********************************************************************************************************************************/

#define APP_INIT_FUNC																								app_init(uSD, spi_screen, screen_buf)
#define APP_IDLE_FUNC																								app_idle(uSD, spi_screen, screen_buf)

#ifdef USE_ST7735
#define DISPLAY_FUNC_INIT(spi, screen_buf)																			st7735_init(spi, screen_buf)
#define DISPLAY_FUNC_GET_X()																						st7735_get_x()
#define DISPLAY_FUNC_GET_Y()																						st7735_get_y()
#define DISPLAY_FUNC_CLEAR(inst, buf, inkColor)																		st7735_clear(inst, buf, inkColor);
#define DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, x, y, inkColor)														st7735_draw_pixel(inst, box, buf, x, y, inkColor)
#define DISPLAY_FUNC_DRAW_RECTANGLE(inst, box, buf, x, y, x_size, y_size, fill, inkColor)							st7735_draw_rectangle(inst, box, buf, x, y, x_size, y_size, fill, inkColor)
#define DISPLAY_FUNC_DRAW_STRING(inst, box, buf, string, x, y, terminalMode, wordWrap, foreColor, inkColor)			st7735_draw_string(inst, box, buf, string, x, y, terminalMode, wordWrap, foreColor, inkColor)
#define DISPLAY_FUNC_DRAW_H_LINE(inst, box, buf, x1, x2, y, width, inkColor)										st7735_draw_h_line(inst, box, buf, x1, x2, y, width, inkColor)
#define DISPLAY_FUNC_DRAW_V_LINE(inst, box, buf, y1, y2, x, width, inkColor)										st7735_draw_v_line(inst, box, buf, y1, y2, x, width, inkColor)
#define DISPLAY_FUNC_SET_CONTRAST(inst, value)																		st7735_set_contrast(inst, value)
//#define DISPLAY_FUNC_REFRESH(inst, box, buf)																		st7735_rfsh(inst, box, buf)

#define GUI_UPPER_LIMIT_ROW																							0
#define GUI_LOWER_LIMIT_ROW																							9
#endif
#ifdef USE_ST7789
#define DISPLAY_FUNC_INIT(spi, screen_buf)																			st7789_init(spi, screen_buf)
#define DISPLAY_FUNC_GET_X()																						st7789_get_x()
#define DISPLAY_FUNC_GET_Y()																						st7789_get_y()
#define DISPLAY_FUNC_CLEAR(inst, buf, inkColor)																		st7789_clear(inst, buf, inkColor);
#define DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, x, y, inkColor)														st7789_draw_pixel(inst, box, buf, x, y, inkColor)
#define DISPLAY_FUNC_DRAW_RECTANGLE(inst, box, buf, x, y, x_size, y_size, fill, inkColor)							st7789_draw_rectangle(inst, box, buf, x, y, x_size, y_size, fill, inkColor)
#define DISPLAY_FUNC_DRAW_STRING(inst, box, buf, string, x, y, terminalMode, wordWrap, foreColor, inkColor)			st7789_draw_string(inst, box, buf, string, x, y, terminalMode, wordWrap, foreColor, inkColor)
#define DISPLAY_FUNC_DRAW_H_LINE(inst, box, buf, x1, x2, y, width, inkColor)										st7789_draw_h_line(inst, box, buf, x1, x2, y, width, inkColor)
#define DISPLAY_FUNC_DRAW_V_LINE(inst, box, buf, y1, y2, x, width, inkColor)										st7789_draw_v_line(inst, box, buf, y1, y2, x, width, inkColor)
#define DISPLAY_FUNC_SET_CONTRAST(inst, value)																		st7789_set_contrast(inst, value)
//#define DISPLAY_FUNC_REFRESH(inst, box, buf)																		st7789_rfsh(inst, box, buf)

#define GUI_UPPER_LIMIT_ROW																							0
#define GUI_LOWER_LIMIT_ROW																							29
#endif
#ifdef USE_SSD1306
#define DISPLAY_FUNC_INIT(spi, screen_buf)																			ssd1306_init(spi, screen_buf)
#define DISPLAY_FUNC_GET_X()																						ssd1306_get_x()
#define DISPLAY_FUNC_GET_Y()																						ssd1306_get_y()
#define DISPLAY_FUNC_CLEAR(inst, buf, inkColor)																		ssd1306_clear(inst, buf, inkColor);
#define DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, x, y, inkColor)														ssd1306_draw_pixel(inst, box, buf, x, y, inkColor)
#define DISPLAY_FUNC_DRAW_RECTANGLE(inst, box, buf, x, y, x_size, y_size, fill, inkColor)							ssd1306_draw_rectangle(inst, box, buf, x, y, x_size, y_size, fill, inkColor)
#define DISPLAY_FUNC_DRAW_STRING(inst, box, buf, string, x, y, terminalMode, wordWrap, foreColor, inkColor)			ssd1306_draw_string(inst, box, buf, string, x, y, terminalMode, wordWrap, foreColor, inkColor)
#define DISPLAY_FUNC_DRAW_H_LINE(inst, box, buf, x1, x2, y, width, inkColor)										ssd1306_draw_h_line(inst, box, buf, x1, x2, y, width, inkColor)
#define DISPLAY_FUNC_DRAW_V_LINE(inst, box, buf, y1, y2, x, width, inkColor)										ssd1306_draw_v_line(inst, box, buf, y1, y2, x, width, inkColor)
#define DISPLAY_FUNC_SET_CONTRAST(inst, value)																		ssd1306_set_contrast(inst, value)
#define DISPLAY_FUNC_REFRESH(inst, buf)																				ssd1306_rfsh(inst, buf)

#define GUI_UPPER_LIMIT_ROW																							0
#define GUI_LOWER_LIMIT_ROW																							7
#endif

#define GUI_APP_FILE_INCLUDE																						"app-music-player.h"
#define GUI_LIST_OF_DISPLAYED_EXTENSIONS																			{"MP3","WMA","FLA"} // Upper cases
#define GUI_ACT_FUNC_ON_FILE_SELECT(uSD, spi_screen, screen_buf)													app_play(uSD, spi_screen, screen_buf)
#define GUI_ACT_FUNC_AT_uSD_INSERT(uSD, spi_screen, screen_buf)														app_sd_insert(uSD, spi_screen, screen_buf)
#define GUI_ACT_FUNC_ON_UP_BTN_PRESS(uSD, spi_screen, screen_buf)													app_up(uSD, spi_screen, screen_buf)
#define GUI_ACT_FUNC_ON_DN_BTN_PRESS(uSD, spi_screen, screen_buf)													app_dn(uSD, spi_screen, screen_buf)
#define GUI_ACT_FUNC_ON_LEFT_BTN_PRESS(uSD, spi_screen, screen_buf)													app_left(uSD, spi_screen, screen_buf)
#define GUI_ACT_FUNC_ON_RIGHT_BTN_PRESS(uSD, spi_screen, screen_buf)												app_right(uSD, spi_screen, screen_buf)

/*******************************************/
#endif /* DEF_H_ */
