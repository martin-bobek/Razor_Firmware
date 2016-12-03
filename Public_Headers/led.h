#ifndef LED_H
#define LED_H

#include "SAM3U2C.h"

typedef uint32_t Led_Type;

#define RED       PIO_PB20
#define ORANGE    PIO_PB15
#define YELLOW    PIO_PB17
#define GREEN     PIO_PB19
#define CYAN      PIO_PB16
#define BLUE      PIO_PB18
#define PURPLE    PIO_PB14
#define WHITE     PIO_PB13

#define LED_ALL   (RED | ORANGE | YELLOW | GREEN | CYAN | BLUE | PURPLE | WHITE)

void LedOn(Led_Type led_msk);
void LedOff(Led_Type led_msk);
void LedToggle(Led_Type led_msk);

#endif