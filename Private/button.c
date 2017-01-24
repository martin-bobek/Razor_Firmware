#include "button.h"

static uint32_t au32DebounceRegister[4] = { 0u };
static uint8_t u8ButtonFlags = 0u;

#define DEBOUNCE_PERIOD_MSK   (1u << 20)
#define DEBOUNCE_STABLE_MSK   ((1u << 1) | 1u)

#define BUTTON0_ACTIVE      (1u << 0)
#define BUTTON1_ACTIVE      (1u << 1)
#define BUTTON2_ACTIVE      (1u << 2)
#define BUTTON3_ACTIVE      (1u << 3)
#define BUTTON0_PRESSED     (1u << 4)
#define BUTTON1_PRESSED     (1u << 5)
#define BUTTON2_PRESSED     (1u << 6)
#define BUTTON3_PRESSED     (1u << 7)

uint8_t WasButtonPressed(Button_Type button)
{
  return (button < 4) ? u8ButtonFlags & (BUTTON0_PRESSED << button) : 0u;
}

void ButtonAcknowledge(Button_Type button)
{
  if (button < 4)
    u8ButtonFlags &= ~(BUTTON0_PRESSED << button);
}

uint8_t IsButtonPressed(Button_Type button)
{
  return (button < 4) ? u8ButtonFlags & (BUTTON0_ACTIVE << button) : 0u;
}

void ButtonService(void)
{
  static uint32_t u32CurrentEdges = 0;
  u32CurrentEdges |= PIOB->PIO_ISR << 1;
  u32CurrentEdges |= PIOA->PIO_ISR >> 17;
  
  for (uint8_t i = 0; i < 4; i++)
  {
    au32DebounceRegister[i] <<= 1;
    au32DebounceRegister[i] |= (u32CurrentEdges >> i) & 1u;  // perhaps shift u32CurrentEdges by 1 each time (this also clears it).
  }
  u32CurrentEdges = 0;
  
  if (((au32DebounceRegister[0] & (DEBOUNCE_PERIOD_MSK | DEBOUNCE_STABLE_MSK)) == DEBOUNCE_PERIOD_MSK)
      && !(((PIOA->PIO_PDSR >> 17) ^ u8ButtonFlags) & 1u))
  {
    au32DebounceRegister[0] = 0u;
    u8ButtonFlags ^= BUTTON0_ACTIVE;
    if (u8ButtonFlags & BUTTON0_ACTIVE)
    {
      u8ButtonFlags |= BUTTON0_PRESSED;
      PIOA->PIO_REHLSR = BUTTON_A0_MSK;
    }
    else
      PIOA->PIO_FELLSR = BUTTON_A0_MSK;
    if (!(((PIOA->PIO_PDSR >> 17) ^ u8ButtonFlags) & 1u))
      u32CurrentEdges |= BUTTON0_ACTIVE;
  }
  
  if (((au32DebounceRegister[1] & (DEBOUNCE_PERIOD_MSK | DEBOUNCE_STABLE_MSK)) == DEBOUNCE_PERIOD_MSK) 
      && !((PIOB->PIO_PDSR ^ (u8ButtonFlags >> 1)) & 1u))
  {
    au32DebounceRegister[1] = 0u;
    u8ButtonFlags ^= BUTTON1_ACTIVE;
    if (u8ButtonFlags & BUTTON1_ACTIVE)
    {
      u8ButtonFlags |= BUTTON1_PRESSED;
      PIOB->PIO_REHLSR = BUTTON_B1_MSK;
    }
    else
      PIOB->PIO_FELLSR = BUTTON_B1_MSK;
    if (!((PIOB->PIO_PDSR ^ (u8ButtonFlags >> 1)) & 1u))
      u32CurrentEdges |= BUTTON1_ACTIVE;
  }
  
  if (((au32DebounceRegister[2] & (DEBOUNCE_PERIOD_MSK | DEBOUNCE_STABLE_MSK)) == DEBOUNCE_PERIOD_MSK) 
      && !(((PIOB->PIO_PDSR >> 1) ^ (u8ButtonFlags >> 2)) & 1u))
  {
    au32DebounceRegister[2] = 0u;
    u8ButtonFlags ^= BUTTON2_ACTIVE;
    if (u8ButtonFlags & BUTTON2_ACTIVE)
    {
      u8ButtonFlags |= BUTTON2_PRESSED;
      PIOB->PIO_REHLSR = BUTTON_B2_MSK;
    }
    else
      PIOB->PIO_FELLSR = BUTTON_B2_MSK;
    if (!(((PIOB->PIO_PDSR >> 1) ^ (u8ButtonFlags >> 2)) & 1u))
      u32CurrentEdges |= BUTTON2_ACTIVE;
  }
       
  if (((au32DebounceRegister[3] & (DEBOUNCE_PERIOD_MSK | DEBOUNCE_STABLE_MSK)) == DEBOUNCE_PERIOD_MSK) 
      && !(((PIOB->PIO_PDSR >> 2) ^ (u8ButtonFlags >> 3)) & 1u))
  {
    au32DebounceRegister[3] = 0u;
    u8ButtonFlags ^= BUTTON3_ACTIVE;
    if (u8ButtonFlags & BUTTON3_ACTIVE)
    {
      u8ButtonFlags |= BUTTON3_PRESSED;
      PIOB->PIO_REHLSR = BUTTON_B3_MSK;
    }
    else
      PIOB->PIO_FELLSR = BUTTON_B3_MSK;
    if (!(((PIOB->PIO_PDSR >> 2) ^ (u8ButtonFlags >> 3)) & 1u))
      u32CurrentEdges |= BUTTON3_ACTIVE;
  }
}