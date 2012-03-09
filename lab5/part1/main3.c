

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>


#include "init3.h"                    // defines for the init functions


sbit zero = P1^0;
sbit one = P1^1;

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void) 
{
   char keypress;

   WDTCN = 0xde;                    // disable watchdog timer
   WDTCN = 0xad;


   SYSCLK_Init ();                     // initialize oscillator

   UART0_Init ();                      // initialize UART0


   XBR0 = 0x04;                     // Route UART to GPIO
   XBR1 = 0x14;
   XBR2 = 0xC0;                     // Enable crossbar 
   P1MDOUT = 0x03;                  //1.0 and 1.1 are push pull
   //IP = 0x04; //set int1 priority to high

   zero = 0x01; //pin is initially high
   one = 0x01; 

   IE = 0x85; //enable interrupts



   printf ("\nEE587 Experiment #5\n");
   printf ("Initialization Complete...\n");
   zero = 0x00; //cause an interrupt

  
   while (1) {

       printf("what would you like to do? (d,b,r,w):");
    
       keypress = getchar();

       printf("\n");
        
    
       switch(keypress) {

           case ' ':;
            
           case 'i':
               zero = 0x00; //interrupt
               break;

           default:
               printf("Invalid Command '%c'\n",keypress);
               break;
       }
    
    
    
    }  // while(1)
    
}  // main


void isr_zero(void) interrupt 0
{
   printf ("Interrupt worked\n");
   zero = 0x01; //clear the interrupt

}


void isr_one(void) interrupt 2
{

}








