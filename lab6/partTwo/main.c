//Edward Poore and Danny Peters
//Lab 6 Part 2
//3-23-12

  
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>
#include "init3.h"                    // defines for the init functions



unsigned char transmit = 0; //transmit flag
unsigned char status = 0x00; //holds status register info
sbit rReady = P1^0; //receiver ready line (receiver is ready when this line is low)
unsigned int bufferPointer = 0x00; //pointer to keep track of the buffer stack
unsigned char buffer[64]; //character buffer
unsigned char buffCount = 0; //number of characters in the buffer
unsigned int counter = 0x00; //counter variable

void main(void){
 
  WDTCN = 0xde;                    // disable watchdog timer
  WDTCN = 0xad;
  SYSCLK_Init ();                     // initialize oscillator
  UART1_Init ();                      // initialize UART1
  XBR0 = 0x04;                     // Route both UARTS to GPIO such that UART1 is placed on P0.2 and P0.3
  XBR1 = 0x00;
  XBR2 = 0xC4;                     // Enable crossbar
  P1MDOUT = 0x00;                  //1.0 is open drain so that it can be used as an input
  EIE2 = 0x40;                     //enable interrupts from UART1
  IE = 0x80; //enable interrupts globally
  rReady = 0x01; //set the bit high so that it acts as an input (open drain)
  
  while(1)
  {
    if(bufferPointer != 0x00 && rReady == 0x00 && buffCount > 0x00) //if there is data in the buffer and the ready line is low
    {
      SBUF1 = buffer[0]; //send the first character in the buffer
      transmit = 0x01; //transmit flag
      while(transmit == 0x01){} //wait for the data to be sent
    }
    
  } //while(1)
    
} //main    
   

void isr_uart1(void) interrupt 20 {

  status = SCON1 & 0x03; // SCON1 & 0b00000011 to get TI1, RI1
  if(status == 0x01) //RI1 set (byte received)
  {
    if(bufferPointer == 0) //if the buffer is empty, go to first position
      bufferPointer++;
    buffer[bufferPointer-1] = SBUF1; //get data from UART1 receive latch and place it in the next slot in the buffer
    if(!(bufferPointer > 64)) //if the buffer is not full (64 characters)
      bufferPointer++; 
      buffCount++;
    SCON1 = 0x50; //clear RI1
  }
  
  if(status == 0x02) //TX1 set (byte has been sent)
  {
    bufferPointer--; //decrement the pointer for the sent byte
    buffCount--; //there is 1 less character in the buffer
    for(counter = 0x00; counter < 63; counter++) //shift all of the bytes in the buffer back one space to replace the sent byte at buffer[0]
    {   
        buffer[counter] = buffer[counter+1];
    }
    transmit = 0x00; //clear xmit flag
    SCON1 = 0x50; //clear TI1
  }

} //isr_uart1




  