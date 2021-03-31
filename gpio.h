//
//  ptt.h
//  TMAControl-CClient
//
//  Created by Ronan LE MEILLAT on 25/03/2021.
//

#ifndef gpio_h
#include "config.h"

#ifdef HAVE_LIBWIRINGPI
#include <wiringPi.h>
#else
#define LOW 0
#define HIGH 1
#endif

#ifndef INVERTED_LOGIC
#define PTT_ON  LOW     // PTT_ON correspond to +5V on radio PTT pin  (after optoisolator)
#define PTT_OFF HIGH      // PTT_OFF correspond to 0V on radio PTT pin  (after optoisolator) 
#else
#define PTT_ON  HIGH     // PTT_ON correspond to 0V on Motorola PTT pin  (after optoisolator)
#define PTT_OFF LOW      // PTT_OFF correspond to +5V on Motorola PTT pin   (after optoisolator)
#endif

#define gpio_h
int power_on_radio();
int power_off_radio();
int push_to_talk_setup();
int push_to_talk_on();
int push_to_talk_off();
#endif 
