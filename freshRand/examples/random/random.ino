#include "freshRand.h"

//This is an extremely simple to use library. Basically all you do is call freshRand(X) when you want a random byte. X
//Is a strength parameter that affects the quality and speed of generation. Lower numbers are faster.
//8 is the minimum for decent quality, 255 is the slowest and best. 8 will probably take around 250-800 microseconds per random number(it adds up!)
//Anything lower than 8 is likely to be mostly non random and highly biased, unless there is sufficent randomness in the timing and stack

//However, you might be OK using freshRand(0) in applications involving human input, because the micros() value is
//Fairly random when sampled an a random time. Y

//You can also create an entropy pool object with "EntropyPool pool;", and call pool.urandom() which returns one byte.
//EntropyPool has a method seed(X) that mixes in X bytes of entropy into the state. The bytes are generated with strength 24.
//EntropyPool class is an RC4 based entropy pool, so the numbers may or may not be cryptographically random, and likely are not.
//EntropyPool will also consume over 258 bytes of RAM, and as such might not be workable or even compilable on some chips.

EntropyPool pool;


void setup() {
  Serial.begin(9600);
  pool.seed(255);
}

void loop() {
  //Repeatedly spew random numbers out the serial port.
  //Note that 8 is a VERY low number very close to the minimum. Use 255 for anything important, and 16-48 at least for games and stuff.
  Serial.write(pool.urandom()); 
}



