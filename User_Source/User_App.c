#include "button.h"
#include "led.h"
#include "main.h"

void User_Application(void)
{
  Led_Type on = 0, toggle = 0;
  if (IsButtonPressed(BUTTON_0))
    on = WHITE;
  else
    LedOff(WHITE);
  if (WasButtonPressed(BUTTON_0))
  {
    ButtonAcknowledge(BUTTON_0);
    toggle = PURPLE;
  }
  
  if (IsButtonPressed(BUTTON_1))
    on |= BLUE;
  else
    LedOff(BLUE);
  if (WasButtonPressed(BUTTON_1))
  {
    ButtonAcknowledge(BUTTON_1);
    toggle |= CYAN;
  }
  
  if (IsButtonPressed(BUTTON_2))
    on |= GREEN;
  LedOff(GREEN);
  if (WasButtonPressed(BUTTON_2))
  {
    ButtonAcknowledge(BUTTON_2);
    toggle |= YELLOW;
  }
  
  if (IsButtonPressed(BUTTON_3))
    on |= ORANGE;
  else
    LedOff(ORANGE);
  if (WasButtonPressed(BUTTON_3))
  {
    ButtonAcknowledge(BUTTON_3);
    toggle |= RED;
  }
  
  LedOn(on);
  LedToggle(toggle);
}