//Edward Poore and Danny Peters
//Pin 1.0 is the pin toggled in main
  
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>
#include "init3.h"                    // defines for the init functions



unsigned char in_char; //global inbyte
  
  
void main(void){

  
  WDTCN = 0xde;                    // disable watchdog timer
  WDTCN = 0xad;
  SYSCLK_Init ();                     // initialize oscillator
  UART1_Init ();                      // initialize UART1
  XBR0 = 0x00;                     // Route UART to GPIO
  XBR1 = 0x00;
  XBR2 = 0xC4;                     // Enable crossbar
  P1MDOUT = 0x01;                  //1.0 is push pull to show on the scope
  EIE2 = 0x40;                     //enable interrupts from UART1
    
  IE = 0x80; //enable interrupts



  






  
  
}; //main    
   
   
  

void isr_uart1(void) interrupt 20 {








}; //isr_uart1






  