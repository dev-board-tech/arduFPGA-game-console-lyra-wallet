# arduFPGA game console LYRA network wallet

### Private key encryption convention:

The private key is stored in a fixed size box of 64 Bytes encoded in HEX format in internal emulated EEPROM, the box is composed of four independent 16 Bytes blocks AES256 ECB encrypted, the box is AES256 encrypted with double SHA256 hash of the device password, inside the encrypted box is stored the private key ( first and second 16 Bytes block's ) and a checksum composed of first 4 bytes of double SHA256 hash of the private key ( first 4 bytes of the third 16 Bytes block ), the box is saved in internal emulated EEPROM and is 128 HEX chars long ( 64 bytes ).

If a change in the internal emulated EEPROM has happened and the INT button is press, the internal emulated EEPROM is saved to uSD as a .eep file with the same name of the application name and in the same directory as the application.

The save of the internal EEPROM memory is not part of the wallet application, is part of the first stage bootloader and the GUI explorer installed on the device and is automated using INT button, the same is with the load of the content of .eep file to the internal EEPROM memory, In order to save the changes made in the wallet, user need to press INT button to exit the wallet application and will be automatically redirected to GUI explorer.

The box has no checksum, instead the wallet application will check the private key against the private key checksum to see if the password is the right one.

The device can store as many wallets as the uSD can store, each wallet directory needing to have a copy of the wallet application.
