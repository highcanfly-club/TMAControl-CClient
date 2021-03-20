//
//  signatureCheck.c
//  TMAControl-CClient
//
//  Created by Ronan LE MEILLAT on 17/03/2021.
//

#include <openssl/ssl.h>
#include <string.h>
#include "signatureCheck.h"

RSA* createPublicRSA(const char *key) {
    X509 *x509 = NULL;
    RSA *rsa = NULL;
    EVP_PKEY *pkey = NULL;
    
    BIO *certbio = BIO_new_mem_buf((void*)key, -1);
    if (certbio==NULL) {
        return 0;
    }
    if( PEM_read_bio_X509(certbio, &x509,NULL, NULL) )
    {
        pkey = X509_get_pubkey(x509);
        rsa = EVP_PKEY_get1_RSA(pkey);
        if (rsa )
        {
            return rsa;
        }
    }
    return NULL;
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

int RSAVerifySignature( RSA* rsa,
                       unsigned char* MsgHash,
                       size_t MsgHashLen,
                       const char* Msg,
                       size_t MsgLen,
                       int* Authentic) {
    *Authentic = 0;
    EVP_PKEY* pubKey  = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pubKey, rsa);
    EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_create();
    
    if (EVP_DigestVerifyInit(m_RSAVerifyCtx,NULL, EVP_sha256(),NULL,pubKey)<=0) {
        return 0;
    }
    if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, Msg, MsgLen) <= 0) {
        return 0;
    }
    int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, MsgHash, MsgHashLen);
    if (AuthStatus==1) {
        *Authentic = 1;
        EVP_MD_CTX_free(m_RSAVerifyCtx);
        return 1;
    } else if(AuthStatus==0){
        *Authentic = 0;
        EVP_MD_CTX_free(m_RSAVerifyCtx);
        return 0;
    } else{
        *Authentic = 0;
        EVP_MD_CTX_free(m_RSAVerifyCtx);
        return 0;
    }
}
