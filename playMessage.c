//
//  playMessage.c
//  TMAControl-CClient
//
//  Created by Ronan LE MEILLAT on 18/03/2021.
//

#include "playMessage.h"
#include <ao/ao.h>
#include <mpg123.h>
#include <string.h>

#define BITS 8

int playMessage(char *uuid)
{
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;

    size_t filename_size_needed = strlen(SOUND_FILES_LOCATION)+strlen(uuid)+strlen(SOUND_EXTENSION)+1;
    char *filename = (char*)malloc((filename_size_needed)*sizeof(char));
    snprintf(filename,filename_size_needed,"%s%s%s", SOUND_FILES_LOCATION, uuid, SOUND_EXTENSION);
    
    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mh, filename);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    fprintf(stderr, "Playing: %s\n", filename);
    /* decode and play */
    int mpg123_ret = MPG123_OK;
    while (mpg123_ret == MPG123_OK)
        {
            mpg123_ret = mpg123_read(mh, buffer, buffer_size, &done);
            ao_play(dev, buffer, done);
        }

    /* clean up */
    free(buffer);
    free(filename);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

    return 0;
}
