/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Iulian Gheorghiu (morgoth@devboard.tech)
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

#ifndef PORTABLESIGNATURES_H_
#define PORTABLESIGNATURES_H_

#include <stdbool.h>
#include <stdint.h>

bool lyraValidateAccountId(uint8_t *accountId);
bool lyraValidateKey(uint8_t *key);
bool lyraValidatePublicKey(uint8_t *key);
bool lyraValidatePrivateKey(uint8_t *key);
bool lyraVerifyAccountSignature(uint8_t *message, uint16_t messageLen, uint8_t *accountId, uint8_t *signature);
bool lyraVerifyAuthorizerSignature(uint8_t *message, uint16_t messageLen, uint8_t *publicKey, uint8_t *signature);
bool lyraVerifySignature(uint8_t *message, uint16_t messageLen, uint8_t *decodedKey, uint8_t *signature);
bool lyraGetSignature(uint8_t *message, uint16_t messageLen, uint8_t *privateKey, uint8_t *signature);

uint16_t lyraDerivePublicKey(uint8_t *decodedPublicKey, uint8_t *privateKey);
uint16_t lyraGetAccountIdFromPrivateKey(uint8_t *encodedPublicKey, uint8_t *privateKey);



#endif /* PORTABLESIGNATURES_H_ */