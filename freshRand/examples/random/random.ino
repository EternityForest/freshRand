#include "freshRand.h"

//This is an extremely simple to use library. Basically all you do is call freshRand(X) when you want a random byte. X
//Is a strength parameter that affects the quality and speed of generation. Lower numbers are faster.
//8 is the minimum for decent quality, 255 is the slowest and best. 8 will probably take around 250-800 microseconds per random number(it adds up!)
//Anything lower than 8 is likely to be mostly non random and highly biased, unless there is sufficent randomness in the timing and stack

//However, you might be OK using freshRand(0) in applications involving human input, because the micros() value is
//Fairly random when sampled an a random time. Y


void setup() {
  Serial.begin(9600);
}

void loop() {
  //Repeatedly spew random numbers out the serial port.
  Serial.write(freshRand(24)); 
}



