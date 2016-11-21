#include <xc.h>

#include "hd44780_settings.h"
#include "hd44780/hd44780.h"

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

char s1[] = "In the beginning was the Word, and the  Word was with God, and the Word was God.";
char s2[] = "He was with God in the beginning.       Through him all things were made; ...";

void main(void)
{
	char* ptr;
	unsigned int i;

	OSCCONbits.IRCF = 0x01; // 2 MHz

	hd44780_init();
	hd44780_device = 1;

	hd44780_device = 0;
	hd44780_set_address(0x00);
	for (ptr = s1; *ptr; ptr++)
		hd44780_put_char(*ptr);
	hd44780_device = 1;
	hd44780_set_address(0x00);
	for (ptr = s2; *ptr; ptr++)
		hd44780_put_char(*ptr);

	while(1);
}
