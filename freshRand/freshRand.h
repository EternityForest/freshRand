#ifndef _freshRand
#define _freshRand

//Uncomment this to use PURE mode, in which stack garbage and timing data are not used, and the temperature sensor is used as the only entropy source.
//This mode exists for testing purposes, because there may be applications in which the timing is deterministic.
//So we need to be sure that the algorithms work under those conditions.

//#define PURE


unsigned char freshRand(unsigned char);
unsigned char freshURand(unsigned char);
unsigned int getTemperature();

#include "entropypools.h"
#endif
