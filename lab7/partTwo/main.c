//Edward Poore and Danny Peters

/*
  Operation---
  Prompt user for a blink rate then set that rate.
*/
  
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>
#include "init3.h"                    // defines for the init functions


unsigned int counter = 0; //counter to use in clock division


void main(void){
 
  WDTCN = 0xde;                    // disable watchdog timer
  WDTCN = 0xad;
  SYSCLK_Init ();                     // initialize oscillator
  UART0_Init ();                      // initialize UART0
  P74OUT = 0x08; //port 5 high bit is push pull
  XBR0 = 0x0C;                     // 
  XBR1 = 0x00;
  XBR2 = 0xC0;                     // Enable crossbar without weak pullups
  P5 = 0x0FF; //start P5 
  PCA0CN    = 0x00; //dont make the timer run
  PCA0MD    = 0x09;
  PCA0CPM0 = 0x21; //capture/compare module set
  
    
  EIE1 = 0x08; //enable CF interrupts
  IE = 0x80; //enable interrupts globally, UART0

  while(1); //spin forever
  
    
} //main    
   

void isr_pca(void) interrupt 9 {

  if(counter == 2){ //if the counter variable has reached the desired counter value
    counter = 0; //reset the counter
    if(P5 == 0x00){ //if the LED is off, turn it on
      P5 = 0x0FF; //set only the high 4 bits of P5
    }
    else{ //if the LED is on, or in an error state (not F0 or 00), turn it off
      P5 = 0x00;
    }
  }//if(counter == divider)
  else{
    counter = counter + 1;
  }
 
  PCA0CN = 0x00; //reset the interrupt flag
} //isr_pca



  