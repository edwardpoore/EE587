//Edward Poore and Danny Peters
//Pin 1.0 is
/*
  Operation---
  Prompt user for a blink rate then set that rate.
*/
  
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>
#include "init3.h"                    // defines for the init functions

void main(void){
 
  WDTCN = 0xde;                    // disable watchdog timer
  WDTCN = 0xad;
  SYSCLK_Init ();                     // initialize oscillator
  UART0_Init ();                      // initialize UART0
  XBR0 = 0x00;                     // Route only uart0
  XBR1 = 0x00;
  XBR2 = 0xC4;                     // Enable crossbar without weak pullups
  P1MDOUT = 0x01;                  //pin 1.0 is push pull to show on the scope
  IE = 0x90; //enable interrupts globally, UART0
  
  
  printf ("\nEE587 Experiment #5\n");
  printf ("Initialization Complete...\n");
  printf ("Press h for help...\n");
    
} //main    
   

void isr_uart0(void) interrupt 4 {


} //isr_uart0




  