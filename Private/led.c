#include "led.h"

void LedOn(Led_Type led_msk)
{
  PIOB->PIO_SODR = led_msk & LED_ALL;
}

void LedOff(Led_Type led_msk)
{
  PIOB->PIO_CODR = led_msk & LED_ALL;
}

void LedToggle(Led_Type led_msk)
{
  led_msk &= LED_ALL;
  Led_Type current_state = PIOB->PIO_ODSR;
  PIOB->PIO_CODR = led_msk & current_state;
  PIOB->PIO_SODR = led_msk & ~current_state;
}