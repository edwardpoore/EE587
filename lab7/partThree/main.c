//Edward Poore and Danny Peters

/*
  Operation---
  Prompt user for a blink rate then set that rate.
*/
  
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>
#include "init3.h"                    // defines for the init functions

unsigned int counter = 0;
unsigned int measure = 0;
unsigned int time0;
unsigned int time1;
unsigned int width = 0;

void main(void){
 
  WDTCN = 0xde;                    // disable watchdog timer
  WDTCN = 0xad;
  SYSCLK_Init ();                     // initialize oscillator
  UART0_Init ();                      // initialize UART0
  XBR0 = 0x14;                     // Route only uart0
  XBR1 = 0x00;
  XBR2 = 0xC0;                     // Enable crossbar without weak pullups

  PCA0CN    = 0x40;
  PCA0MD    = 0x09;
  PCA0CPM0 = 0x21; //module 0 catches rising edges
  PCA0CPM1 = 0x11; //module 1 catches falling edges
  EIE1 = 0x08; //enable CF interrupts
  IE = 0x80; //enable interrupts globally, UART0

  while(1){}; //spin forever
  
    
} //main    
   

void isr_pca(void) interrupt 9 {
    float display = 0.0;

    if(CCF0){
        time0 = PCA0CPL0 & 0x00FF;
        time0 = time0 | (PCA0CPH0 << 8);
        measure = 1;
    } //module 0

    if(CCF1){
        if(measure){
        time1 = PCA0CPL1 & 0x00FF;
        time1 = time1 | (PCA0CPH1 << 8);
        width = time1-time0;
        measure = 0;
        }
    } //module 1
  
    if(CF){
        display = width*0.045548654;
        //printf("%u \n", width);
        printf("%f microseconds\n",display);
        }


  PCA0CN = 0x40; //reset the interrupt flag
} //isr_pca



  