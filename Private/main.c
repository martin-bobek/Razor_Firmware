#include "main.h"
#include "led.h"
#include "button.h"

#define WATCHDOG_BONE() WDT->WDT_CR = (WDT_CR_WDRSTT | WDT_CR_KEY_PASSWD)
#define HEARTBEAT_ON() PIOA->PIO_CODR = PIO_PA31
#define HEARTBEAT_OFF() PIOA->PIO_SODR = PIO_PA31

int main()
{
  // system initializations
  
  User_Initialization();
  
  // initialize systick
  
  while(1)
  {
    WATCHDOG_BONE();
    
    // system update operations
    ButtonService();
    
    LedOn(RED);
    LedOff(BLUE);
    LedToggle(GREEN);
    
    WasButtonPressed(BUTTON_0);
    IsButtonPressed(BUTTON_1);
    ButtonAcknowledge(BUTTON_3);
    
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