#include "SAM3U2C.h"
#include "main.h"
#include "led.h"
#include "button_private.h"

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
  
  /* Clock Initialization */
  
  /*
  Steps:
    -Set the MOSCXTEN bit, MOSCSEL bit and write startup time in MOSCXTST field in CKGR_MOR.
    -poll MOSCXTS bit in PMC_SR until it is on. Then move on.
    -poll MOSCSELS bit in PMC_SR to see when this change is done. Then move on.
    -write 1 to DIVA, 7 to MULA, an appropriate value to PLLACOUNT, and 1 to bit 29 in CKGR_PLLAR.
    -poll LOCKA bit in PMC_SR untill PLLA is running. Then move on.
    -Write 1 to FWS in EEFC_FMR for EEFC0
    -Write 1 to Pres in PMC_MCKR
    -poll MCKRDY bit in PMC_SR. then move on.
    -Write 2 to CSS in PMC_MCKR.
    -poll MCKRDY bin in PMC_SR. then move on.
    -clear MOSCRCEN bit in CKGR_MOR (be carefull not to overwrite previous state of CKGR_MOR) (MOSCRCS in PMC_SR should automatically be cleared indicating clock is off) (can this be safely done in first write)
    -write 4 to CSS and 1 to PRES in PMC_PCK[0]
    -write 1 to PCK0 bit in PMC_SCER
    
  Activate USB UTMI PLL and wait for stabalization.
    turn on and enable crystal oscillator
    
  
  Need to configure PIOA to enable peripheral B output on PA27
  Check LPM bit in PMC_FSMR (could prevent the processor from sleeping)
  Select between RC and Crystal slow lock: XTALSEL bit in SUPC_CR - default is RC oscillator 
  */
  
  PMC->CKGR_MOR = CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTST(240) | CKGR_MOR_KEY_PASSWD; // where does this value come from
  while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));
  PMC->CKGR_MOR = CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL | CKGR_MOR_KEY_PASSWD;
  while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));
  PMC->CKGR_PLLAR = CKGR_PLLAR_DIVA(1) | CKGR_PLLAR_MULA(7) | CKGR_PLLAR_PLLACOUNT(63) | CKGR_PLLAR_ONE; // where does this value come from
  while (!(PMC->PMC_SR & PMC_SR_LOCKA));
  EFC0->EEFC_FMR = EEFC_FMR_FWS(2);
  PMC->PMC_MCKR = PMC_MCKR_CSS_MAIN_CLK | PMC_MCKR_PRES_CLK_2;
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
  PMC->PMC_MCKR = PMC_MCKR_CSS_PLLA_CLK | PMC_MCKR_PRES_CLK_2;
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
  PMC->PMC_PCK[0] = PMC_PCK_CSS_MCK;
  PMC->PMC_SCER = PMC_SCER_PCK0;
  
  
  
  
  //SUPC->SUPC_CR = SUPC_CR_XTALSEL | SUPC_CR_KEY_PASSWD;             /* Selects 32.768kHz (external) crystal oscillator (instead of RC) for Slow Clock */
  
  
  
  /* End Clock Initialization */
  
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
  
  // Add PMC write protection (WPEN bit)
  
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