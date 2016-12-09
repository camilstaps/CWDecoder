#ifndef _H_MORSE_SPECIFIC
#define _H_MORSE_SPECIFIC

#include "decoder.h"
#include "timer.h"

#define MORSE_PRINT(x) (print(x))
#define MORSE_TIMER() (get_time())
#define MORSE_DELAY(x) (delay(x))

#endif
