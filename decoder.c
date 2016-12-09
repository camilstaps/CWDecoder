#include <xc.h>
#include <stdlib.h>

#include "decoder.h"
#include "timer.h"
#include "hd44780_settings.h"
#include "hd44780/hd44780.h"
#include "morse.h"

#pragma config BOREN=ON
#pragma config CCPMX=RB0
#pragma config CP=OFF
#pragma config CPD=OFF
#pragma config DEBUG=OFF
#pragma config FCMEN=ON
#pragma config FOSC=INTOSCIO
#pragma config IESO=OFF
#pragma config LVP=OFF
#pragma config MCLRE=ON
#pragma config PWRTE=ON
#pragma config WDTE=OFF
#pragma config WRT=OFF

char start_string[] = "Listening... ";

void print(unsigned char x)
{
	hd44780_put_char(x);
}

void init(void)
{
	/* System: 8MHz clock (2MIPS) */
	OSCCONbits.IRCF = 0x07;

	/* Turn off all peripherals */
	ANSEL = 0x00;
	ADCON0bits.ADON = 0;
	CMCON = 0x07;

	RCSTAbits.SPEN = 0;
	SSPCONbits.SSPEN = 0;

	/* The CW input */
	TRISBbits.TRISB2 = 1;

	timer_init();
	hd44780_init();
	morse_init();
}

void main(void)
{
	char* ptr;

	init();

	hd44780_device = 0;
	hd44780_set_address(0x00);
	for (ptr = start_string; *ptr; ptr++)
		hd44780_put_char(*ptr);

	while (1)
		morse_process(PORTBbits.RB2);
}
