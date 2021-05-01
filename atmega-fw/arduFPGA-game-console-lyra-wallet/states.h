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


#ifndef __STATES_H__
#define __STATES_H__

typedef enum {
	APP_STATE_IDLE = 0,
	APP_STATE_OPERATION_CANCELLED = 0,
	APP_STATE_RESET_LOGO_ROLL,
	APP_STATE_RESET_DEVICE,
	APP_STATE_DECRYPT_BOX,
	APP_STATE_CHECK_INITIALIZED,
	APP_STATE_CHOOSE_MENU_INIT,
	APP_STATE_CHOOSE_MENU_LOOP,
	APP_STATE_INITIALIZATION_PASSWORD_INIT,
	APP_STATE_INITIALIZATION_PASSWORD_CHECK_INIT,
	APP_STATE_INITIALIZATION_PASSWORD_CHECK,
	APP_STATE_INITIALIZATION_PASSWORD_FAIL,
	APP_STATE_INITIALIZATION_PRIVATE_INIT,
	APP_STATE_INITIALIZATION_PRIVATE_HANDLE,
	APP_STATE_INITIALIZATION_PRIVATE_CHECK,
	APP_STATE_INITIALIZATION_PRIVATE_STORE,
	APP_STATE_ENTRY_LOGO_INIT,
	APP_STATE_ENTRY_LOGO_ROLL_1,
	APP_STATE_ENTRY_LOGO_ROLL_2,
	APP_STATE_PASSWORD_INIT,
	APP_STATE_PASSWORD_HANDLE,
	APP_STATE_PASSWORD_FINISH,
	APP_STATE_WALLET_UNLOCKED_INIT,
	APP_STATE_WALLET_UNLOCKED_ROLL,
	APP_STATE_UART_RX_PACKET_TO_SIGN_INIT,
	APP_STATE_UART_RX_PACKET_TO_SIGN,
	APP_STATE_SIGN_ROLL_ENTRY,
	APP_STATE_SIGN_ROLL_EXIT,
	APP_STATE_LOCK_LOGO_ROLL
}appState_e;

typedef enum {
	DEVICE_STATE_NONE = 0,
	DEVICE_STATE_LOCKED,
	DEVICE_STATE_UNLOCKED
}deviceState_e;

typedef enum {
	PASSWORD_STATE_NONE = 0,
	PASSWORD_STATE_OK,
	PASSWORD_STATE_CANCEL,
	PASSWORD_STATE_WRONG,
}passwordState_e;

typedef enum {
	PASS_END = 0,
	PASS_CHAR_0 = '0',
	PASS_CHAR_1,
	PASS_CHAR_2,
	PASS_CHAR_3,
	PASS_CHAR_4,
	PASS_CHAR_5,
	PASS_CHAR_6,
	PASS_CHAR_7,
	PASS_CHAR_8,
	PASS_CHAR_9,
}passChar_e;

typedef enum {
	MENU_DEV_EMPTY_NEW_WALLET = 0,
	MENU_DEV_EMPTY_RECOVER_WALLET,
}menuDevEmpty_e;

typedef enum {
	PRIVATE_KEY_STATE_REQUEST_SAVE = 0,
	PRIVATE_KEY_STATE_CHECK
}privateKeyState_e;



#endif /* ___STATE_H__ */