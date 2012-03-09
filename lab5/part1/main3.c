/*
 * Experiment 5
 * Written by Edward Poore & Danny Peters
 * March 9, 2012
 * Used for generating various interrupts.
 * Pin 1.0 -> Pin 0.2
 * Pin 1.1 -> Pin 0.3
 *
 * This code will solve problems 1 and 2 in the experiment.
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

unsigned int longInt = 0; //flag to cause a long latecy interrupt
float x = 5.0; //variable to divide


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
   //IP = 0x04; //set int1 priority to high

   zero = 0x01; //pin is initially high
   one = 0x01; //init

   IE = 0x85; //enable interrupts

   printf ("\nEE587 Experiment #5\n");
   printf ("Initialization Complete...\n");
   printf ("Press h for help...\n");

   while (1) {

       printf("What would you like to do? (l,s,h):");
       keypress = 'l'; //getchar();

       printf("\n");
       switch(keypress) {

           case ' ':;
           case 's': //short interrupt = first interrupt with longInt = 0
               zero = 0x00;
               break;

           case 'l': //long interrupt = interrupt -> trigger second interrupt -> return to a divide -> second interrupt
               longInt =1; //set the flag to cause a long interrupt
               zero = 0x00; //trigger first interrupt
               x=5.0/x; //a division for the zero interrupt to return to before the one interrupt is called
               break;

           case 'h': //help
               printf ("Press l to cause a long latency interrupt\n");
               printf ("Press s to cause a short latency interrupt\n");
               break;

           default:
               printf("Invalid Command '%c'\n",keypress);
               break;
       }
    }  // while(1)
}  // main


void isr_zero(void) interrupt 0 //interrupt function for int0
{
   //int i;
   //for (i=0;i<1000000;i++); //delay
   zero = 0x01; //clear the interrupt
   printf ("Interrupt zero called.\n");
   if(longInt > 0)
   {
        one = 0x00; //cause an interrupt on the second line (long latency)
   }
}

void isr_one(void) interrupt 2 //interrupt function for int1
{
  one = 0x01; //clear the interrupt
  printf ("Interrupt one called.\n");
  longInt = 0; //reset longInt
}








