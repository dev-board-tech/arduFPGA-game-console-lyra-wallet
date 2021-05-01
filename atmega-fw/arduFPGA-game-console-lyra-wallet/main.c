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


#define IMPLEMENT_PUBLIC_KEY_GENERATION 0

#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>


#include "def.h"
#include "states.h"
#include "api.h"
#include "sys/sys.h"
#include "util/util.h"
#include "memory.h"
#include "gfx/draw.h"
//#include "gfx/3d.h"
#include "sys/delay.h"
#include DISPLAY_DRIVER_FILE


#include "convert/base58.h"
#include "crypto/portableSignatures.h"
#include "crypto/base58.h"
#include "crypto/aes256.h"
#include "crypto/sha256.h"
#include "gfx/logo.h"
#include "resources/logo/logo_init.h"

/* Platform dependent drivers */
#include "driver/kbd.h"
#include "driver/spi.h"
//#include "driver/twi_s.h"
#include "driver/uart.h"

uint8_t AES_IV[] ={0xBA, 0x4C, 0x51, 0xE9, 0xA7, 0xDA, 0x4B, 0xDF, 0x2C, 0x45, 0xF4, 0xF1, 0x94, 0xDB, 0xAB, 0x7F};


/* SPI variables */
spi_t spi;

/* UART variables */
char uartRxBuf[65];
uint8_t uartRxCnt = 0;
bool uartNewDataRx = false;

/* Display variables */
#ifndef SSD1306_USE_NO_BUF
uint8_t *screen_buf;
#else /* SSD1306_USE_NO_BUF */
uint8_t screen_buf[1];
#endif /* SSD1306_USE_NO_BUF */
box_t box = {0, 0, 0, 0};

/* GUI variables */
int16_t x_logo1_pos = 0;
int16_t y_logo1_pos = 0;
logo_t logo1;
int16_t x_logo2_pos = 0;
int16_t y_logo2_pos = 0;
logo_t logo2;
char *displayedStr1;
char *displayedStr2;

/* Password variables */
passChar_e password[PASSWORD_BUF_LENGTH];
uint8_t passCnt = 0;
uint8_t passChar = 0;
bool cursorState = false;
uint8_t passwordHash[32];
uint8_t passwordHashCheck[32];

/* Application state */
appState_e appState = APP_STATE_ENTRY_LOGO_INIT; // Main state: Represent the application state.
appState_e appDestinationState = APP_STATE_CHECK_INITIALIZED; // General custom state: Some states are used by different states and after completion need to go to a custom state, this state will be load to "appState".
appState_e appDestinationDecriptState = APP_STATE_WALLET_UNLOCKED_INIT; // Password match custom state: This state can be customized, if the password is the right one the application will jump to this state.

/* Device state */
deviceState_e deviceState = DEVICE_STATE_NONE;

/* EEPROM memory allocation */
eepromStructure_t *eeprom = NULL;

/* Encryption buffers */
uint8_t privateKeyBuf[65];
uint8_t privateKeyBufCheck[65];
uint8_t privateKeyCnt;
uint8_t privateKeyChar;
uint8_t buf64[64];
uint8_t buf128[128];
#define PRIVATE_KEY		"mjnQ4Zv73zkmRB6KKhm3SCv2kMjs45gWuHaW8HrFenb16FehG"
char message[] = "Oh shit";

/* AES Encryption variables */
aes256_context ctxt;

/* Package for signing */
uint8_t packForSigning[1025];
uint16_t packForSigningCnt;

/* Menu variables */
menuDevEmpty_e menuDevEmpty = MENU_DEV_EMPTY_NEW_WALLET;
privateKeyState_e privateKeyState = PRIVATE_KEY_STATE_REQUEST_SAVE;

timer_t genTimer;
timer_t lockTimer;

extern const char BASE58_ALPHABET[59];
extern const char BASE58_ALPHABET_MAP[256];

void service() {
	timer_service();
}

void screen_clear_paint_logo() {
	DISPLAY_FUNC_CLEAR(&spi, screen_buf, false);
	logo_idle(&logo1, &spi, screen_buf, &box, x_logo1_pos, y_logo1_pos);
	logo_idle(&logo2, &spi, screen_buf, &box, x_logo2_pos, y_logo2_pos);
	if(deviceState != DEVICE_STATE_NONE) {
		strcpy_P((char *)buf64, deviceState == DEVICE_STATE_LOCKED ? PSTR("LOCKED") : PSTR("UNLOCKED"));
		DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)buf64, 80, 0, false, false, false, true);
	}
	strcpy_P((char *)buf64, PSTR("NETWORK"));
	DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)buf64, x_logo2_pos + 5, y_logo2_pos + 30, false, false, false, true);
}

uint8_t signatureEncoded[128];
uint8_t signatureDecoded[68];

int main(void) {
	screen_buf = (uint8_t *)calloc(1, 1024);
	//packForSigning = (uint8_t *)calloc(1, 2048);
	//password = (passChar_e *)calloc(1, PASSWORD_BUF_LENGTH);
	//passwordHash = (uint8_t *)calloc(1, 32);
	//passwordHashCheck = (uint8_t *)calloc(1, 32);
	//buf64 = (uint8_t *)calloc(1, 64);
	//buf128 = (uint8_t *)calloc(1, 128);
	//privateKeyBuf = (uint8_t *)calloc(1, 64);
	displayedStr1 = calloc(1, 33);
	displayedStr2 = calloc(1, 33);
	
	lockTimer.value = 600000; // 10 Minutes
	timer_enable(&lockTimer, false);


/*#if IMPLEMENT_PUBLIC_KEY_GENERATION
	strcpy_P((char *)privateKeyBuf, PSTR(PRIVATE_KEY));
	
	lyraDerivePublicKey(buf128, privateKeyBuf);
	lyraEncodePublicKey(buf128, buf128);
	
	memset(signatureEncoded, 0, sizeof(signatureEncoded));

	//volatile int pKeyLen = lyraGetAccountIdFromPrivateKey(publicKey, (uint8_t *)PRIVATE_KEY);

	memset(signatureDecoded, 0, sizeof(signatureDecoded));
	
	lyraGetSignature((uint8_t *)message, strlen((char *)message), privateKeyBuf, signatureDecoded);
	lyraEncode(signatureEncoded, signatureDecoded, 64);

	//pKeyLen = lyraDecodeAccountId(pubKeyConcatenated, publicKey);
#endif
	
	//memset(buf128, 0xFF, 128);
	//eeprom_write_block(buf128, eeprom->box, sizeof(eeprom->box));
	memset(buf128, 0, 128);
	memset(privateKeyBuf, 0, 64);*/

#ifdef _AVR_IO_H_
#ifdef _AVR_IOM328P_H_
    cli();
    
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = 1 << TOIE0;
    sei();
#else
	void (*vect_set_service)(uint16_t) = (void *)BOOT_VECTOR_SET_SERVICE_VECT;
	vect_set_service((uint16_t)service);
    sei();
#endif
#endif /* !_AVR_IO_H_ */
	spi.spcr = &SPCR;
	spi.spsr = &SPSR;
	spi.spdr = &SPDR;
	spi_init(&spi);
	
	box.x_max = DISPLAY_FUNC_GET_X();
	box.y_max = DISPLAY_FUNC_GET_Y();
	DISPLAY_FUNC_INIT(&spi, screen_buf);
	DISPLAY_FUNC_SET_CONTRAST(&spi, 255);
	
	uart_init(115200);
/*#ifdef UCSR1A
	UCSR1B |= RXCIE1;
#elif  defined (UCSR0A)
	UCSR0B |= RXCIE0;
#endif*/

	kbd_init();

	/* Check if memory is empty */
	int cnt = 0;
	int var1 = 0;
	int strPos = 0;
	uint8_t chk[4];
	bool isHex = false;
	uint8_t c = 0;
    while (1) {
		if(timer_tik(&lockTimer)) {
			/* Delete all encription variables */
			memset(privateKeyBuf, 0, sizeof(privateKeyBuf));
			memset(privateKeyBufCheck, 0, sizeof(privateKeyBufCheck));
			memset(passwordHash, 0, sizeof(passwordHash));
			//memset(passwordHashCheck, 0, sizeof(passwordHashCheck));
			memset(buf64, 0, sizeof(buf64));
			memset(buf128, 0, sizeof(buf128));
			
			timer_enable(&lockTimer, false);
			deviceState = DEVICE_STATE_LOCKED;
			appState = APP_STATE_LOCK_LOGO_ROLL;
		}
		switch(appState) {
			case APP_STATE_IDLE:
				if(!uartNewDataRx && uart_get_c_nb(&c)) {
					if(c != 0x0a && c != 0x0d) {
						if(uartRxCnt < sizeof(uartRxBuf) - 1) {
							uartRxBuf[uartRxCnt++] = c;
							uartRxBuf[uartRxCnt] = 0;
						}
						if (uartRxCnt == sizeof(uartRxBuf) - 1) {
							uartNewDataRx = true;
						}
					} else {
						uartNewDataRx = true;
					}
				}

				if(uartNewDataRx) {
					if(!strcmp_P(uartRxBuf, PSTR(UART_API_CMD_CLEAR_DEVICE_STR))) {
						appState = APP_STATE_RESET_LOGO_ROLL;
						appDestinationState = APP_STATE_DECRYPT_BOX;
						appDestinationDecriptState = APP_STATE_RESET_DEVICE;
					} else if(!strcmp_P(uartRxBuf, PSTR(UART_API_CMD_SIGN_STR))) {
						memset(uartRxBuf, 0, sizeof(uartRxBuf));
						uartRxCnt = 0;
						uartNewDataRx = false;
						appState = APP_STATE_SIGN_ROLL_ENTRY;
					} else if(!strcmp_P(uartRxBuf, PSTR(UART_API_CMD_RX_GET_PUBLIC_KEY))) {
						uart_put_s_P(PSTR(UART_API_CMD_RX_GET_PUBLIC_KEY"_WAIT\r"));
						//strcpy_P((char *)privateKeyBuf, PSTR(PRIVATE_KEY));
						cli();
						lyraDerivePublicKey(buf128, privateKeyBuf);
						lyraEncodePublicKey(buf128, buf128);
						sei();
						uart_put_s((char *)buf128);
						uart_put_c('\r');
						memset(uartRxBuf, 0, sizeof(uartRxBuf));
						uartRxCnt = 0;
						uartNewDataRx = false;
					} else {
						uart_put_s_P(PSTR("NULL\r"));
						memset(uartRxBuf, 0, sizeof(uartRxBuf));
						uartRxCnt = 0;
						uartNewDataRx = false;
					}
				}
				break;
			case APP_STATE_LOCK_LOGO_ROLL:
				screen_clear_paint_logo();
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				y_logo1_pos-=2;
				y_logo2_pos-=2;
				if(y_logo1_pos < -16) {
					y_logo1_pos+=2;
					y_logo2_pos+=2;
					delay_ms(400);
					appState = APP_STATE_PASSWORD_INIT;
					appDestinationState = APP_STATE_DECRYPT_BOX;
				}
				break;
			case APP_STATE_RESET_LOGO_ROLL:
				screen_clear_paint_logo();
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				y_logo1_pos-=2;
				y_logo2_pos-=2;
				if(y_logo1_pos < -16) {
					y_logo1_pos+=2;
					y_logo2_pos+=2;
					delay_ms(400);
					strcpy_P(displayedStr1, PSTR("Enter password to clear:"));
					appState = APP_STATE_PASSWORD_INIT;
				}
				break;
			case APP_STATE_RESET_DEVICE:
				memset(buf128, 0xFF, 128);
				eeprom_write_block(buf128, eeprom->box, sizeof(eeprom->box));
				DISPLAY_FUNC_CLEAR(&spi, screen_buf, false);
				strcpy_P((char *)displayedStr2, PSTR("Device reset successfully"));
				DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 0, 22, false, false, false, true);
				strcpy_P((char *)displayedStr2, PSTR("Press INT button and reload."));
				DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 0, 34, false, false, false, true);
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				while(1);
				break;
			case APP_STATE_DECRYPT_BOX:
				eeprom_read_block(buf128, eeprom->box, sizeof(eeprom->box)); // Read the box
				util_get_bin_from_hex_buf(buf64, (char *)buf128, 64); // Convert hex to bin.
				cli();
				aes256_init(&ctxt, passwordHash); //Decrypt first 32 bytes of the box.
				aes256_decrypt_ecb(&ctxt, buf64);
				aes256_decrypt_ecb(&ctxt, buf64 + 16);
				aes256_decrypt_ecb(&ctxt, buf64 + 32);
				aes256_decrypt_ecb(&ctxt, buf64 + 48);
				aes256_done(&ctxt);
				lyraGetCheckSum(chk, buf64, 32);
				sei();
				if(memcmp(chk, buf64 + 32, 4)) {
					strcpy_P((char *)displayedStr2, PSTR("WRONG PASSWORD!"));
					DISPLAY_FUNC_CLEAR(&spi, screen_buf, false);
					DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 24, 28, false, false, false, true);
					DISPLAY_FUNC_REFRESH(&spi, screen_buf);
					while(1) {
						kbd_idle();
						if(kbd_changed()) {
							uint8_t key = kbd_get();
							if(key == KBD_A_KEY) {
								appState = APP_STATE_PASSWORD_INIT; // Wrong password.
								break;
							}
						}
					}
				} else {
					lyraEncodePrivateKey(privateKeyBuf, buf64);
					timer_enable(&lockTimer, true);
					deviceState = DEVICE_STATE_UNLOCKED;
					appState = appDestinationDecriptState;// Box was decrypted successfully.
				}
				break;
			case APP_STATE_CHECK_INITIALIZED:
				for (cnt = 0; cnt < 128; cnt++) {
					isHex = util_is_hex(eeprom_read_byte(eeprom->box + cnt));
					if(!isHex) {
						//appState = APP_STATE_INITIALIZATION_PASSWORD_INIT; // Memory empty or corrupted.
						appState = APP_STATE_CHOOSE_MENU_INIT; // Memory empty or corrupted.
						break;
					}
				}
				if(isHex) {
					strcpy_P(displayedStr1, PSTR("Enter password:"));
					appState = APP_STATE_PASSWORD_INIT; // Entire box has hex characters, go to password entry.
					appDestinationState = APP_STATE_DECRYPT_BOX; // After password successfully entered go to decrypt the box.
					appDestinationDecriptState = APP_STATE_WALLET_UNLOCKED_INIT; // After box successfully decrypted, where to land.
				}
				break;
			case APP_STATE_CHOOSE_MENU_INIT:
				menuDevEmpty = MENU_DEV_EMPTY_NEW_WALLET;
				appState = APP_STATE_CHOOSE_MENU_LOOP;
				break;
			case APP_STATE_CHOOSE_MENU_LOOP:
				kbd_idle();
				if(kbd_changed()) {
					uint8_t key = kbd_get();
					if(key == KBD_U_KEY && menuDevEmpty == MENU_DEV_EMPTY_RECOVER_WALLET) {
						menuDevEmpty = MENU_DEV_EMPTY_NEW_WALLET;
					} else if(key == KBD_D_KEY && menuDevEmpty == MENU_DEV_EMPTY_NEW_WALLET) {
						menuDevEmpty = MENU_DEV_EMPTY_RECOVER_WALLET;
					} else if(key == KBD_A_KEY) {
						appState = APP_STATE_INITIALIZATION_PASSWORD_INIT;
					} else if(key == KBD_B_KEY && menuDevEmpty == MENU_DEV_EMPTY_NEW_WALLET) {
						appState = APP_STATE_ENTRY_LOGO_INIT;
					}
				}
				screen_clear_paint_logo();
				strcpy_P(displayedStr1, PSTR("1) Create new wallet"));
				DISPLAY_FUNC_DRAW_RECTANGLE(&spi, &box, screen_buf, 0, 48, DISPLAY_FUNC_GET_X(), 8, true, (menuDevEmpty == MENU_DEV_EMPTY_NEW_WALLET ? true : false));
				DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, displayedStr1, 0, 48, false, false, (menuDevEmpty == MENU_DEV_EMPTY_NEW_WALLET ? true : false), (menuDevEmpty == MENU_DEV_EMPTY_NEW_WALLET ? false : true));
				strcpy_P(displayedStr1, PSTR("2) Recover from private key"));
				DISPLAY_FUNC_DRAW_RECTANGLE(&spi, &box, screen_buf, 0, 56, DISPLAY_FUNC_GET_X(), 8, true, (menuDevEmpty == MENU_DEV_EMPTY_RECOVER_WALLET ? true : false));
				DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, displayedStr1, 0, 56, false, false, (menuDevEmpty == MENU_DEV_EMPTY_RECOVER_WALLET ? true : false), (menuDevEmpty == MENU_DEV_EMPTY_RECOVER_WALLET ? false : true));
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				break;
			case APP_STATE_INITIALIZATION_PASSWORD_INIT:
				strcpy_P(displayedStr1, PSTR("Enter pass for new wallet:"));
				appState = APP_STATE_PASSWORD_INIT; // Go to password entry.
				appDestinationState = APP_STATE_INITIALIZATION_PASSWORD_CHECK_INIT; // After password successfully entered go to check the entered password.
				break;
			case APP_STATE_INITIALIZATION_PASSWORD_CHECK_INIT:
				if(strlen((char *)password) < MIN_PASSWORD_LENGTH) {
					DISPLAY_FUNC_CLEAR(&spi, screen_buf, false);
					strcpy_P((char *)displayedStr2, PSTR("Password too short."));
					DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 0, 28, false, false, false, true);
					DISPLAY_FUNC_REFRESH(&spi, screen_buf);
					while(1) {
						kbd_idle();
						if(kbd_changed()) {
							uint8_t key = kbd_get();
							if(key == KBD_A_KEY) {
								appState = APP_STATE_INITIALIZATION_PASSWORD_INIT;
								break;
							}
						}
					}
				} else {
					memcpy(buf128, passwordHash, 32); // Save the first entered password in a secondary buffer.
					strcpy_P(displayedStr1, PSTR("Reenter pass for new wallet:"));
					appState = APP_STATE_PASSWORD_INIT; // Go to password entry.
					appDestinationState = APP_STATE_INITIALIZATION_PASSWORD_CHECK; // After password successfully entered go and compare the two passwords.
				}
				break;
			case APP_STATE_INITIALIZATION_PASSWORD_CHECK:
				if(memcmp(buf128, passwordHash, 32)) {
					appState = APP_STATE_INITIALIZATION_PASSWORD_FAIL; // Password check fail, return to enter password.
				} else {
					privateKeyState = PRIVATE_KEY_STATE_REQUEST_SAVE;
					appState = APP_STATE_INITIALIZATION_PRIVATE_INIT;
				}
				break;
			case APP_STATE_INITIALIZATION_PASSWORD_FAIL:
				DISPLAY_FUNC_CLEAR(&spi, screen_buf, false);
				strcpy_P((char *)displayedStr2, PSTR("Passwords mismatch."));
				DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 0, 28, false, false, false, true);
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				while(1) {
					kbd_idle();
					if(kbd_changed()) {
						uint8_t key = kbd_get();
						if(key == KBD_A_KEY) {
							appState = APP_STATE_INITIALIZATION_PASSWORD_INIT;
							break;
						}
					}
				}
				break;
			case APP_STATE_INITIALIZATION_PRIVATE_INIT:
				if(menuDevEmpty == MENU_DEV_EMPTY_RECOVER_WALLET) {
					memset(privateKeyBuf, 0, 64);
					//strcpy_P((char *)privateKeyBuf, PSTR("mjnQ4Zv73zkmRB6KKhm3SCv2kMjs45gWuHaW8HrFenb16FehG")); // dummy key for debug.
					strcpy_P(displayedStr1, PSTR("Enter private key:"));
				} else if(privateKeyState == PRIVATE_KEY_STATE_REQUEST_SAVE) { // generate private key
					/* TODO! */
					memset(buf64, 0, 32);
					RNG256_CFG = 0;
					for (cnt = 0; cnt < 32; cnt++) {
						buf64[cnt] = RNG256_DAT;
					}
					RNG256_CFG = 1;
					lyraEncodePrivateKey(privateKeyBuf, buf64);
					strcpy_P(displayedStr1, PSTR("Save private key on a paper"));
				} else if(privateKeyState == PRIVATE_KEY_STATE_CHECK) {
					memset(privateKeyBuf, 0, 64);
					strcpy_P(displayedStr1, PSTR("Enter private key to check"));
				}
				privateKeyCnt = 0;
				privateKeyChar = pgm_read_byte(&BASE58_ALPHABET_MAP[privateKeyBuf[privateKeyCnt]]) == -1 ? 0 : (pgm_read_byte(&BASE58_ALPHABET_MAP[privateKeyBuf[privateKeyCnt]]) + 1);
				timer_enable(&genTimer, true);
				strPos = 0;
				appState = APP_STATE_INITIALIZATION_PRIVATE_HANDLE;
				break;
			case APP_STATE_INITIALIZATION_PRIVATE_HANDLE:
				if(privateKeyCnt < 64) {
					if(timer_tik(&genTimer)) {
						privateKeyBuf[privateKeyCnt] = cursorState ? '_' : (privateKeyChar ? pgm_read_byte(&BASE58_ALPHABET[privateKeyChar - 1]) : 0);
						cursorState = !cursorState;
					}
				} else {
					privateKeyBuf[privateKeyCnt] = 0;
				}
				kbd_idle();
				if(kbd_changed()) {
					uint8_t key = kbd_get();
					if(key == KBD_U_KEY && (menuDevEmpty == MENU_DEV_EMPTY_RECOVER_WALLET || privateKeyState == PRIVATE_KEY_STATE_CHECK)) { // Key editing available only on wallet recovery.
						if(privateKeyCnt < 64) {
							privateKeyChar++;
							if(privateKeyChar > sizeof(BASE58_ALPHABET))
								privateKeyChar = 1;
							if(privateKeyChar < 1)
								privateKeyChar = 1;
						}
						cursorState = false;
					} else if(key == KBD_D_KEY && (menuDevEmpty == MENU_DEV_EMPTY_RECOVER_WALLET || privateKeyState == PRIVATE_KEY_STATE_CHECK)) { // Key editing available only on wallet recovery.
						if(privateKeyCnt < 64) {
							if(privateKeyChar < 1 || privateKeyChar > sizeof(BASE58_ALPHABET))
								privateKeyChar = 1;
							privateKeyChar--;
							if(privateKeyChar < 1)
								privateKeyChar = sizeof(BASE58_ALPHABET);
						}
						cursorState = false;
					} else if(key == KBD_R_KEY) {
						privateKeyBuf[privateKeyCnt] = (privateKeyChar ? pgm_read_byte(&BASE58_ALPHABET[privateKeyChar - 1]) : 0);
						if(privateKeyCnt < 64 && privateKeyBuf[privateKeyCnt] != 0) {
							privateKeyCnt++;
							if((privateKeyCnt * 6) + strPos > DISPLAY_FUNC_GET_X() - 6)
								strPos -= 6;
						}
						privateKeyChar = pgm_read_byte(&BASE58_ALPHABET_MAP[privateKeyBuf[privateKeyCnt]]) == -1 ? 0 : (pgm_read_byte(&BASE58_ALPHABET_MAP[privateKeyBuf[privateKeyCnt]]) + 1);
						cursorState = true;
					} else if(key == KBD_L_KEY) {
						privateKeyBuf[privateKeyCnt] = (privateKeyChar ? pgm_read_byte(&BASE58_ALPHABET[privateKeyChar - 1]) : 0);
						if(privateKeyCnt != 0) {
							privateKeyCnt--;
							if((privateKeyCnt * 6) + strPos < 0)
								strPos += 6;
						}
						privateKeyChar = pgm_read_byte(&BASE58_ALPHABET_MAP[privateKeyBuf[privateKeyCnt]]) == -1 ? 0 : (pgm_read_byte(&BASE58_ALPHABET_MAP[privateKeyBuf[privateKeyCnt]]) + 1);
						cursorState = true;
					} else if(key == KBD_A_KEY) {
						privateKeyBuf[privateKeyCnt] = (privateKeyChar ? pgm_read_byte(&BASE58_ALPHABET[privateKeyChar - 1]) : 0);
						appState = APP_STATE_INITIALIZATION_PRIVATE_CHECK;
						break;
					} else if(key == KBD_B_KEY) {
						appState = APP_STATE_ENTRY_LOGO_INIT;
						appDestinationState = APP_STATE_CHECK_INITIALIZED;
					}
					privateKeyBuf[privateKeyCnt] = cursorState ? '_' : (privateKeyChar ? pgm_read_byte(&BASE58_ALPHABET[privateKeyChar - 1]) : 0);
					timer_enable(&genTimer, true);
				}

				screen_clear_paint_logo();
				DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, displayedStr1, 0, 48, false, false, false, true);
				DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)privateKeyBuf, strPos, 56, true, false, false, true);
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				break;
			case APP_STATE_INITIALIZATION_PRIVATE_CHECK:
				DISPLAY_FUNC_DRAW_RECTANGLE(&spi, &box, screen_buf, 0, 56, DISPLAY_FUNC_GET_X(), 8, true, false);
				strcpy_P((char *)displayedStr2, PSTR("Checking."));
				DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 40, 56, false, false, false, true);
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				memset(buf64, 0, 64);
				var1 = lyraDecodePrivateKey(buf64, privateKeyBuf);
				if(var1 != 32) {
					DISPLAY_FUNC_DRAW_RECTANGLE(&spi, &box, screen_buf, 0, 56, DISPLAY_FUNC_GET_X(), 8, true, false);
					strcpy_P((char *)displayedStr2, PSTR("Invalid key."));
					DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 32, 56, false, false, false, true);
					DISPLAY_FUNC_REFRESH(&spi, screen_buf);
					while(1) {
						kbd_idle();
						if(kbd_changed()) {
							uint8_t key = kbd_get();
							if(key == KBD_A_KEY) {
								appState = APP_STATE_INITIALIZATION_PRIVATE_HANDLE;
								break;
							}
						}
					}
				} else {
					appState = APP_STATE_INITIALIZATION_PRIVATE_STORE;
				}
				break;
			case APP_STATE_INITIALIZATION_PRIVATE_STORE:
				if(menuDevEmpty == MENU_DEV_EMPTY_RECOVER_WALLET || privateKeyState == PRIVATE_KEY_STATE_CHECK) {
					if(memcmp(privateKeyBufCheck, privateKeyBuf, sizeof(privateKeyBufCheck)) && menuDevEmpty != MENU_DEV_EMPTY_RECOVER_WALLET) {
						DISPLAY_FUNC_DRAW_RECTANGLE(&spi, &box, screen_buf, 0, 56, DISPLAY_FUNC_GET_X(), 8, true, false);
						strcpy_P((char *)displayedStr2, PSTR("Key does not match."));
						DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 20, 56, false, false, false, true);
						DISPLAY_FUNC_REFRESH(&spi, screen_buf);
						while(1) {
							kbd_idle();
							if(kbd_changed()) {
								uint8_t key = kbd_get();
								if(key == KBD_A_KEY) {
									appState = APP_STATE_INITIALIZATION_PRIVATE_HANDLE;
									break;
								}
							}
						}
					} else {
						cli();
						lyraGetCheckSum(chk, buf64, 32);
						memcpy(buf64 + 32, chk, 4);
						aes256_init(&ctxt, passwordHash); //Encrypt first 32 bytes of the box.
						aes256_encrypt_ecb(&ctxt, buf64);
						aes256_encrypt_ecb(&ctxt, buf64 + 16);
						aes256_encrypt_ecb(&ctxt, buf64 + 32);
						aes256_encrypt_ecb(&ctxt, buf64 + 48);
						aes256_done(&ctxt);
						util_get_hex_buf((char *)buf128, buf64, 64);
						eeprom_write_block(buf128, eeprom->box, sizeof(eeprom->box));
						DISPLAY_FUNC_CLEAR(&spi, screen_buf, false);
						strcpy_P((char *)displayedStr2, PSTR("Wallet saved!"));
						DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 0, 24, false, false, false, true);
						strcpy_P((char *)displayedStr2, PSTR("Press INT button and reload."));
						DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 0, 32, false, false, false, true);
						DISPLAY_FUNC_REFRESH(&spi, screen_buf);
						sei();
						while(1);
					}
				} else {
					memcpy(privateKeyBufCheck, privateKeyBuf, sizeof(privateKeyBufCheck));
					privateKeyState = PRIVATE_KEY_STATE_CHECK;
					appState = APP_STATE_INITIALIZATION_PRIVATE_INIT;
				}
				break;
			case APP_STATE_ENTRY_LOGO_INIT:
				deviceState = DEVICE_STATE_NONE;
				logo_init(&logo1, logoInit_Black, false);
				logo_init(&logo2, lyraText, true);
				x_logo1_pos = -64;
				y_logo1_pos = 0;
				x_logo2_pos = 128;
				y_logo2_pos = 16;
				appState = APP_STATE_ENTRY_LOGO_ROLL_1;
				break;
			case APP_STATE_ENTRY_LOGO_ROLL_1:
				screen_clear_paint_logo();
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				x_logo1_pos+=2;
				x_logo2_pos-=2;
				if(x_logo1_pos > 0) {
					x_logo1_pos-=2;
					x_logo2_pos+=2;
					delay_ms(500);
					appState = APP_STATE_ENTRY_LOGO_ROLL_2;
				}
				break;
			case APP_STATE_ENTRY_LOGO_ROLL_2:
				screen_clear_paint_logo();
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				y_logo1_pos-=2;
				y_logo2_pos-=2;
				if(y_logo1_pos < -16) {
					y_logo1_pos+=2;
					y_logo2_pos+=2;
					delay_ms(400);
					appState = appDestinationState;
				}
				break;
			case APP_STATE_PASSWORD_INIT:
				memset(password, 0, PASSWORD_BUF_LENGTH);
				memset(passwordHash, 0, 32);
				DISPLAY_FUNC_CLEAR(&spi, screen_buf, false);
				genTimer.value = 250;
				timer_enable(&genTimer, true);
				cursorState = false;
				passCnt = 0;
				passChar = PASS_END;
				appState = APP_STATE_PASSWORD_HANDLE;
				break;
			case APP_STATE_PASSWORD_HANDLE:
				if(passCnt < MAX_PASSWORD_LENGTH) {
					if(timer_tik(&genTimer)) {
						password[passCnt] = cursorState ? '_' : passChar;
						cursorState = !cursorState;
					}
				} else {
					password[passCnt] = PASS_END;
				}
				kbd_idle();
				if(kbd_changed()) {
					uint8_t key = kbd_get();
					if(key == KBD_U_KEY) {
						if(passCnt < MAX_PASSWORD_LENGTH) {
							passChar++;
							if(passChar > PASS_CHAR_9)
								passChar = PASS_CHAR_0;
							if(passChar < PASS_CHAR_0)
								passChar = PASS_CHAR_0;
						}
					} else if(key == KBD_D_KEY) {
						if(passCnt < MAX_PASSWORD_LENGTH) {
							if(passChar < PASS_CHAR_0)
								passChar = PASS_CHAR_0;
							passChar--;
							if(passChar < PASS_CHAR_0)
								passChar = PASS_CHAR_9;
						}
					} else if(key == KBD_R_KEY) {
						password[passCnt] = passChar;
						passChar = PASS_END;
						if(passCnt < MAX_PASSWORD_LENGTH && password[passCnt] >= PASS_CHAR_0)
							passCnt++;
					} else if(key == KBD_L_KEY) {
						password[passCnt] = PASS_END;
						if(passCnt != 0) {
							passCnt--;
						}
						passChar = password[passCnt];
					} else if(key == KBD_A_KEY) {
						appState = APP_STATE_PASSWORD_FINISH;
					} else if(key == KBD_B_KEY) {
						if(appDestinationState == APP_STATE_SIGN_ROLL_EXIT) {
							uart_put_s_P(PSTR("CANCELLED\r"));
							appState = APP_STATE_WALLET_UNLOCKED_ROLL;
						} else {
							appState = APP_STATE_PASSWORD_INIT;
						}
					}
					password[passCnt] = passChar;
					cursorState = false;
					timer_enable(&genTimer, true);
				}
				screen_clear_paint_logo();
				DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, displayedStr1, 0, 48, false, false, false, true);
				DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)password, 0, 56, false, false, false, true);
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				break;
			case APP_STATE_PASSWORD_FINISH:
				cli();
				sha256(passwordHash, password, MAX_PASSWORD_LENGTH);
				sha256(passwordHash, passwordHash, 32);
				sei();
				appState = appDestinationState;
				break;
			case APP_STATE_WALLET_UNLOCKED_INIT:
				y_logo1_pos = -16;
				y_logo2_pos = 0;
				appState = APP_STATE_WALLET_UNLOCKED_ROLL;
				break;
			case APP_STATE_WALLET_UNLOCKED_ROLL:
				if(y_logo1_pos != 0) {
					y_logo1_pos+=2;
					y_logo2_pos+=2;
				} else {
					appState = APP_STATE_IDLE;
				}
				screen_clear_paint_logo();
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				break;
			case APP_STATE_SIGN_ROLL_ENTRY:
				screen_clear_paint_logo();
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				y_logo1_pos-=2;
				y_logo2_pos-=2;
				if(y_logo1_pos < -16) {
					y_logo1_pos+=2;
					y_logo2_pos+=2;
					uart_put_s_P(PSTR("WAIT_PASSWORD\r"));
					delay_ms(400);
					appDestinationState = APP_STATE_SIGN_ROLL_EXIT;
					appState = APP_STATE_PASSWORD_INIT;
				}
				break;
			case APP_STATE_SIGN_ROLL_EXIT:
				if(y_logo1_pos != 0) {
					y_logo1_pos+=2;
					y_logo2_pos+=2;
					} else {
					appState = APP_STATE_UART_RX_PACKET_TO_SIGN_INIT;
				}
				screen_clear_paint_logo();
				DISPLAY_FUNC_REFRESH(&spi, screen_buf);
				break;
			case APP_STATE_UART_RX_PACKET_TO_SIGN_INIT:
				uart_put_s_P(PSTR("PACK_WAIT\r"));
				memset(uartRxBuf, 0, sizeof(uartRxBuf));
				memset(packForSigning, 0, sizeof(packForSigning));
				uartRxCnt = 0;
				packForSigningCnt = 0;
				uartNewDataRx = false;
				appState = APP_STATE_UART_RX_PACKET_TO_SIGN;
				break;
			case APP_STATE_UART_RX_PACKET_TO_SIGN:
				if(!uartNewDataRx && uart_get_c_nb(&c)) {
					if(c == 0x0a || c == 0x0d) {
							uartNewDataRx = true;
					} else {
						if(packForSigningCnt < sizeof(packForSigning) - 1) {
							packForSigning[packForSigningCnt++] = c;
							packForSigning[packForSigningCnt] = 0;
						}
						if (packForSigningCnt == sizeof(uartRxBuf) - 1) {
							uart_put_s_P(PSTR("NEXT_PACK\r"));
						}
					}
				}
				if(uartNewDataRx) {
					DISPLAY_FUNC_DRAW_RECTANGLE(&spi, &box, screen_buf, 0, 56, DISPLAY_FUNC_GET_X(), 8, true, false);
					strcpy_P((char *)displayedStr2, PSTR("Signing transaction."));
					DISPLAY_FUNC_DRAW_STRING(&spi, &box, screen_buf, (char *)displayedStr2, 18, 56, false, false, false, true);
					uart_put_s((char *)packForSigning);
					uart_put_c('\r');
					uart_put_s_P(PSTR(UART_API_CMD_SIGN_STR"_WAIT\r"));
					memset(buf128, 0, sizeof(buf128));
					cli();
					lyraGetSignature((uint8_t *)packForSigning, strlen((char *)packForSigning), privateKeyBuf, buf128);
					lyraEncode(signatureEncoded, buf128, 64);
					sei();
					uart_put_s((char *)signatureEncoded);
					uart_put_c('\r');
					memset(uartRxBuf, 0, sizeof(uartRxBuf));
					memset(packForSigning, 0, sizeof(packForSigning));
					uartRxCnt = 0;
					packForSigningCnt = 0;
					uartNewDataRx = false;
					appState = APP_STATE_WALLET_UNLOCKED_ROLL;
				}
				break;
		}
    }
}

/* Interrupt vector in case of ATmega328P usage, for soft timers. */
#ifdef _AVR_IOM328P_H_
ISR(TIMER0_OVF_vect) {
	timer_service();
}
#endif

/* Interrupt vector for UART RX in case of interrupts usage. */
/*#ifdef UCSR1A
ISR(USART1_RX_vect) {
	uint8_t c = UDR1;
#elif  defined (UCSR0A)
ISR(USART0_RX_vect) {
	uint8_t c = UDR0;
#endif
	if(!uartNewDataRx) {
		if(uartRxCnt < sizeof(uartRxBuf) - 1) {
			uartRxBuf[uartRxCnt++] = c;
			uartRxBuf[uartRxCnt] = 0;
		}
		if (uartRxCnt == sizeof(uartRxBuf) - 1) {
			uartNewDataRx = true;
		}
	}
}*/
