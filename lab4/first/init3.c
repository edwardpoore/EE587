//-----------------------------------------------------------------------------
// init3.c
//-----------------------------------------------------------------------------
// Initialization Routines for EE587 Experiment #3.
//
// Author: James E. Lumpp
// Date: 2/21/02
// Target: 8051F020
// Assignment: Experiment #3
//
#include <c8051f020.h>                 // SFR declarations



//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       22118400          // SYSCLK frequency in Hz
//#define SYSCLK       400000

#define BAUDRATE     9600              // Baud rate of UART in bps
// 16-bit SFR declarations
sfr16    TMR3RL   = 0x92;              // Timer3 reload registers
sfr16    TMR3     = 0x94;              // Timer3 counter registers

#define  SMB_FREQUENCY  200000L         // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz
#define  EEPROM_ADDR    0xA0           // Device address for slave target
                                       // Note: This address is specified
                                       // in the Microchip 24LC02B
                                       // datasheet.

#define  MY_ADDR        0x00           // Address of this SMBus device
                                       // (dummy value since this device does
                                       // not have any defined slave states)




//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to use an 22.1184MHz crystal
// as its clock source.
//
void SYSCLK_Init (void)
{
   int i;                              // delay counter

   OSCXCN = 0x67;                      // start external oscillator with
                                       // 22.1184MHz crystal

   for (i=0; i < 3000; i++) ;           // XTLVLD blanking interval (>1ms)

   while (!(OSCXCN & 0x80)) ;          // Wait for crystal osc. to settle

   OSCICN = 0x08;                      // select external oscillator as SYSCLK
                                       // source and enable missing clock
                                       // detector
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports
//
void PORT_Init (void)
{
   XBR0    = 0x05;                     // Enable UART0 and SMBUS
   XBR1    = 0x80;
   
   XBR2    = 0xC0;                     // Enable crossbar and weak pull-ups

}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Configure the UART0 using Timer1, for <baudrate> and 8-N-1.
//
void UART0_Init (void)
{
   SCON0   = 0x50;                     // SCON0: mode 1, 8-bit UART, enable RX
   TMOD    = 0x20;                     // TMOD: timer 1, mode 2, 8-bit reload
   TH1    = -(SYSCLK/BAUDRATE/16);     // set Timer1 reload value for baudrate
   TR1    = 1;                         // start Timer1
   CKCON |= 0x10;                      // Timer1 uses SYSCLK as time base
   PCON  |= 0x80;                      // SMOD00 = 1
   TI0    = 1;                         // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// SMBus_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// The SMBus peripheral is configured as follows:
// - SMBus enabled
// - Assert Acknowledge low (AA bit = 1b)
// - Free and SCL low timeout detection enabled
//
void SMBus_Init (void)
{
     SMB0CN = 0x41;
     SMB0CR = 0xD0;
}
//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer3 configured for use by the SMBus low timeout detect feature as
// follows:
// - SYSCLK/12 as Timer3 clock source
// - Timer3 reload registers loaded for a 25ms overflow period
// - Timer3 pre-loaded to overflow after 25ms
// - Timer3 enabled
//
void Timer3_Init (void)
{
   TMR3CN = 0x02;                      // Timer3 uses SYSCLK/12

   TMR3RL =  0xAF;           // Timer3 configured to overflow after
   TMR3RLH = 0xFD;
   EIE2 |=   0x01;                       // Timer3 interrupt enable
   TMR3CN |= 0x02;                     // Start Timer3
}
void Interrupts_Init(void)
{
   EIE1      = 0x02;    

}