#ifndef BUTTON_INTERRUPT_H
#define BUTTON_INTERRUPT_H

#include "button.h"

void ButtonService(void);

typedef union {
  uint8_t au8Btn[4];
  uint32_t u32All;
} EdgeFlags_t;
extern volatile EdgeFlags_t EdgeFlags;

#define BUTTON_A0_MSK       PIO_PA17
#define BUTTON_B1_MSK       PIO_PB0
#define BUTTON_B2_MSK       PIO_PB1
#define BUTTON_B3_MSK       PIO_PB2

#define BUTTON_ALL_A        BUTTON_A0_MSK
#define BUTTON_ALL_B        (BUTTON_B1_MSK | BUTTON_B2_MSK | BUTTON_B3_MSK)

#endif