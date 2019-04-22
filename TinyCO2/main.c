#define SWITCH PB1
#define _BV(x)   (1 << x)

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include "pwm.h"
#include "mhz19.h"
#include "uart.h"

//PB0 PWM
//PB1
//PB3 uart TX
//PB4 uart RX

typedef unsigned char byte;

byte remoteOn = 0;

//watchdog timer
ISR(WDT_vect)
{
	//get ppm
	retrieveData();
	PWM0 = getPPM() / 8;
		
	sei();
}

//remote switching
ISR(INT0_vect)
{
	if (PINB & (1 << SWITCH))
	{
		remoteOn = 1;
	}
	else
	{
		remoteOn = 0;
	}
}

int main(void)
{
	InitPWM();
	PWM0 = 0;
	
	DDRB &= ~(1 << SWITCH); //Set to logic input
	PORTB |= (1 << SWITCH); //Activate Pull-up Resistor
	GIMSK |= (1 << INT0); //INT0 interrupt  on bit PB1 SWITCH
	MCUCR |= (1 << ISC00); //logical change
	
	if(MCUSR & _BV(WDRF)){            // If a reset was caused by the Watchdog Timer...
		MCUSR &= ~_BV(WDRF);                 // Clear the WDT reset flag
		WDTCR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
		WDTCR = 0x00;                      // Disable the WDT
	}

	// Set up Watch Dog Timer for Inactivity
	WDTCR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
	WDTCR =   _BV(WDTIE) |              // Enable WDT Interrupt
	_BV(WDP3) | _BV(WDP0) ;   // Set Timeout to ~8 seconds

	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	
	sei();

	for (;;)
	{
		if (MCUCR & _BV(SE))
		{    // If Sleep is Enabled...
			cli();                 // Disable Interrupts
			sleep_bod_disable();   // Disable BOD
			sei();                 // Enable Interrupts
			sleep_cpu();           // Go to Sleep
		}
	}
}

