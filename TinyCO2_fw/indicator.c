#define LEDLOW PB2
#define LEDHIGH PB5

#define LEDOFF (PORTB & ~(1<<LEDLOW)) & ~(1 << LEDHIGH)
#define LEDGREEN (PORTB | (1 << LEDLOW)) & ~(1 << LEDHIGH)
#define LEDYELLOW (PORTB | (1 << LEDLOW)) | (1 << LEDHIGH)
#define LEDRED (PORTB | (1 << LEDHIGH)) & ~(1 << LEDLOW)

#include "indicator.h"
#include "pwm.h"
#include <util/delay.h>

void InitIndicators()
{
	DDRB |= (1<<LEDLOW) | (1<<LEDHIGH);
	InitPWM();
}

void IndicatorSetArrow(SCALE scale, int value)
{
	if(scale == PPM)
	{
		value /= 8;
		if (value < 53)  {PWM0 = 0;						return;}
		if (value < 76)	 {PWM0 = value + value/2 - 78;	return;}	//424 .. 600
		if (value < 125) {PWM0 = value - 40;			return;}	//608 .. 992
		if (value < 151) {PWM0 = value + value/2 - 103;	return;}	//1000.. 1200
		if (value < 201) {PWM0 = value - 30;			return;}	//1208.. 1600
		if (value < 254) {PWM0 = value + value/2 - 129;	return;}	//1608.. 2000
		PWM0 = 0; return;
	}
	else if(scale == TEMPERATURE)
	{
		PWM0 = 8 * value - 70;
	}
}

void IndicatorSetLed(int value)
{
	if(value > 1600)
		PORTB = LEDRED;
	else if (value > 1200)
		PORTB = LEDYELLOW;
	else if (value > 400)
		PORTB = LEDGREEN;
	else
		PORTB = LEDOFF;
}

void IndicatorDance()
{
	PORTB = LEDRED;
	for(int i = 0; i<250; i++)
	{
		_delay_ms(20);
		PWM0 = i;
	}
	PORTB = LEDYELLOW;
	for(int i = 250; i>0; i--)
	{
		_delay_ms(20);
		PWM0 = i;
	}
	PORTB = LEDGREEN;
}