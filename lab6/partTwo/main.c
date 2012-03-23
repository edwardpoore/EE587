//Edward Poore and Danny Peters

  
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>
#include "init3.h"                    // defines for the init functions



unsigned char transmit = 0; //transmit flag
unsigned char status = 0x00; //holds status register info
sbit rReady = P1^0; //receiver ready line
unsigned int bufferPointer = 0x00; //pointer to places in the buffer
unsigned char buffer[64]; //character buffer
unsigned char buffCount = 0; 
unsigned int counter = 0x00;

void main(void){
 
  WDTCN = 0xde;                    // disable watchdog timer
  WDTCN = 0xad;
  SYSCLK_Init ();                     // initialize oscillator
  UART1_Init ();                      // initialize UART1
  XBR0 = 0x04;                     // Route UART to GPIO
  XBR1 = 0x00;
  XBR2 = 0xC4;                     // Enable crossbar
  P1MDOUT = 0x00;                  //1.0 is open drain
  EIE2 = 0x40;                     //enable interrupts from UART1
  IE = 0x80; //enable interrupts globally
  rReady = 0x01; //set the bit high so that it acts as an input
  


  while(1)
  {
    if(bufferPointer != 0x00 && rReady == 0x00 && buffCount > 0x00) //if there is data in the buffer and the ready line is low
    {
      SBUF1 = buffer[0]; //send the character
      transmit = 0x01;
      while(transmit == 0x01){} //wait for the data to be sent
    }
    
  }
    
} //main    
   

void isr_uart1(void) interrupt 20 {

  status = SCON1 & 0x03; // SCON1 & 0b00000011 to get TI1, RI1
  if(status == 0x01) //RI1 set
  {
    if(bufferPointer == 0)
      bufferPointer++;
    buffer[bufferPointer-1] = SBUF1; //get data from UART1 receive latch
    if(!(bufferPointer > 64))
      bufferPointer++;
      buffCount++;
    SCON1 = 0x50; //clear RI1
  }
  
  if(status == 0x02) //TX1 set
  {
    bufferPointer--; //decrement the pointer for the sent byte
    buffCount--;
    for(counter = 0x00; counter < 63; counter++)
    {   
        buffer[counter] = buffer[counter+1];
    }
    transmit = 0x00; //clear xmit flag
    SCON1 = 0x50; //clear TI1
  }

} //isr_uart1




  