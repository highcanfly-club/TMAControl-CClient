AUTOMAKE_OPTIONS = subdir-objects
LIBS += -lcrypto -lssl
tmaClient_CFLAGS = $(OPENSSL_INCLUDES)
tmaClient_LDFLAGS = $(OPENSSL_LDFLAGS) 
bin_PROGRAMS = tmaClient

tmaClient_SOURCES = signatureCheck.c playMessage.c cJSON/cJSON.h cJSON/cJSON.c tmaClient.c tmaClient.h playMessage.h gpio.c gpio.h date_parse.c date_parse.h
