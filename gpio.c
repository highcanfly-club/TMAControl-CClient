//
//  ptt.c
//  TMAControl-CClient
//
//  Created by Ronan LE MEILLAT on 25/03/2021.
//
//
#include "gpio.h"
#include <stdio.h>
#ifdef HAVE_LIBWIRINGPI
#include <wiringPi.h>
#endif

int power_on_radio(){
#ifdef HAVE_LIBWIRINGPI	
		wiringPiSetup();
		pinMode(PTT_PIN, OUTPUT);
		digitalWrite(PTT_PIN,HIGH);
		return 0;
#else
        fprintf(stderr,"NO GPIO Found install libwiringPi\n");
        return -1;
#endif
}

int power_off_radio(){
#ifdef HAVE_LIBWIRINGPI	
		digitalWrite(PTT_PIN,LOW);
		return 0;
#else
        fprintf(stderr,"NO GPIO Found install libwiringPi\n");
        return -1;
#endif
}

int push_to_talk_setup(){
#ifdef HAVE_LIBWIRINGPI
	wiringPiSetup();
	pinMode(PTT_PIN, OUTPUT);
	digitalWrite(PTT_PIN,HIGH);
	return 0;
#else
        fprintf(stderr,"NO GPIO Found install libwiringPi\n");
        return -1;
#endif
}

int push_to_talk_on(){
#ifdef HAVE_LIBWIRINGPI
	fprintf(stderr,"Putting pin GPIO.%d to %d\n",PTT_PIN,LOW);
	digitalWrite(PTT_PIN,LOW); //PTT is active low on Motorola
	return 0;
#else
	fprintf(stderr,"NO GPIO Found install libwiringPi\n");
	return -1;
#endif

}

int push_to_talk_off(){
#ifdef HAVE_LIBWIRINGPI
	fprintf(stderr,"Putting pin GPIO.%d to %d\n",PTT_PIN,HIGH);
	digitalWrite(PTT_PIN,HIGH); //PTT is active low on Motorola
        return 0;
#else
        fprintf(stderr,"NO GPIO Found install libwiringPi\n");
        return -1;
#endif
}
