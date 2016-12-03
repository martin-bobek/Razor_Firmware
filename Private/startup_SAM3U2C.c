#include "SAM3U2C.h"

extern void CSTACK$$Limit;          /* Brings the stack base pointer into scope */
void __iar_program_start(void);     /* Declares IAR's entry point of execution */

const DeviceVectors __vector_table @ ".intvec" = 
{
  &CSTACK$$Limit,
  (void*)__iar_program_start,
  (void*)NMI_Handler,
  (void*)HardFault_Handler,
  (void*)MemManage_Handler,
  (void*)BusFault_Handler,
  (void*)UsageFault_Handler,
  0,
  0,
  0,
  0,
  (void*)SVC_Handler,
  (void*)DebugMon_Handler,
  0,
  (void*)PendSV_Handler,
  (void*)SysTick_Handler,
  (void*)SUPC_Handler,
  (void*)RSTC_Handler,
  (void*)RTC_Handler,
  (void*)RTT_Handler,
  (void*)WDT_Handler,
  (void*)PMC_Handler,
  (void*)EFC0_Handler,
  0,
  (void*)UART_Handler,
  0,
  (void*)PIOA_Handler,
  (void*)PIOB_Handler,
  0,
  (void*)USART0_Handler,
  (void*)USART1_Handler,
  (void*)USART2_Handler,
  0,
  (void*)HSMCI_Handler,
  (void*)TWI0_Handler,
  (void*)TWI1_Handler,
  (void*)SPI_Handler,
  (void*)SSC_Handler,
  (void*)TC0_Handler,
  (void*)TC1_Handler,
  (void*)TC2_Handler,
  (void*)PWM_Handler,
  (void*)ADC12B_Handler,
  (void*)ADC_Handler,
  (void*)DMAC_Handler,
  (void*)UDPHS_Handler,
};

static void Unused_Handler(void)
{
  __disable_irq();
  while(1);
}
#pragma weak         NMI_Handler = Unused_Handler
#pragma weak   HardFault_Handler = Unused_Handler
#pragma weak   MemManage_Handler = Unused_Handler
#pragma weak    BusFault_Handler = Unused_Handler
#pragma weak  UsageFault_Handler = Unused_Handler
#pragma weak         SVC_Handler = Unused_Handler
#pragma weak    DebugMon_Handler = Unused_Handler
#pragma weak      PendSV_Handler = Unused_Handler
#pragma weak     SysTick_Handler = Unused_Handler
#pragma weak        SUPC_Handler = Unused_Handler
#pragma weak        RSTC_Handler = Unused_Handler
#pragma weak         RTC_Handler = Unused_Handler
#pragma weak         RTT_Handler = Unused_Handler
#pragma weak         WDT_Handler = Unused_Handler
#pragma weak         PMC_Handler = Unused_Handler
#pragma weak        EFC0_Handler = Unused_Handler
#pragma weak        UART_Handler = Unused_Handler
#pragma weak        PIOA_Handler = Unused_Handler
#pragma weak        PIOB_Handler = Unused_Handler
#pragma weak      USART0_Handler = Unused_Handler
#pragma weak      USART1_Handler = Unused_Handler
#pragma weak      USART2_Handler = Unused_Handler
#pragma weak       HSMCI_Handler = Unused_Handler
#pragma weak        TWI0_Handler = Unused_Handler
#pragma weak        TWI1_Handler = Unused_Handler
#pragma weak         SPI_Handler = Unused_Handler
#pragma weak         SSC_Handler = Unused_Handler
#pragma weak         TC0_Handler = Unused_Handler
#pragma weak         TC1_Handler = Unused_Handler
#pragma weak         TC2_Handler = Unused_Handler
#pragma weak         PWM_Handler = Unused_Handler
#pragma weak      ADC12B_Handler = Unused_Handler
#pragma weak         ADC_Handler = Unused_Handler
#pragma weak        DMAC_Handler = Unused_Handler
#pragma weak       UDPHS_Handler = Unused_Handler