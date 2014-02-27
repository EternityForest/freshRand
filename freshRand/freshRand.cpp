#include "freshRand.h"
#include "Arduino.h"

/*
Generate one random byte. Two entropy sources are used, clock drift and temperature.
 How this works is we repeatedly take temperature readings and add them to a variable called entropy.
 We keep track of the number of times the reading has been the same as the last one, and the number of times it has been different.
 When the number of changes in the signal and the number of values that are the same both exceed the strength parameter, we return the value of entropy.
 */

unsigned char freshRand(unsigned char strength)
{
  //deliberately left uninitialized, so in case there is some stack garbage that we can use.
  //Unless one uses PURE mode, which does not use any entropy sources except the temperature sensor.
  //PURE mode is useful for testing.

#ifdef PURE
  unsigned char entropy = 0;
#else
  unsigned char entropy;
#endif

  unsigned char changes = 0;
  unsigned char sames = 0;
  unsigned char last=0;
  unsigned char temp = 0;

  //If there is a timer running, add in the timer value. This alone is random enough for quite a bit.
  //Keep in mind that a typical application will have interrupts going, it will have user input, sensors,
  //And all kinds of things that influence the exact times that things happen.
  //First try getting the raw timer value, but if that fails, fallback to the micros() function.
  
  //Don't use timer values in PURE mode.

#ifdef PURE
#ifdef TMR0CNT
  entropy += TMR0CNT;
#else
  entropy += micros()%255;
#endif
#endif

  while((changes<strength) || (sames< strength))
  {
    temp = (getTemperature() & 255);


    //Entropy estimation
    if (temp != last)
    {
      changes ++;
      last=temp;
    }
    else
    {
      sames++;
    }

    //Make it compile cleanly. We want that variable to be uninitialized!
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    //Bit rotate the byte, and add in the byte we got from the sensor.
    entropy = ((entropy<<1) | (entropy>>7));
    //Add the byte via XORing
    entropy ^= temp;
    //Adding 0x55 into entropy every round means that the 
    entropy += 0x55;
  }

  
    //Once we have enough entropy, return the byte.
#ifdef PURE
#ifdef TMR0CNT
  entropy ^= TMR0CNT;
#else
  entropy ^= micros()%255;
#endif
#endif

  //Once we have enough entropy, return the byte.
  return (entropy);
#pragma GCC diagnostic pop
}

unsigned int getTemperature()
{

#ifdef  __AVR_ATmega32U4__
unsigned char oldADCSRA = ADCSRA;
unsigned char oldADMUX = ADMUX;
unsigned char oldADCSRB = ADCSRB;

  //disable ADC...now new values can be written in MUX register
  ADCSRA &= ~(1 << ADEN);   
  // Set MUX to use on-chip temperature sensor
  ADMUX = (1 << MUX0) | (1 << MUX1) | (1 << MUX2);
  ADCSRB =  (1 << MUX5);   // MUX 5 bit part of ADCSRB

  ADCSRB |=  (1 << ADHSM);   // High speed mode

  // Set Voltage Reference to internal 2.56V reference with external capacitor on AREF pin
  ADMUX |= (1 << REFS1) | (1 << REFS0);

  // Enable ADC conversions
  ADCSRA |= (1 << ADEN);

  ADCSRA |= (1 << ADSC);

  while(!(ADCSRA & (1 << ADIF)));
  ADCSRA |= (1 << ADIF);

  delayMicroseconds(5);

  //Leave things like we found them
  ADMUX = oldADMUX;
  ADCSRB = oldADCSRB;
  

  return ADC;
#elif defined(__MSP430G2452__) || defined(__MSP430G2553__) || defined(__MSP430G2231__) 
  return analogRead(TEMPSENSOR);

#elif defined( __AVR_ATmega328P__) || defined( __AVR_ATmega168P__) || defined( __AVR_ATmega328__) || defined( __AVR_ATmega168__)
  unsigned int wADC;
  
  unsigned char oldADCSRA = ADCSRA;
unsigned char oldADMUX = ADMUX;
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

    // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;
  
  //Leave things like we found them
  ADMUX = oldADMUX;
  
  return(wADC);
#else
#warning "Your processor either has no temperature sensor or it is not supported by this library. Entropy may be extremely poor"
#endif
}

