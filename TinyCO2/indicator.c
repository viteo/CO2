#include "indicator.h"
#include "pwm.h"

//int approximate(int ppm)
//{
	//ppm /= 8;
	//if (ppm < 53) return 0;
	//if (ppm < 76) return ppm + ppm/2 - 78;	//424 .. 600
	//if (ppm < 125) return ppm - 40;			//608 .. 992
	//if (ppm < 151) return ppm + ppm/2 - 103;//1000.. 1200
	//if (ppm < 201) return ppm - 30;			//1208.. 1600
	//if (ppm < 254) return ppm + ppm/2 - 129;//1608.. 2000
	//else return 0;
//}

void IndicatorSetArrow(SCALE scale, int value)
{
	if(scale == PPM)
	{
		//PWM0 = approximate(value);
		value /= 8;
		if (value < 53) PWM0 = 0;						return;
		if (value < 76) PWM0 = value + value/2 - 78;	return;	//424 .. 600
		if (value < 125) PWM0 = value - 40;				return;	//608 .. 992
		if (value < 151) PWM0 = value + value/2 - 103;	return;	//1000.. 1200
		if (value < 201) PWM0 = value - 30;				return;	//1208.. 1600
		if (value < 254) PWM0 = value + value/2 - 129;	return;	//1608.. 2000
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