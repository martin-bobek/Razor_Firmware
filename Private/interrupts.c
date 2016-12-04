#include "SAM3U2C.h"
#include "button_private.h"

extern uint32_t G_u32SystemTick;

void SysTick_Handler(void)
{
  G_u32SystemTick++;
}

void PIOA_Handler(void)
{
  uint32_t u32InterruptSource = PIOA->PIO_ISR;
  
  if (u32InterruptSource & BUTTON_A0_MSK)
  {
    PIOA->PIO_IDR = BUTTON_A0_MSK;
    EdgeFlags.au8Btn[0] |= 1u;
  }
}

void PIOB_Handler(void)
{
  uint32_t u32InterruptSource = PIOB->PIO_ISR;
  
  if (u32InterruptSource & BUTTON_B1_MSK)
  {
    PIOB->PIO_IDR = BUTTON_B1_MSK;
    EdgeFlags.au8Btn[1] |= 1u;
  }
  if (u32InterruptSource & BUTTON_B2_MSK)
  {
    PIOB->PIO_IDR = BUTTON_B2_MSK;
    EdgeFlags.au8Btn[2] |= 1u;
  }
  if (u32InterruptSource & BUTTON_B3_MSK)
  {
    PIOB->PIO_IDR = BUTTON_B3_MSK;
    EdgeFlags.au8Btn[3] |= 1u;
  }
}