#include "SAM3U2C.h"
#include "main.h"
#include "led.h"
#include "button.h"

static void SystemSleep(void);

volatile uint32_t G_u32SystemTick = 0u;

#define MCK_HZ            4000000u

#define HEARTBEAT         PIO_PA31
#define LED_ALL           (RED | ORANGE | YELLOW | GREEN | CYAN | BLUE | PURPLE | WHITE)
#define BUTTON_ALL_A      BUTTON_A0_MSK
#define BUTTON_ALL_B      (BUTTON_B1_MSK | BUTTON_B2_MSK | BUTTON_B3_MSK)
#define ID_Msk(ID)        (0x1 << (ID))
#define IP_Msk(IP)        ((IP) << 4)

#define WATCHDOG_BONE()   WDT->WDT_CR = (WDT_CR_WDRSTT | WDT_CR_KEY_PASSWD)
#define HEARTBEAT_ON()    PIOA->PIO_CODR = PIO_PA31
#define HEARTBEAT_OFF()   PIOA->PIO_SODR = PIO_PA31

int main()
{
  //__disable_irq();
  
  PMC->PMC_PCER0 = ID_Msk(ID_PIOA) | ID_Msk(ID_PIOB);               /* Peripheral clock enabled for PIO A and B */
  PIOA->PIO_PER = BUTTON_ALL_A | HEARTBEAT;                         /* Enables PIO A control of selected pins */
  PIOB->PIO_PER = LED_ALL | BUTTON_ALL_B;           // | LCD_ALL    /* Enables PIO B control of selected pins */
  PIOA->PIO_OER = HEARTBEAT;                                        /* Sets selected PIO A pins as output; remaining enabled pins are inputs */
  PIOB->PIO_OER = LED_ALL;                          // | LCD_ALL;   /* Sets selected PIO B pins as output; remaining enabled pins are inputs */
  PIOA->PIO_PUDR = BUTTON_ALL_A | HEARTBEAT;                        /* Disables PIO A pull-ups for select pins */
  PIOB->PIO_PUDR = LED_ALL | BUTTON_ALL_B;          // | LCD_ALL    /* Disables PIO B pull-ups for select pins */
  PIOA->PIO_AIMER = BUTTON_ALL_A;                                   /* Enables additional PIO A detection modes for buttons */
  PIOB->PIO_AIMER = BUTTON_ALL_B;                                   /* Enables additional PIO B detection modes for buttons */
  PIOA->PIO_WPMR = PIO_WPMR_WPEN | PIO_WPMR_WPKEY(0x50494F);        /* Enables PIO A write protection */
  PIOB->PIO_WPMR = PIO_WPMR_WPEN | PIO_WPMR_WPKEY(0x50494F);        /* Enables PIO B write protection */
  
  User_Initialization();
  
  SysTick->LOAD = MCK_HZ / 1000 - 1;                                /* Sets SysTick to 1ms */
  SCB->SHP[11] = IP_Msk(1);                                         /* Sets SysTick exception priority to 1 */
  //NVIC->ICPR[0] = 0xFFFFFFFF;                                       /* Clear all pending interrupts */
  __enable_irq();                                                   /* Enables interrupts */          
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |                      /* Sets SysTick clock source as MCK, ... */
    SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;             /* ... enables Systick exception request, and activates the timer */
  
  while(1)
  {
    WATCHDOG_BONE();
    
    ButtonService();
    
    User_Application();
    
    HEARTBEAT_OFF();
    SystemSleep();
    HEARTBEAT_ON();
  }
}

static void SystemSleep(void)
{
  static uint32_t u32ExpectedSystemTick = 0;
  
  if (G_u32SystemTick != u32ExpectedSystemTick)
  {
    __disable_irq(); // write proper warning code to display error message to terminal
    while(1); 
  }
  
  while (G_u32SystemTick == u32ExpectedSystemTick)
    __WFI();
    
  u32ExpectedSystemTick++;
}

static void User_Undefined(void) { }
#pragma weak  User_Initialization = User_Undefined
#pragma weak     User_Application = User_Undefined