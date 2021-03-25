AUTOMAKE_OPTIONS = subdir-objects
LIBS = -lcrypto -lssl -lcurl -lao -lmpg123
tmaClient_CFLAGS = $(OPENSSL_INCLUDES)
tmaClient_LDFLAGS = $(OPENSSL_LDFLAGS) 
bin_PROGRAMS = tmaClient

tmaClient_SOURCES = signatureCheck.c playMessage.c cJSON/cJSON.h cJSON/cJSON.c tmaClient.c playMessage.h 
