#ifndef _HD44780_SETTINGS_H_
#define _HD44780_SETTINGS_H_

#include <xc.h>
#include "timer.h"

/* 2ms would be something between 1 and 2; 3 is safe */
#define HD44780_DELAY_1640US() (delay(3))

#define HD44780_4BIT
#define HD44780_DUAL

#define HD44780_DATA PORTA
#define HD44780_DATA_TRIS TRISA
#define HD44780_DATA_SHIFT(x) (x & 0x0f)
#define HD44780_DATA_R7(x) (x & 0x08)

#define HD44780_RS PORTBbits.RB4
#define HD44780_RS_TRIS TRISBbits.TRISB4
#define HD44780_RW PORTBbits.RB3
#define HD44780_RW_TRIS TRISBbits.TRISB3

#define HD44780_E1 PORTBbits.RB0
#define HD44780_E1_TRIS TRISBbits.TRISB0
#define HD44780_E2 PORTBbits.RB1
#define HD44780_E2_TRIS TRISBbits.TRISB1

#endif
