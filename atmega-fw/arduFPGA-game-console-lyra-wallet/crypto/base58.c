/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Iulian Gheorghiu
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/interrupt.h>
#include "lyra_global.h"
#include "base58.h"
#include "convert/base58.h"


#include "crypto/sha256.h"

void lyraGetCheckSum(uint8_t *chk, uint8_t *data, uint16_t dataLen) {
	uint8_t hash[32];
	//cli();
	sha256(hash, data, dataLen);
	sha256(hash, hash, 32);
	//sei();
	chk[0] = hash[0];
	chk[1] = hash[1];
	chk[2] = hash[2];
	chk[3] = hash[3];
}

uint16_t lyraAddCheckSum(uint8_t *dataWithCheckSum, uint8_t *data, uint16_t dataLen) {
	uint8_t checkSum[LYRA_CHKSUM_SIZE_IN_BYTES];
	lyraGetCheckSum(checkSum, data, dataLen);
	if(dataWithCheckSum == data)
		memcpy(dataWithCheckSum + dataLen, checkSum, LYRA_CHKSUM_SIZE_IN_BYTES);
	else {
		memcpy(dataWithCheckSum, data, dataLen);
		memcpy(dataWithCheckSum + dataLen, checkSum, LYRA_CHKSUM_SIZE_IN_BYTES);
	}
	return dataLen + LYRA_CHKSUM_SIZE_IN_BYTES;
}

//Returns 0 if the checksum is invalid
uint16_t lyraVerifyAndRemoveCheckSum(uint8_t *dataWithoutCheckSum, uint8_t *data, uint16_t dataLen) {
	uint8_t checkSum[LYRA_CHKSUM_SIZE_IN_BYTES];
	lyraGetCheckSum(checkSum, data, dataLen - LYRA_CHKSUM_SIZE_IN_BYTES);
	if(memcmp(checkSum, data + dataLen - LYRA_CHKSUM_SIZE_IN_BYTES, LYRA_CHKSUM_SIZE_IN_BYTES))
		return 0;
		
	if(dataWithoutCheckSum == data)
		memset(data + dataLen - LYRA_CHKSUM_SIZE_IN_BYTES, 0, LYRA_CHKSUM_SIZE_IN_BYTES);
	else
		memcpy(dataWithoutCheckSum, data, dataLen - LYRA_CHKSUM_SIZE_IN_BYTES);
	return dataLen + LYRA_CHKSUM_SIZE_IN_BYTES;
}

uint16_t lyraEncode(uint8_t *dataEncoded, uint8_t *data, uint16_t dataLen) {
	return EncodeBase58(data, dataLen, dataEncoded, dataLen << 1);
}

uint16_t lyraEncodeWithCheckSum(uint8_t *dataWithCheckSum, uint8_t *data, uint16_t dataLen) {
	int16_t dLen = lyraAddCheckSum(dataWithCheckSum, data, dataLen);
	return EncodeBase58(dataWithCheckSum, dLen, dataWithCheckSum, dLen << 1);
}

uint16_t lyraEncodePrivateKey(uint8_t *encodedPrivateKeyData, uint8_t *privateKeyData) {
	return lyraEncodeWithCheckSum(encodedPrivateKeyData, privateKeyData, 32);
}

uint16_t lyraEncodePublicKey(uint8_t *encodedPublicKeyData, uint8_t *publicKeyData) {
	return lyraEncodeWithCheckSum(encodedPublicKeyData, publicKeyData, 64);
}

uint16_t lyraEncodeAccountId(uint8_t *encodedAccountId, uint8_t *publicKeyData) {
	encodedAccountId[0] = LYRA_ADDRESSPREFIX;
	return lyraEncodePublicKey(encodedAccountId + 1, publicKeyData) + 1;
}

// If an error eccur, will return 0
uint16_t lyraDecodeWithCheckSum(uint8_t *dataWithoutCheckSum, uint8_t *data, uint16_t dataLen) {
	int dLen = DecodeBase58(data, dataLen, dataWithoutCheckSum);
	if(!dLen)
		return 0;
		
	uint16_t retLen = lyraVerifyAndRemoveCheckSum(dataWithoutCheckSum, dataWithoutCheckSum, dLen);
	if(retLen)
		return retLen - (LYRA_CHKSUM_SIZE_IN_BYTES * 2);
	else
		return 0;
}

uint16_t lyraDecodePrivateKey(uint8_t *decodedPrivateKeyData, uint8_t *privateKeyData) {
	return lyraDecodeWithCheckSum(decodedPrivateKeyData, privateKeyData, strlen((char *)privateKeyData));
}

uint16_t lyraDecodePublicKey(uint8_t *decodedPublicKeyData, uint8_t *publicKeyData) {
	return lyraDecodeWithCheckSum(decodedPublicKeyData, publicKeyData, strlen((char *)publicKeyData));
}

uint16_t lyraDecodeAccountId(uint8_t *decodedAccountId, uint8_t *publicKeyData) {
	return lyraDecodePublicKey(decodedAccountId, publicKeyData + 1);
}

