//
//  tmaClient.h
//  TMAControl-CClient
//
//  Created by Ronan LE MEILLAT on 28/03/2021.
//

#ifndef tmaClient_h
#define tmaClient_h

enum _config_error
{
    E_SUCCESS = 0,
    E_NO_URL = -1,
    E_HTTPS_REQUIRED = -2,
    E_RSA_ERROR = -3,
    E_JSON_ERROR =-4,
    E_MESSAGE_NOT_RECEIVED = -5,
    E_NO_SIGNATURE_ERROR = -6,
    E_WRONG_SIGNATURE = -7,
    E_NO_JSON_INNER = -8,
    E_NO_TIMESTAMP_MESSAGE = -9,
    E_CONNECTION_ERROR = -10,
    E_DT_ERROR = -11,
    E_NO_UUID = -12
};

/* type to provide in your API */
typedef enum _config_error error_t;

/* use this to provide a perror style method to help consumers out */
struct _errordesc {
    int  code;
    char *message;
} errordesc[] = {
    { E_SUCCESS, "No error" },
    { E_NO_URL, "Error secured message URL is required perhaps you forgot https://tmalille31.highcanfly.club/tmastatesecuredmessage"},
    { E_HTTPS_REQUIRED, "Error secured message URL is required, yours doesn't seem to be an https:// url"},
    { E_RSA_ERROR, "RSA key error" },
    { E_JSON_ERROR, "Can't decode json message"},
    { E_MESSAGE_NOT_RECEIVED,"Error secured message not received"},
    { E_NO_SIGNATURE_ERROR, "Don't see signature in message"},
    { E_WRONG_SIGNATURE, "Error signature is wrong"},
    { E_NO_JSON_INNER, "Don't see inner json in message"},
    { E_NO_TIMESTAMP_MESSAGE,"Don't see timestamp in inner json message"},
    { E_CONNECTION_ERROR, "Can't connect to server Error" },
    { E_DT_ERROR, "Δt too important"},
    { E_NO_UUID, "Don't see uuid in inner json message."}
};

#endif /* tmaClient_h */
