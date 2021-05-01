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

#ifndef LYRA_BASE58_H_
#define LYRA_BASE58_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void lyraGetCheckSum(uint8_t *chk, uint8_t *data, uint16_t dataLen);
uint16_t lyraAddCheckSum(uint8_t *dataWithCheck, uint8_t *data, uint16_t dataLen);
uint16_t lyraVerifyAndRemoveCheckSum(uint8_t *dataWithoutCheckSum, uint8_t *data, uint16_t dataLen);
uint16_t lyraEncode(uint8_t *dataEncoded, uint8_t *data, uint16_t dataLen);
uint16_t lyraEncodeWithCheckSum(uint8_t *dataWithCheckSum, uint8_t *data, uint16_t dataLen);
uint16_t lyraEncodePrivateKey(uint8_t *encodedPrivateKeyData, uint8_t *privateKeyData);
uint16_t lyraEncodePublicKey(uint8_t *encodedPublicKeyData, uint8_t *publicKeyData);
uint16_t lyraEncodeAccountId(uint8_t *encodedAccountId, uint8_t *publicKeyData);
uint16_t lyraDecodeWithCheckSum(uint8_t *dataWithoutCheckSum, uint8_t *data, uint16_t dataLen);
uint16_t lyraDecodePrivateKey(uint8_t *decodedPrivateKeyData, uint8_t *privateKeyData);
uint16_t lyraDecodePublicKey(uint8_t *decodedPublicKeyData, uint8_t *publicKeyData);
uint16_t lyraDecodeAccountId(uint8_t *decodedAccountId, uint8_t *publicKeyData);



#endif /* LYRA_BASE58_H_ */