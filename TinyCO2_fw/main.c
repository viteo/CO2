#define SWITCH PB5
#define _BV(x)   (1 << x)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "mhz19.h"
#include "indicator.h"

//PB0 PWM
//PB1 LED
//PB2 LED
//PB3 uart TX
//PB4 uart RX
//PB5 switch

typedef unsigned char byte;

ISR(WDT_vect)	//Watchdog timer
{
	retrieveData();
	int ppm = getPPM();
	IndicatorSetArrow(PPM, ppm);
	IndicatorSetLed(ppm);
	
	sei();
}

ISR(PCINT0_vect)	//control switching
{
	if (PINB & (1 << SWITCH))
	{ //switch to interntal control
		{ //software reset
			WDTCR = (_BV(WDTIF) | _BV(WDE) | _BV(WDCE) | _BV(WDP2) | _BV(WDP0));
			while(1) {}
		}
	}
	else
	{ //switch to external control
		WDTCR &= ~(_BV(WDTIE)); //disable watchdog
	}
}

int main(void)
{
	if(MCUSR & _BV(WDRF))	// If a reset was caused by the Watchdog Timer...
	{
		MCUSR &= ~_BV(WDRF);				// Clear the WDT reset flag
		WDTCR |= (_BV(WDCE) | _BV(WDE));	// Enable the WD Change Bit
		WDTCR = 0x00;						// Disable the WDT
	}

	InitIndicators();
	IndicatorDance(); //10seconds time to init mhz19

	DDRB &= ~(1 << SWITCH);	//Set SWITCH to logic input
	PORTB |= (1 << SWITCH);	//Activate Pull-up Resistor
	PCMSK |= (1 << PCINT5); //Pin change mask
	GIMSK |= (1 << PCIE);	//Pin change interrupt on bit SWITCH
	MCUCR |= (1 << ISC00);	//logical change

	// Set up Watch Dog Timer for Inactivity
	WDTCR |= _BV(WDCE);					// Enable the WD Change Bit
	WDTCR = _BV(WDP3) | _BV(WDP0);		// Set Timeout to ~8 seconds and clear WDE
	
	if(PINB & (1 << SWITCH))
		WDTCR |= _BV(WDTIE);			// Enable WDT Interrupt
	sei();

	for (;;)
	{
		if(PINB & (1 << SWITCH))
		{

		}
		else
		{
			if(!waitData())
			{
				int ppm = getPPM();
				IndicatorSetArrow(PPM, ppm);
				IndicatorSetLed(ppm);
			}
			sei();
		}
	}
}

