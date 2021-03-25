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
#include "playMessage.h"
#include "gpio.h"
#include <unistd.h>


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
int connect_and_process(char *url);

int main(int argc, char *argv[])
{
    if (argc != 2){
        fprintf(stderr, "Error secured message URL is required perhaps you forgot https://tmalille31.highcanfly.club/tmastatesecuredmessage\n");
        return -1;
    }
  //  while (1)
    {
        int ret = connect_and_process(argv[1]);
   //     usleep(1000);
    }
    return 0;
}

int connect_and_process(char *url)
{
    struct string message;
    init_string(&message);
    
    CURL *curl;
    CURLcode code;
    

    
    push_to_talk_setup();

    curl = curl_easy_init();
    assert(curl);
    
    
    char *sz_cert = NULL;

    // Connect to the server, fill the message with the response and fill the sz_cert with the x509 certificate received
    // if there is a certificate problem libcurl reports it, and close the connection

    code = connectToServer(curl, url, &sz_cert, &message);
    if (code != CURLE_OK)
    {
        fprintf(stderr, "Error %u: %s\n%s\n",
                code,
                curl_easy_strerror(code),
                error_buffer);
        curl_easy_cleanup(curl);
        free(sz_cert);
        free(message.ptr);
        message.len=0;
        return -10;
    }
    else
    {
#ifdef DEBUG
        fprintf(stderr,"%s",sz_cert);
#endif
    RSA *rsa = createPublicRSA(sz_cert);
    
    curl_easy_cleanup(curl);
        if (code != CURLE_OK)
        {
            RSA_free(rsa);
            return -1;
        }
#ifdef DEBUG
    fprintf(stdout,"raw message: %s\n",message.ptr);
#endif
    cJSON *json = cJSON_Parse(message.ptr);

    free(sz_cert);
    free(message.ptr); message.len=0;
    
    if (json)
    {
        const cJSON *_message = NULL;
        const cJSON *_signature = NULL;
        
        _message = cJSON_GetObjectItemCaseSensitive(json, "message");
        _signature = cJSON_GetObjectItemCaseSensitive(json, "signature");
        
        unsigned char *bsignature = NULL;
        size_t bsignature_len;
    
        if (!_message){
            fprintf(stderr, "Error secured message not received, is there any server problem or is your URL wrong?\n");
            RSA_free(rsa);
            return -2;
        }
        
        if(_signature)
        {
            hexstringToBytes(_signature->valuestring, &bsignature, &bsignature_len);}
        else{
            RSA_free(rsa);
            return -3;
        }
        
        // keeps this debug code if I need more testing
        // modify the signature for getting a wrong one (in fact if 32th byte is 6……
        //*(bsignature+32)=6;
        //    printf("0x");
        //    for(size_t count = 0; count < strlen(_signature->valuestring)/2; count++)
        //        printf("%02x", bsignature[count]);
        //    printf("\n");
        
        
        
        // Now it's time to check the message
        int authentic;
        RSAVerifySignature( rsa,
                           bsignature,
                           bsignature_len,
                           _message->valuestring,
                           strlen(_message->valuestring),
                           &authentic);
        if (!authentic){
            fprintf(stderr, "Error signature is wrong.\n");
            if (bsignature) free(bsignature);
            RSA_free(rsa);
            return -3;
        }
        
        const cJSON *json_2nd_level = NULL;
        json_2nd_level = cJSON_Parse(_message->valuestring);
        if (json_2nd_level)
        {
            const cJSON *_timestamp = NULL;
            const cJSON *_uuid = NULL;
            _timestamp = cJSON_GetObjectItemCaseSensitive(json_2nd_level, "timestamp");
            _uuid = cJSON_GetObjectItemCaseSensitive(json_2nd_level, "uuid");
            if (_timestamp && _uuid)
            {   // TODO
                // check timestamp
                //
#ifdef DEBUG
                fprintf(stdout,"signature: %s\n",_signature->valuestring);
                fprintf(stdout,"uuid: %s\n",_uuid->valuestring);
                fprintf(stdout,"timestamp: %s\n",_timestamp->valuestring);
#endif
                
                push_to_talk_on();
                playMessage(_uuid->valuestring);
                push_to_talk_off();
            }
            cJSON_Delete((cJSON *)json_2nd_level);
            
        }
        if (bsignature)
        {
            free(bsignature);
            
        }
        cJSON_Delete(json);
    }
        RSA_free(rsa);
    return 0;
    }
    
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


