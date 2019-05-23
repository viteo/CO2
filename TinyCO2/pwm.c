#include <avr/io.h>
#include "pwm.h"

void InitPWM()
{
	#ifdef PWM_PB0
	DDRB|=(1<<PWM_PB0);
	#endif
	#ifdef PWM_PB1
	DDRB|=(1<<PWM_PB1);
	#endif
	
	#ifdef PWM_NORMAL
	// Normal Mode
	// F_PWM = F_CPU / (Prescaler * 512)
	#ifdef PWM_PB0
	TCCR0A |=(1<<COM0A0);
	PWM0 = 0;
	#endif
	#ifdef PWM_PB1
	TCCR0A |=(1<<COM0B0);
	PWM1 = 0;
	#endif
	#endif /* !PWM_NORMAL */

	#ifdef PWM_CTC
	// Clear Timer on Compare Match (CTC) Mode
	// F_PWM = F_CPU / (2 * Prescaler * (1 + OCR0A))
	TCCR0A |=(1<<WGM01);
	#ifdef PWM_PB0
	TCCR0A |= (1<<COM0A0);
	PWM0 = 0;
	#endif
	#ifdef PWM_PB1
	TCCR0A |= (1<<COM0B0);
	PWM1 = 0;
	#endif
	#endif /* !PWM_CTC */

	#ifdef PWM_FAST
	// Fast PWM Mode
	// F_PWM = F_CPU / (Prescaler * 256)
	TCCR0A |=(1<<WGM00) | (1<<WGM01);
	#ifdef PWM_PB0
	TCCR0A |=(1<<COM0A1) | (0<<COM0A0); // TCNT0 == OCROA ? 0; Overflow ? 1 (inverted)
	PWM0 = 0;
	#endif
	#ifdef PWM_PB1
	TCCR0A |=(1<<COM0B1) | (1<<COM0B0); // TCNT0 == OCROB ? 1; Overflow ? 0
	PWM1 = 255;
	#endif
	#endif /* !PWM_FAST */

	#ifdef PWM_PHASECORRECT
	// Phase Correct PWM Mode
	// F_PWM = F_CPU / (Prescaler * 512)
	TCCR0A |=(1<<WGM00);
	#ifdef PWM_PB0
	TCCR0A |=(1<<COM0A1) | (0<<COM0A0); //inverted
	PWM0 = 0;
	#endif
	#ifdef PWM_PB1
	TCCR0A |=(1<<COM0B1) | (1<<COM0B0);
	PWM1 = 255;
	#endif
	#endif /* !PWM_PHASECORRECT */

	#ifdef PRESCALE
	TCCR0B |= PRESCALE;
	#endif
}