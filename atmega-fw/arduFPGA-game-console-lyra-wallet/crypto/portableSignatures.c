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

#include "portableSignatures.h"

#include "lyra_global.h"
#include "crypto/base58.h"
#if( USED_ECC_LIB == USE_NANO_ECC_LYB )
#include "ecc/ecc.h"
#elif( USED_ECC_LIB == USE_EASY_ECC_LYB )
#include "ecc/ecc.h"
#endif

#include "crypto/sha256.h"

EccPoint test_pub;

bool lyraValidateAccountId(uint8_t *accountId) {
	if(accountId[0] != LYRA_ADDRESSPREFIX)
	return false;
	uint8_t decodedAccountId[69];
	if(lyraDecodeAccountId(decodedAccountId, accountId))
		return true;
	else
		return false;
}

bool lyraValidateKey(uint8_t *key) {
	uint8_t decodedKey[69];
	if(lyraDecodeAccountId(decodedKey, key))
		return true;
	else
		return false;
}

bool lyraValidatePublicKey(uint8_t *key) {
	return lyraValidateKey(key);
}

bool lyraValidatePrivateKey(uint8_t *key) {
	return lyraValidateKey(key);
}

bool lyraVerifyAccountSignature(uint8_t *message, uint16_t messageLen, uint8_t *accountId, uint8_t *signature) {
	if(!message || !messageLen || !accountId || !signature)
		return false;
	uint8_t decodedAccountId[69];
	if(! lyraDecodeAccountId(decodedAccountId, accountId))
		return false;
	return lyraVerifySignature(message, messageLen, decodedAccountId, signature);
}

bool lyraVerifyAuthorizerSignature(uint8_t *message, uint16_t messageLen, uint8_t *publicKey, uint8_t *signature) {
	if(!message || !messageLen || !publicKey || !signature)
		return false;
	uint8_t decodedPublicKey[68];
	if(!lyraDecodePublicKey(decodedPublicKey, publicKey))
		return false;
	return lyraVerifySignature(message, messageLen, decodedPublicKey, signature);
}

bool lyraVerifySignature(uint8_t *message, uint16_t messageLen, uint8_t *decodedKey, uint8_t *signature) {
	/* TODO */
	return false;
}

bool lyraGetSignature(uint8_t *message, uint16_t messageLen, uint8_t *privateKey, uint8_t *signature) {
	uint8_t decodedPrivateKey[36];
	uint8_t hash[32];
	if(!lyraDecodePrivateKey(decodedPrivateKey, privateKey))
		return false;
	sha256(hash, message, messageLen);
	ecdsa_sign(decodedPrivateKey, hash, signature);
	return true;
}

uint16_t lyraDerivePublicKey(uint8_t *decodedPublicKey, uint8_t *privateKey) {
	int privKeyDecodedLen = lyraDecodePrivateKey(decodedPublicKey, privateKey);
	if(!privKeyDecodedLen)
		return 0;
#if( USED_ECC_LIB == USE_NANO_ECC_LYB )
	ecc_make_key(&test_pub, decodedPublicKey, decodedPublicKey);
	memcpy(decodedPublicKey, test_pub.x, 32);
	memcpy(decodedPublicKey + 32, test_pub.y, 32);
#elif( USED_ECC_LIB == USE_EASY_ECC_LYB )
	uint8_t p_publicKey[ECC_BYTES * 2];
	ecc_make_key(decodedPublicKey, decodedPublicKey);
#endif
	return 64;
}

uint16_t lyraGetAccountIdFromPrivateKey(uint8_t *encodedPublicKey, uint8_t *privateKey) {
	uint8_t publicKeyDec[64];
	if(!lyraDerivePublicKey(publicKeyDec, privateKey))
		return 0;
	return lyraEncodeAccountId(encodedPublicKey, publicKeyDec);
}

uint16_t lyraGeneratePrivateKey(uint8_t *encodedPrivateKey) {
	uint8_t decodedPrivateKey[36];
	/* TODO */
	return lyraEncodePrivateKey(encodedPrivateKey, decodedPrivateKey);
}

