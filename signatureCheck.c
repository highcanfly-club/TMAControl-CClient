#include <openssl/ssl.h>
#include <string.h>
#include "signatureCheck.h"

RSA* createPublicRSA(const char *key) {
  RSA *rsa = NULL;

  BIO *keybio = BIO_new_mem_buf((void*)key, -1);
  if (keybio==NULL) {
      return 0;
  }
  rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
  return rsa;
}

int hexstringToBytes(const char* hexstring, unsigned char** buffer, size_t *length)
{    
    const char  *pos = hexstring;
    *length = strlen(hexstring)/2; //on hexstring each byte is coded with 2 chars
  //  *buffer = (unsigned char*)malloc(*length);

    unsigned char *val=(unsigned char*)malloc( (*length)*sizeof(unsigned char));
     /* WARNING: no sanitization or error-checking whatsoever */
    for (size_t count = 0; count < (*length)*sizeof(unsigned char); count++) {
        sscanf(pos, "%2hhx", &val[count]);
        pos += 2;
    }
    
    *buffer = &(*val);
    
    return 0;
    }
