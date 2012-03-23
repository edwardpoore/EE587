//Edward Poore and Danny Peters
//Pin 1.0 is the pin toggled in main
/*
  Operation---
  Receive a character on the uart
  RI1 interrupt runs, stores the character, and sets the flag to transmit
  Catch the flag.. place in_byte into the uart data register to begin xmit
  Wait till its done. When done TI1 interrupt will clear the transmit flag.
*/
  
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>
#include "init3.h"                    // defines for the init functions

unsigned char in_char = 'a'; // byte to hold received data
unsigned char status = 0x00; //holds status register info
sbit flip = P1^0; //bit to toggle in main
unsigned char transmit = 0x00; //transmit flag
unsigned int count;

void main(void){
 
  WDTCN = 0xde;                    // disable watchdog timer
  WDTCN = 0xad;
  SYSCLK_Init ();                     // initialize oscillator
  UART1_Init ();                      // initialize UART1
  XBR0 = 0x04;                     // Route both UARTS to GPIO such that UART1 ends up on P0.2 and P0.3
  XBR1 = 0x00;
  XBR2 = 0xC4;                     // Enable crossbar without weak pullups
  P1MDOUT = 0x01;                  //pin 1.0 is push pull to show on the scope
  EIE2 = 0x40;                     //enable interrupts from UART1
  IE = 0x80; //enable interrupts globally

  while(1)
  {
    flip = ~flip; //toggle the bit for the oscope
    if(transmit == 0x01) //when we receive a byte, send it back
    {
      for(count=0;count<50000; count++){} //delay
      SBUF1 = in_char; //transmit the byte. transmit begins when data is written to the register
      while(transmit == 0x01){} //wait for the transmit to finish (flag cleared in isr)
    }
  }
    
} //main    
   

void isr_uart1(void) interrupt 20 {

  status = SCON1 & 0x03; // SCON1 & 0b00000011 to get TI1, RI1 from hardware status register
  if(status == 0x01) //RI1 set (a byte has been received)
  {
    in_char = SBUF1; //get data from UART1 receive latch
    transmit = 0x01; //set the flag to transmit
    SCON1 = 0x50; //clear RI1
  }
  
  if(status == 0x02) //TI1 set (a byte has been sent)
  {
    transmit = 0x00; //clear the need to transmit flag because the xmit is done
    SCON1 = 0x50; //clear TI1
  }
} //isr_uart1




  