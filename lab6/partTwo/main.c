//Edward Poore and Danny Peters
//Pin 1.0 is the pin toggled in main
  
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>
#include "init3.h"                    // defines for the init functions



unsigned char in_char = 'i'; //inbyte
unsigned char out_char = 'o'; //outbyte
unsigned char status = 0x00; //holds status register info
sbit rReady = P1^0; //receiver ready line

void main(void){
 
  WDTCN = 0xde;                    // disable watchdog timer
  WDTCN = 0xad;
  SYSCLK_Init ();                     // initialize oscillator
  UART1_Init ();                      // initialize UART1
  XBR0 = 0x00;                     // Route UART to GPIO
  XBR1 = 0x00;
  XBR2 = 0xC4;                     // Enable crossbar
  P1MDOUT = 0x00;                  //1.0 is open drain
  EIE2 = 0x40;                     //enable interrupts from UART1
  IE = 0x80; //enable interrupts globally
  rReady = 0x01; //set the bit high so that it acts as an input
  

  unsigned char chaReady = 0x00; //flag to store a character
  unsigned int bufferPointer = 0x00; //pointer to places in the buffer
  unsigned char buffer[64]; //character buffer
  while(1)
  {
    if(chaReady = 0x01)
    {
      
    
  
  
  
  
  }
    
}; //main    
   

void isr_uart1(void) interrupt 20 {

  status = SCON1 & 0x03; // SCON1 & 0b00000011 to get TI1, RI1
  if(status == 0x01) //RI1 set
  {
    in_char = SBUF1; //get data from UART1 receive latch
    SCON1 = 0x50; //clear RI1
  }
  
  if(status == 0x02) //TX1 set
  {
    SBUF1 = out_char; //place data in the transmit buffer
    SCON1 = 0x50; //clear TI1
  }

}; //isr_uart1




  