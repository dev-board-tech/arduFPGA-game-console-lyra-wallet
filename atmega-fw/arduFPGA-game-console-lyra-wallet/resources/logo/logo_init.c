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

#include <stdint.h>
#include "logo_init.h"

// 'logoInit_Black', 64x64px
const uint8_t logoInit_Black [] PROGMEM = {
	0x40, 0x40,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x03, 0xfe, 0x7f, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x0f, 0xfe, 0x7f, 0xf0, 0x00, 0x00,
	0x00, 0x00, 0x1f, 0xfe, 0x7f, 0xf8, 0x00, 0x00, 
	0x00, 0x00, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00,
	0x00, 0x00, 0xff, 0xfc, 0x7f, 0xff, 0x00, 0x00, 
	0x00, 0x01, 0xff, 0xfc, 0x3f, 0xff, 0x80, 0x00,
	0x00, 0x03, 0xff, 0xf8, 0x3f, 0xff, 0xc0, 0x00, 
	0x00, 0x07, 0xff, 0xf8, 0x3f, 0xff, 0xe0, 0x00,
	0x00, 0x0f, 0xff, 0xf8, 0x1f, 0xff, 0xf0, 0x00, 
	0x00, 0x1f, 0xff, 0xf0, 0x1f, 0xff, 0xf8, 0x00,
	0x00, 0x1f, 0xff, 0xe0, 0x0f, 0xff, 0xfc, 0x00, 
	0x00, 0x3f, 0xff, 0xe0, 0x0f, 0xff, 0xfc, 0x00,
	0x00, 0x7f, 0xff, 0xc1, 0x06, 0x7f, 0xfe, 0x00, 
	0x00, 0x7f, 0xff, 0x81, 0x00, 0xff, 0xfe, 0x00,
	0x00, 0xff, 0xff, 0x03, 0x81, 0xff, 0xff, 0x00, 
	0x00, 0xff, 0xfe, 0x07, 0xc3, 0xcf, 0xff, 0x00,
	0x00, 0xff, 0xfc, 0x0f, 0xc7, 0x9f, 0xff, 0x00, 
	0x00, 0xff, 0xf8, 0x0f, 0xef, 0x3f, 0xff, 0x80,
	0x01, 0xff, 0xf0, 0x3f, 0xfe, 0x1f, 0xff, 0x80, 
	0x01, 0xff, 0xc0, 0x7f, 0xfc, 0x0f, 0xff, 0x80,
	0x01, 0xff, 0x80, 0xff, 0xfc, 0x03, 0xff, 0x80, 
	0x01, 0xfe, 0x01, 0xff, 0xfe, 0x00, 0xff, 0x80,
	0x01, 0xf8, 0x03, 0xff, 0xff, 0x80, 0x3f, 0x80, 
	0x01, 0xc0, 0x0f, 0xff, 0xff, 0xc0, 0x07, 0x80,
	0x01, 0x00, 0x1f, 0xff, 0xff, 0xe0, 0x00, 0x80, 
	0x01, 0xe0, 0x0f, 0xff, 0xff, 0xc0, 0x07, 0x80,
	0x01, 0xf8, 0x03, 0xff, 0xff, 0x00, 0x3f, 0x80, 
	0x01, 0xfe, 0x01, 0xff, 0xfe, 0x01, 0xff, 0x80,
	0x01, 0xff, 0x80, 0x7f, 0xfc, 0x07, 0xff, 0x80, 
	0x01, 0xff, 0xc0, 0xff, 0xf8, 0x0f, 0xff, 0x80,
	0x00, 0xff, 0xf1, 0xff, 0xf0, 0x3f, 0xff, 0x80, 
	0x00, 0xff, 0xf3, 0xcf, 0xe0, 0x7f, 0xff, 0x00,
	0x00, 0xff, 0xe7, 0x87, 0xc0, 0xff, 0xff, 0x00, 
	0x00, 0xff, 0xef, 0x07, 0xc1, 0xff, 0xff, 0x00,
	0x00, 0x7f, 0xfe, 0x03, 0x83, 0xff, 0xfe, 0x00, 
	0x00, 0x7f, 0xfc, 0x81, 0x03, 0xff, 0xfe, 0x00,
	0x00, 0x3f, 0xfd, 0xc0, 0x07, 0xff, 0xfc, 0x00, 
	0x00, 0x1f, 0xff, 0xe0, 0x0f, 0xff, 0xfc, 0x00,
	0x00, 0x1f, 0xff, 0xe0, 0x0f, 0xff, 0xf8, 0x00, 
	0x00, 0x0f, 0xff, 0xf0, 0x1f, 0xff, 0xf0, 0x00,
	0x00, 0x07, 0xff, 0xf8, 0x1f, 0xff, 0xe0, 0x00, 
	0x00, 0x03, 0xff, 0xf8, 0x3f, 0xff, 0xe0, 0x00,
	0x00, 0x01, 0xff, 0xf8, 0x3f, 0xff, 0xc0, 0x00, 
	0x00, 0x00, 0xff, 0xfc, 0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x7f, 0xfc, 0x7f, 0xfe, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0xfe, 0x7f, 0xfc, 0x00, 0x00,
	0x00, 0x00, 0x0f, 0xfe, 0x7f, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0x03, 0xfe, 0x7f, 0xc0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'LYRA_BW', 64x32px
const unsigned char lyraText [] PROGMEM = {
	0x40, 0x20,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0xfe, 0x0f, 0x86, 0x00, 0x7f, 0xe1, 0xff, 0xe0, 0xfe, 0x0f, 0x86, 0x00, 0x1f, 0xe0, 0xff,
	0xe0, 0xfe, 0x0f, 0x86, 0x00, 0x0f, 0xc0, 0xff, 0xe0, 0xfe, 0x0f, 0x86, 0x00, 0x0f, 0xc0, 0x7f,
	0xe0, 0xff, 0x0f, 0x86, 0x1e, 0x0f, 0x80, 0x7f, 0xe0, 0xff, 0x07, 0x06, 0x1f, 0x0f, 0x80, 0x7f,
	0xe0, 0xff, 0x82, 0x0e, 0x1f, 0x0f, 0x84, 0x3f, 0xe0, 0xff, 0x80, 0x0e, 0x00, 0x0f, 0x0c, 0x3f,
	0xe0, 0xff, 0xc0, 0x1e, 0x00, 0x1f, 0x0c, 0x1f, 0xe0, 0xff, 0xe0, 0x3e, 0x00, 0x3e, 0x0e, 0x1f,
	0xe0, 0xff, 0xe0, 0x3e, 0x0c, 0x3e, 0x00, 0x0f, 0xe0, 0xff, 0xf0, 0x7e, 0x0c, 0x1e, 0x00, 0x0f,
	0xe0, 0x03, 0xf0, 0x7e, 0x0e, 0x1c, 0x00, 0x0f, 0xe0, 0x03, 0xf0, 0x7e, 0x0e, 0x0c, 0x00, 0x07,
	0xe0, 0x03, 0xf0, 0x7e, 0x0e, 0x0c, 0x3f, 0x07, 0xe0, 0x03, 0xf0, 0x7e, 0x0f, 0x00, 0x3f, 0x87,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
