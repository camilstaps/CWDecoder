#include <xc.h>
#include "timer.h"

static unsigned long int millis_timer = 0;

void timer_init(void)
{
	/* Timer 0:
	 * At 8MHz (IRCF=111), a 1:8 prescaler (PS=010)
	 * At 4MHz (IRCF=110), a 1:4 prescaler (PS=001)
	 * At 2MHz (IRCF=101), a 1:2 prescaler (PS=000)
	 * With period 250 this gives a 1kHz period for the ISR.
	 * Does not work for <2MHz clock (then the 250 period should be changed) */
	OPTION_REGbits.T0CS = 0;
	OPTION_REGbits.PSA = 0;
	OPTION_REGbits.PS = OSCCONbits.IRCF - 5;
	INTCON |= 0xe0;
}

void interrupt isr(void)
{
	if (INTCONbits.TMR0IF) {
		TMR0 -= 250;
		millis_timer++;
		INTCONbits.TMR0IF = 0;
	}
}

inline unsigned long int get_time(void)
{
	return millis_timer;
}

inline void delay(unsigned long int millis)
{
	millis += millis_timer;
	while (millis_timer < millis);
}
