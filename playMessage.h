//
//  playMessage.h
//  TMAControl-CClient
//
//  Created by Ronan LE MEILLAT on 18/03/2021.
//

#ifndef playMessage_h
#include "config.h"
#define playMessage_h
#define SOUND_EXTENSION ".mp3"
#ifndef SOUND_FILES_LOCATION
#define SOUND_FILES_LOCATION "/Users/rlemeill/Development/TMAControl-CClient/sounds/"
#endif
#include <stdio.h>
int playMessage(char *uuid );
#endif /* playMessage_h */
