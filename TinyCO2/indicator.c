#define LEDOUT PB2

#include "indicator.h"
#include "pwm.h"
#include <util/delay.h>

void InitIndicators()
{
	DDRB |= (1<<LEDOUT);
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
	if(value > 800)
	return; //turn yellow led
}

void IndicatorDance()
{
	PORTB |= (1 << LEDOUT);
	for(int i = 0; i<250; i++)
	{
		_delay_ms(20);
		PWM0 = i;
	}
	for(int i = 250; i>0; i--)
	{
		_delay_ms(20);
		PWM0 = i;
	}
	PORTB &= ~(1 << LEDOUT);
}