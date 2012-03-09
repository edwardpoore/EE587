/*
 * Experiment 5
 * Written by Edward Poore & Danny Peters
 * March 9, 2012
 * Used for generating various interrupts.
 * Pin 1.0 -> Pin 0.2
 * Pin 1.1 -> Pin 0.3
 *
 * This code will solve problem 3 in the experiment.
*/

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>
#include "init3.h"                    // defines for the init functions

//use p1.0 and p1.1 to trigger interrupts
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
   P1MDOUT = 0x03;                  //1.0 and 1.1 are push pull to set interrupts
   IP = 0x04; //set int1 priority to high

   zero = 0x01; //pin is initially high
   one = 0x01; //init

   IE = 0x85; //enable interrupts

   printf ("\nEE587 Experiment #5\n");
   printf ("Initialization Complete...\n");
   printf ("Press h for help...\n");

   while (1) {

       printf("What would you like to do? (i,h):");
       keypress = getchar();

       printf("\n");
       switch(keypress) {

           case ' ':;
           case 'i': // interrupt = first interrupt with second interrupt taking place during the first
               zero = 0x00;
               break;

           case 'h': //help
               printf ("Press i to trigger the interrupt chain\n");
               break;

           default:
               printf("Invalid Command '%c'\n",keypress);
               break;
       }
    }  // while(1)
}  // main


void isr_zero(void) interrupt 0 //interrupt function for int0
{
   zero = 0x01; //clear the interrupt
   printf ("Interrupt zero called.\n");
   one = 0x00; //cause the second interrupt
   printf ("Back in the first interrupt.\n");
}

void isr_one(void) interrupt 2 //interrupt function for int1
{
  one = 0x01; //clear the interrupt
  printf ("Interrupt one called.\n");
}








