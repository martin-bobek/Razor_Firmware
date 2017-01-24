#ifndef BUTTON_H
#define BUTTON_H

#include "SAM3U2C.h"

typedef enum { BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3 } Button_Type;

uint8_t WasButtonPressed(Button_Type button);
void ButtonAcknowledge(Button_Type button);
uint8_t IsButtonPressed(Button_Type button);


#define BUTTON_A0_MSK       PIO_PA17
#define BUTTON_B1_MSK       PIO_PB0
#define BUTTON_B2_MSK       PIO_PB1
#define BUTTON_B3_MSK       PIO_PB2

void ButtonService(void);

#endif