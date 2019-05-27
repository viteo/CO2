#include "indicator.h"
#include "pwm.h"

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