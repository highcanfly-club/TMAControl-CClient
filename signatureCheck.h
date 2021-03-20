//
//  signatureCheck.h
//  TMAControl-CClient
//
//  Created by Ronan LE MEILLAT on 18/03/2021.
//

#ifndef signatureCheck_h
#define signatureCheck_h

#include <stdio.h>
RSA* createPublicRSA(const char *key);

int hexstringToBytes(const char* hexstring, unsigned char** buffer, size_t* length);
#endif /* signatureCheck_h */
