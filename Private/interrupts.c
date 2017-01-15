#include "SAM3U2C.h"

extern uint32_t G_u32SystemTick;


void SysTick_Handler(void)
{
  G_u32SystemTick++;
}