freshRand
=========

Generate true random numbers on Arduino and Energia. Planned support for 328 ,32u4,MSP430, and others.
Basic usage:

    #include "freshRand.h"
    
    //24 is the strength parameter
    myVariableIWantToBeRandom = freshRand(24)

freshRand will probably take about half a millisecond to generate one truly random byte. A future version of this
library will include an entropy pool that you can seen with true random numbers and use as a PRNG.

freshRand works based on the internal temperature sensor. Basically, when you call freshRand(),
the 8 least significant bits of the temperaure sensor are mixed into a variable called entropy repeatedly.
Basically, this is what happens:

    if not in PURE mode(PURE is off by default but can be enabled by uncommenting a line in freshRand.h):
        entropy is left uninitialized to add in whatever crap was on the stack
        the 8 least significant bits of micros() (or the raw timer0 if available) are ADDED to entropy
        
    Until the entropy estimation algorithm decides its enough:
        Rotate entropy one place right
        Entropy = entropy XOR temperature reading
        Entropy = entropy += 0x55

The 0x55 is probably unnecessary and the algorithm works just fine without it,
but it adds extra uncertanty based on the number of rounds it runs for.
    
The entropy estimation algorithm is based on counting changes. A truly random string of bits has a 50% chance of changing
with every bit. So, in every n bits there will on average be n/2 bits that changed and n/2 bits that stayed the same.
If the balance is interrupted, the bits are less than random and contain less than one bit of entropy per bit,
because they are in some way predictable.

Because the least significant bit of a noisy reading changes much more than the rest, we use the same change counting metric with
the entire byte. This produces a slightly low estimation of the entropy, because other bits are changing too.

In theory, when we detected 4 bits that changed and 4 bits that stayed the same, we might have enough entropy for one byte.
However, statistical analysis proves that 4 is an insignificant strength factor and produces terribly biased digits,
either because the estimating theory is very crude,
there is patterned non-random noise from the oscillators and such in the ADC readings, and the entropy disstilling 
function is imperfect. So, we increase the strength factor. We find that a strength factor of 8 produces random looking numbers,
which pass statistical tests, and a strength of 24-255 will probably be good enough for just about anything unless there is some deep
uniscovered flaw in my design.

In some applications, even a strength factor of 0 will do, becasue of the timing and stack noise. 


