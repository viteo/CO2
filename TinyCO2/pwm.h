#include <avr/io.h>

#ifndef	_PWM_
#define	_PWM_

//#define PWM_NORMAL
//#define PWM_CTC
#define PWM_FAST
//#define PWM_PHASECORRECT

#define PWM_PB0 PB0
//#define PWM_PB1 PB1

#ifdef PWM_PB0
#define PWM0 OCR0A
#endif

#ifdef PWM_PB1
#define PWM1 OCR0B
#endif

//#define PRESCALE (1<<CS00) // 1
#define PRESCALE (1<<CS01) // 8
//#define PRESCALE ((1<<CS01) | (1<<CS00)) // 64
//#define PRESCALE (1<<CS02) // 256
//#define PRESCALE ((1<<CS00) | (1<<CS02)) //1024

#ifndef F_CPU
#define F_CPU (1200000UL) // 1.2 MHz
#endif  /* !F_CPU */

void InitPWM();

#endif /* !_PWM_ */