/*
 * api.h
 *
 * Created: 25.01.2021 13:15:33
 *  Author: MorgothCreator
 */ 


#ifndef API_H_
#define API_H_

/*
	All commands are executed when the device is unlocked.
	
	"DEVICE_RESET\r" command:
	If this command is received through UART, will prompt for password, if the password match the internal emulated EEPROM is filled with 0xFF, in otder to save the new content of the internal emulated EEPROM to uSD the user need to short press INT button.
	
	"GET_PUBLIC_KEY\r" command:
	If this command is received through UART, will return the public key after about teen seconds.
	
	"SIGN\r" command:
	If this command is received through UART, the device will respond with "WAIT_PASSWORD\r", after user enter the password on the device, the device will respond with "PACK_WAIT\r",
		after this, the device can receive the package to sign as string, broken in packets of 64Bytes, at each received packet by the device will respond with "NEXT_PACK\r", the last packet need to end with "\r" or "\n".
	When all packets are received and "\r" or "\n" was detected the device will respond with "SIGN_WAIT\r" and after approximative teen seconds will respond with the encoded 58 signature string.
	If "SIGN\r" command was received and 'B' key on the device is press instead of entering the password and accepting to sign with 'A' button, the device will send "CANCELLED\r".
	
*/

typedef enum {
	UART_API_CMD_NONE = 0,
	UART_API_CMD_CLEAR_DEVICE
}uartApiCmd_e;

#define UART_API_CMD_CLEAR_DEVICE_STR			"DEVICE_RESET"
#define UART_API_CMD_RX_GET_PUBLIC_KEY			"GET_PUBLIC_KEY"
#define UART_API_CMD_SIGN_STR					"SIGN"



#endif /* API_H_ */