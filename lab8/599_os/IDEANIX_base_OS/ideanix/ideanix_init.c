#include "../OS_config/INCLUDES.H"
#include "ideanix_init.h"
#include "stdio.h"



void init_all(void)
{
   	Port_IO_Init();
	SYSCLK_Init ();
	TimerInitiate();
 	UART0_init();
 	UART1_init(); 
   	WDTCN = 0xde; 	// disable watchdog timer
   	WDTCN = 0xad;


}

void Port_IO_Init(void)
{
	// This assigns the pins as follows
    // P0.0  -  TX0 (UART0), Open-Drain, Digital
    // P0.1  -  RX0 (UART0), Open-Drain, Digital
    // P0.2  -  SCK  (SPI0), Open-Drain, Digital
    // P0.3  -  MISO (SPI0), Open-Drain, Digital
    // P0.4  -  MOSI (SPI0), Open-Drain, Digital
    // P0.5  -  NSS  (SPI0), Open-Drain, Digital
    // P0.6  -  SDA (SMBus), Open-Drain, Digital
    // P0.7  -  SCL (SMBus), Open-Drain, Digital

    // P1.0  -  TX1 (UART1), Open-Drain, Digital
    // P1.1  -  RX1 (UART1), Open-Drain, Digital
  char SFRPAGE_SAVE = SFRPAGE; // Save Current SFR page
 	SFRPAGE   = CONFIG_PAGE;
    	XBR0 = 0x07;	// XBAR0: Initial Reset Value
		XBR1 = 0x00;	// XBAR1: Initial Reset Value
		XBR2 = 0x44;	// XBAR2: Initial Reset Value
    	XBR3 = 0x00;    // XBAR3: Initial Reset Value

	 SFRPAGE= SFRPAGE_SAVE;   
}

void SYSCLK_Init (void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

    SFRPAGE = 0x0F;
	OSCXCN = 0x00;	// EXTERNAL Oscillator Control Register	
    CLKSEL = 0x00;  // Oscillator Clock Selector
	OSCICN = 0x83;	// Internal Oscillator Control Register
						//SYSCLK derived from Internal Oscillator divided by 1.

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}



// Configured using timer2 
void UART0_init(void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
	unsigned int tmp =0;
	unsigned char high  =0 ;
	unsigned char low =0;
    unsigned long tmp2=0;

	SFRPAGE=UART0_PAGE;


    TMR2CF = 0x08;  // Timer 2 Configuration

  //my new code  	
	tmp2 = ((unsigned long)UART0_BAUDRATE)<<4;
	tmp = 65535-(SYSCLK/tmp2); 
  	high = (tmp >> 8) & 0x00ff;
  	low = (tmp & 0x00ff);
		
    RCAP2L = low;  // Timer 2 Reload Register Low Byte
    RCAP2H = high;
	
    TMR2L = 0x00;   // Timer 2 Low Byte	
    TMR2H = 0x00;   // Timer 2 High Byte	
    TMR2CN = 0x04;  // Timer 2 CONTROL	

   	SFRPAGE = 0x00;
    SADEN0 = 0x00;      // Serial 0 Slave Address Enable
    SADDR0 = 0x00;      // Serial 0 Slave Address Register
    SSTA0 |= 0x15;       // UART0 Status and Clock Selection Register
    SCON0 |= 0x50;       // Serial Port Control Register

    PCON |= 0x00;        // Power Control Register
	
   	TI0 = 1;  

	ES0 = 1; // enable UART0 interrupts

   SFRPAGE = SFRPAGE_SAVE;

}




//Configured using timer 1 
void UART1_init(void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = UART1_PAGE;
   SCON1   = 0x10;                     // SCON1: mode 0, 8-bit UART, enable RX

   SFRPAGE = TIMER01_PAGE;



   TMOD   &= ~0xF0;
   TMOD   |=  0x20;                    // TMOD: timer 1, mode 2, 8-bit reload


   if (SYSCLK/UART1_BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/UART1_BAUDRATE/2);
      CKCON |= 0x10;                   // T1M = 1; SCA1:0 = xx
   } else if (SYSCLK/UART1_BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/UART1_BAUDRATE/2/4);
      CKCON &= ~0x13;                  // Clear all T1 related bits
      CKCON |=  0x01;                  // T1M = 0; SCA1:0 = 01
   } else if (SYSCLK/UART1_BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/UART1_BAUDRATE/2/12);
      CKCON &= ~0x13;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/UART1_BAUDRATE/2/48);
      CKCON &= ~0x13;                  // Clear all T1 related bits
      CKCON |=  0x02;                  // T1M = 0; SCA1:0 = 10
   }

   TL1 = TH1;                          // initialize Timer1
   TR1 = 1;                            // start Timer1
   SFRPAGE = UART1_PAGE;
   TI1 = 1;                            // Indicate TX1 ready

   EIE2 = EIE2 | 0x40;
   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page

}

//-----------------------------------------------------------------------------
// TimerInitiate()
//-----------------------------------------------------------------------------
// timer0 for OS ticks. 
// 

void TimerInitiate(void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = TIMER01_PAGE;

	TCON 	|= 0x00;
	TMOD    |= 0x01;                     // TMOD: timer0, mode 1
	CKCON 	&= 0xF4;
	TH0		=  0xb0;
	TL0 	=  0x3f;
	ET0    = 1;                          // enable timer zero overflow interrupt
   	TR0    = 1;                          // start Timer0

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}

