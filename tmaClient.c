//
//  main.c
//  TMAControl-CClient
//
//  Created by Ronan LE MEILLAT on 17/03/2021.
//

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/ssl.h>
#include "cJSON/cJSON.h"
#include "signatureCheck.h"

struct string {
    char *ptr;
    size_t len;
};
char error_buffer[CURL_ERROR_SIZE] = "";

CURLcode connectToServer(CURL *curl, char *url, char **certificate, struct string *s);
CURLcode sslctxfun(CURL *curl, SSL_CTX *sslctx, void *parm);
int verify_callback(int preverify_ok, X509_STORE_CTX *x509_ctx);
void print_certificate(X509 *cert);
void init_string(struct string *s);

/* arrays for certificate chain and errors */
#define MAX_CERTS 20
X509 *certificate[MAX_CERTS];
long certificate_error[MAX_CERTS];

int main(int argc, char *argv[])
{
    struct string message;
    init_string(&message);
    
    CURL *curl;
    CURLcode code;
    
    if (argc != 2){
        fprintf(stderr, "Error secured message URL is required perhaps you forgot https://tmalille31.highcanfly.club/tmastatesecuredmessage\n");
        return -1;
    }
    
    curl = curl_easy_init();
    assert(curl);
    
    
    char *sz_cert;
    code = connectToServer(curl, argv[1], &sz_cert, &message);
    if (code != CURLE_OK)
        fprintf(stderr, "Error %u: %s\n%s\n",
                code,
                curl_easy_strerror(code),
                error_buffer);
    else
        fprintf(stderr,"%sUU",sz_cert);
    RSA *rsa = createPublicRSA(sz_cert);
    
    curl_easy_cleanup(curl);
    if (code != CURLE_OK)
        return -1;
    fprintf(stdout,"raw message: %s\n",message.ptr);
    cJSON *json = cJSON_Parse(message.ptr);
    free(sz_cert);
    free(message.ptr);
    const cJSON *_message = NULL;
    const cJSON *_timestamp = NULL;
    const cJSON *_signature = NULL;
    const cJSON *_uuid = NULL;
    _message = cJSON_GetObjectItemCaseSensitive(json, "message");
    _signature = cJSON_GetObjectItemCaseSensitive(json, "signature");
    
    unsigned char *bsignature;
    size_t bsignature_len;
    
    hexstringToBytes(_signature->valuestring, &bsignature, &bsignature_len);
    // modify the signature for getting a wrong one (in fact if 32th byte is 6……
    //*(bsignature+32)=6;
    //    printf("0x");
    //    for(size_t count = 0; count < strlen(_signature->valuestring)/2; count++)
    //        printf("%02x", bsignature[count]);
    //    printf("\n");
    
    
    if (!_message){
        fprintf(stderr, "Error secured message not received, is there any server problem or is your URL wrong?\n");
        return -2;
    }
    int authentic;
    RSAVerifySignature( rsa,
                       bsignature,
                       bsignature_len,
                       _message->valuestring,
                       strlen(_message->valuestring),
                       &authentic);
    if (!authentic){
        fprintf(stderr, "Error signature is wrong.\n");
        return -3;
    }
    cJSON *json_2nd_level = cJSON_Parse(_message->valuestring);
    _timestamp = cJSON_GetObjectItemCaseSensitive(json_2nd_level, "timestamp");
    _uuid = cJSON_GetObjectItemCaseSensitive(json_2nd_level, "uuid");
    fprintf(stdout,"signature: %s\n",_signature->valuestring);
    fprintf(stdout,"uuid: %s\n",_uuid->valuestring);
    fprintf(stdout,"timestamp: %s\n",_timestamp->valuestring);
    free(bsignature);
    return 0;
}

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    
    return size*nmemb;
}


CURLcode connectToServer(CURL *curl, char *url, char **certificate,struct string *response)
{
    CURLcode code;
    
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
    if (code != CURLE_OK)
        return code;
    code = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (code != CURLE_OK)
        return code;
    //  code = curl_easy_setopt(curl, CURLOPT_WRITEHEADER, stdout);
    //  if (code != CURLE_OK)
    //    return code;
    
    code = curl_easy_setopt(curl, CURLOPT_CERTINFO, 1L);
    if (code != CURLE_OK)
        return code;
    
    code = curl_easy_perform(curl);
    if (!code) {
        struct curl_certinfo *ci;
        code = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &ci);
        
        if (!code) {
            if (ci->num_of_certs)
            {
                struct curl_slist *slist;
                for(slist = ci->certinfo[0]; slist; slist = slist->next)
                {
                    if (strstr( slist->data, "Cert:" ) )
                    {
                        size_t len = strlen(slist->data+strlen("Cert:"));
                        *certificate = (char *) malloc( (len+1)*sizeof(char) );
                        strcpy(*certificate, slist->data+5);
                    }
                }
            }
        }
    }
    return code;
}


