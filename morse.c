#include "morse.h"

#define MORSE_TIME_BOUNCE 1

static unsigned char dit_or_dah;
static unsigned char character_done;
static unsigned char printed_space;

static unsigned long int time_down;
static unsigned long int time_up;

static unsigned long int start_down_time;
static unsigned long int start_up_time;

static unsigned short int dit_time;
static unsigned short int avg_dah_time;

static unsigned char current_char;

void morse_init(void)
{
	dit_or_dah = 1;
	character_done = 1;
	printed_space = 1;

	time_down = 0;
	time_up = 0;

	start_down_time = 0;
	start_up_time = 0;

	dit_time = 10;
	avg_dah_time = 240;

	current_char = 0;
}

void morse_shift_bits(void)
{
	if (time_down < dit_time / 3)
		return;

	current_char <<= 1;
	dit_or_dah = 1;

	if (time_down < dit_time) {
		current_char++;
	} else {
		avg_dah_time = (time_down + avg_dah_time) / 2;
		dit_time = (avg_dah_time / 3) * 2;
	}
}

void morse_print_character(void)
{
	printed_space = 0;

	if (current_char > 63) {
		switch (current_char) {
			case 0x47: MORSE_PRINT(':'); break;
			case 0x4c: MORSE_PRINT(','); break;
			case 0x52: MORSE_PRINT(')'); break;
			case 0x54: MORSE_PRINT('!'); break;
			case 0x55: MORSE_PRINT(';'); break;
			case 0x5e: MORSE_PRINT('-'); break;
			case 0x61: MORSE_PRINT('\''); break;
			case 0x65: MORSE_PRINT('@'); break;
			case 0x6a: MORSE_PRINT('.'); break;
			case 0x73: MORSE_PRINT('?'); break;
			case 0x7a: MORSE_PRINT('s'); MORSE_PRINT('k'); break;
			case 0xf6: MORSE_PRINT('$'); break;
			default:  MORSE_PRINT('#'); break;
		}
	} else if (current_char == 0x35) {
		MORSE_PRINT('a'); MORSE_PRINT('r');
	} else {
		MORSE_PRINT(morse_code[current_char]);
	}
}

void morse_print_space(void)
{
	if (printed_space)
		return;
	printed_space = 1;

	MORSE_PRINT(' ');
}

void morse_process(char audio_present)
{
	if (!audio_present) {
		start_up_time = 0;
		if (start_down_time == 0)
			start_down_time = MORSE_TIMER();
		character_done = 0;
		dit_or_dah = 0;
		MORSE_DELAY(MORSE_TIME_BOUNCE);
		if (current_char == 0)
			current_char = 1;
	} else {
		if (start_up_time == 0)
			start_up_time = MORSE_TIMER();
		time_up = MORSE_TIMER() - start_up_time;
		if (time_up < 10)
			return;
		if (time_up > avg_dah_time * 2)
			morse_print_space();
		if (start_down_time > 0) {
			time_down = MORSE_TIMER() - start_down_time;
			start_down_time = 0;
		}
		if (!dit_or_dah)
			morse_shift_bits();
		if (!character_done) {
			if (time_up > dit_time) {
				morse_print_character();
				character_done = 1;
				current_char = 0;
			}
			time_down = 0;
		}
	}
}
