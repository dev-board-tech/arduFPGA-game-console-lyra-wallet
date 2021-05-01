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


#include "logo.h"

#include "def.h"
#include "sys/sys.h"
#include "sys/delay.h"

#include DISPLAY_DRIVER_FILE

#if __AVR_MEGA__
#include <avr/pgmspace.h>
static const unsigned char MASK_TABLE [] PROGMEM = {
	0b00000001,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000
};
#endif

inline static bool getPixelFromBitmap(logo_t *logoInst, int16_t x ,int16_t y) {
	if(pgm_read_byte(&logoInst->logo[2 + (x >> 3) + (y * (pgm_read_byte(&logoInst->logo[0]) >> 3))]) & pgm_read_byte(&MASK_TABLE[(7 - x) & 0x07]))
		return !logoInst->negative;
	else
		return logoInst->negative;
}

void logo_init(logo_t *logoInst, const uint8_t *logo, bool negative) {
	logoInst->logo = (uint8_t *)logo;
	logoInst->negative = negative;
}

void logo_idle(logo_t *logoInst, spi_t *inst, uint8_t *buf, box_t *box, int16_t x_pos ,int16_t y_pos) {
	for (int16_t y = 0; y < (int16_t)pgm_read_byte(&logoInst->logo[1]); y++) {
		for (int16_t x = 0; x < (int16_t)pgm_read_byte(&logoInst->logo[0]); x++) {
			DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, x + x_pos, y + y_pos, getPixelFromBitmap(logoInst, x, y));
		}
	}
}

