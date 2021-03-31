//
//  ptt.c
//  TMAControl-CClient
//
//  Created by Ronan LE MEILLAT on 25/03/2021.
//
//
#include "gpio.h"
#include <stdio.h>

int power_on_radio(){
#ifdef HAVE_LIBWIRINGPI	
		wiringPiSetup();
		pinMode(POWER_PIN, OUTPUT);
		digitalWrite(PTT_PIN,HIGH);
		return 0;
#else
        fprintf(stderr,"NO GPIO Found install libwiringPi need to put port wPi#%d to %d\n",POWER_PIN,HIGH);
        return -1;
#endif
}

int power_off_radio(){
#ifdef HAVE_LIBWIRINGPI	
		digitalWrite(POWER_PIN,LOW);
		return 0;
#else
        fprintf(stderr,"NO GPIO Found install libwiringPi need to put port wPi#%d to %d\n",POWER_PIN,LOW);
        return -1;
#endif
}

int push_to_talk_setup(){
#ifdef HAVE_LIBWIRINGPI
	wiringPiSetup();
	pinMode(PTT_PIN, OUTPUT);
	digitalWrite(PTT_PIN,PTT_OFF);
	return 0;
#else
        fprintf(stderr,"NO GPIO Found install libwiringPi need to put port wPi#%d to %d\n",PTT_PIN,PTT_OFF);
        return -1;
#endif
}

int push_to_talk_on(){
#ifdef HAVE_LIBWIRINGPI
	fprintf(stderr,"Putting pin GPIO.%d to %d\n",PTT_PIN,PTT_ON);
	digitalWrite(PTT_PIN,PTT_ON); //PTT is active low on Motorola but GPIO is inverted with opto-isolator
	return 0;
#else
	fprintf(stderr,"NO GPIO Found install libwiringPi need to put port wPi#%d to %d\n",PTT_PIN,PTT_ON);
	return -1;
#endif

}

int push_to_talk_off(){
#ifdef HAVE_LIBWIRINGPI
	fprintf(stderr,"Putting pin GPIO.%d to %d\n",PTT_PIN,PTT_OFF);
	digitalWrite(PTT_PIN,PTT_OFF); //PTT is active low on Motorola but GPIIO is inverted with opti-isolator
        return 0;
#else
        fprintf(stderr,"NO GPIO Found install libwiringPi need to put port wPi#%d to %d\n",PTT_PIN,PTT_OFF);
        return -1;
#endif
}
