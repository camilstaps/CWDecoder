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

#define NR_ROWS 2
#define NR_COLS 40
#define NR_DISPLAYS 2
static unsigned char col = 0;
static unsigned char row = 0;
static unsigned char display = 0;
static unsigned char shift = 0;

#if NR_DISPLAYS==2 && NR_ROWS==2 && NR_COLS==40
static char display_0[NR_COLS];
static char display_1[NR_COLS];
static char display_2[NR_COLS];
static char display_3[NR_COLS];

inline void set_display_char(
		unsigned char disp, unsigned char row, unsigned char col, char x)
{
	switch ((disp * NR_ROWS + row + shift) % (NR_DISPLAYS * NR_ROWS))
	{
		case 0: display_0[col] = x; break;
		case 1: display_1[col] = x; break;
		case 2: display_2[col] = x; break;
		case 3: display_3[col] = x;
	}
}

inline char get_display_char(
		unsigned char disp, unsigned char row, unsigned char col)
{
	switch ((disp * NR_ROWS + row + shift) % (NR_DISPLAYS * NR_ROWS))
	{
		case 0: return display_0[col];
		case 1: return display_1[col];
		case 2: return display_2[col];
		case 3: return display_3[col];
	}
	return '^';
}
#endif

#define LCD_SET_ADDRESS(row,col) (hd44780_set_address((row) * 40 + col))

void shift_display(void)
{
	unsigned char display_i, col_i, row_i;

	if (++shift >= NR_DISPLAYS * NR_ROWS)
		shift = 0;

	for (display_i = 0; display_i < NR_DISPLAYS; display_i++) {
		hd44780_device = display_i;
		hd44780_clear();
		for (row_i = 0; row_i < NR_ROWS; row_i++) {
			LCD_SET_ADDRESS(row_i, 0);
			for (col_i = 0; col_i < NR_COLS; col_i++) {
				hd44780_put_char(get_display_char(display_i, row_i, col_i));
			}
		}
	}

	hd44780_device = NR_DISPLAYS-1;
	LCD_SET_ADDRESS(NR_ROWS-1, 0);
	for (col_i = 0; col_i < NR_COLS; col_i++)
		hd44780_put_char(' ');
}

void print(unsigned char x)
{
	hd44780_put_char(x);
	set_display_char(display, row, col, x);

	if (++col >= NR_COLS) {
		col = 0;
		if (++row >= NR_ROWS) {
			row = 0;
			if (++display >= NR_DISPLAYS) {
				shift_display();
				display = NR_DISPLAYS-1;
				row = NR_ROWS-1;
			}
			hd44780_device = display;
		}
		LCD_SET_ADDRESS(row, col);
	}
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
	init();

	hd44780_device = 0;
	hd44780_set_address(0x00);

	while (1)
		morse_process(PORTBbits.RB2);
}
