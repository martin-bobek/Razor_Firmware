#ifndef BUTTON_INTERRUPT_H
#define BUTTON_INTERRUPT_H

static volatile union {
  uint8_t au8Btn[4];
  uint32_t u32All;
} EdgeFlags;

#define BUTTON_A0_MSK       PIO_PA17
#define BUTTON_B1_MSK       PIO_PB0
#define BUTTON_B2_MSK       PIO_PB1
#define BUTTON_B3_MSK       PIO_PB2

#endif