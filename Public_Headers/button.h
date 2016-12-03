#ifndef BUTTON_H
#define BUTTON_H

#include "SAM3U2C.h"

typedef enum { BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3 } Button_Type;

uint8_t WasButtonPressed(Button_Type button);
void ButtonAcknowledge(Button_Type button);
uint8_t IsButtonPressed(Button_Type button);

void ButtonService(void);

#endif