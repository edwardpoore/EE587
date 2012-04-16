/*
*********************************************************************************************************
*                                          
* EE587 Dining Philosophers Example
* James Lumpp, 4/6/12
*                                         
*                       
*********************************************************************************************************
*/

#include "OS_main.h"
#include "..\ideanix\ideanix_init.h"
#include "..\ideanix\ideanix_isr.h"
#include <stdio.h>
sbit  LED = P1^6;                         // green LED: '1' = ON; '0' = OFF
typedef unsigned char uint8;



/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

// OS task definitions of stacks and such
	#define TASK_STK_SIZE 256
	#define NUM_PHILS   5            // MAKE SURE YOU CODE WORKS FOR EVEN NUMBERS AS WELL AS ODD

// stack for tasks      
OS_STK           TaskStk[NUM_PHILS][TASK_STK_SIZE];     /* Tasks stacks                                  */
uint8           TaskIds[NUM_PHILS];   //array of task "names"




/*********************************************************************************************************/
//                                              OS EVENTS                           
/*********************************************************************************************************/


// Semaphores
OS_EVENT         *UARTSem;
OS_EVENT         *ChopSticks[NUM_PHILS];



/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
//void TaskFirst_task (void *dataa) KCREENTRANT;

void  Philosopher(void *myid) KCREENTRANT;

void eat(uint8 me) KCREENTRANT;
void think(uint8 me) KCREENTRANT;
void pickup(uint8 me) KCREENTRANT;
void putdown(uint8 me) KCREENTRANT;



/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/




void main (void) 
{
  	unsigned char error_ck =0 ; 
	uint8 i;


	   EA = 0;   						   // disable global interrupts 
	
	   WDTCN = 0xde;                       // disable watchdog timer
	   WDTCN = 0xad;
	
	   init_all ();
	   OSInit();   /* Initialize uC/OS-II*/
	 
	
// It is important to note that as is stated on page 484 of the uC OS II book in the OSTaskCreate() section
// DO NOT USE task priorities 0,1,2,3,OS_LOWEST_PRIO-3, OS_LOWEST_PRIO -2, OS_LOWEST_PRIO-1, OS_LOWEST_PRIOi
// because they are reserved for use by uC/OS-II.  

//THEREFORE 4 is the highest priority task available to the user : 

  		for(i=0;i<NUM_PHILS;i++){
			TaskIds[i] = i;
			error_ck=OSTaskCreate(Philosopher,(void *)&TaskIds[i],&TaskStk[i][TASK_STK_SIZE],i+4);   // start with priority 4 (i+4 = 0+4 = 4)
  		}

// DECLARE  OS EVENT OBJECTS 

		///////////////////////////////  SEMAPHORES ////////////////////////////////////////////

	   	UARTSem  = OSSemCreate(1);    // create a UART semaphore

		for(i=0;i<NUM_PHILS;i++){
			ChopSticks[i] = OSSemCreate(1);
  		}

		///////////////////////////  END OF SEMAPHORE DECLARATIONS ///////////////////////////
	
		/////////////////////////////// QUEUE DECLARATIONS ///////////////////////////////////
//			ANGRY_Q = OSQCreate(&ANGRY_Q[0],10);
//			CALM_Q  = OSQCreate(&CALM_Q[0],10);

		/////////////////////////////END OF QUEUE DECLARATIONS ///////////////////////////////

	EA = 1;     // enable global interrupts
	
	SFRPAGE=UART0_PAGE;
        LED = 0;
	printf("\f\r");
	printf("###################### STARTING OS ########################\r\n");   	
   	printf("#############   Welcome to EE587 S12 Philosphers  #########\r\n");
    	printf("######### The OS version %u is up!, All processes started #\r\n",OSVersion());
    	printf("###########################################################\r\n");

        LED = 1;  // Show some sign of life

	// start OS
	OSStart();  // Start multitasking    
}

/*
*********************************************************************************************************
*                                              First (STARTUP) TASK
*********************************************************************************************************
*/






void Philosopher(void *myid) KCREENTRANT
{
uint8 me = *(uint8 *)myid;
unsigned char error,sfrsave;

    OSSemPend(UARTSem,0,&error);
    sfrsave = SFRPAGE;
    SFRPAGE=UART0_PAGE;
    printf("Philosopher %bd at your service... What's for dinner?\n\r",me);
    SFRPAGE=sfrsave;
    OSSemPost(UARTSem);

  while (1){
		
	think(me);  //get hungry
	pickup(me);  //pickup stick[myid] and stick[(myid+1)%NUM_PHILS]
	eat(me);   //get philosophical
	putdown(me); //pickup stick[myid] and stick[(myid+1)%NUM_PHILS]
  }

}
 


void eat(uint8 me) KCREENTRANT
{
unsigned int sleepnumber;
unsigned char error,sfrsave;

    sleepnumber =  ((5-me)*30); //could be sleepnumber = (rand()&0x00FF);  //mask off to number 0-255
    OSTimeDly(sleepnumber);
    OSSemPend(UARTSem,0,&error);  // get access to serial port 
    sfrsave = SFRPAGE;
    SFRPAGE=UART0_PAGE;
    printf("yum %bd",me);
    SFRPAGE=sfrsave;
    OSSemPost(UARTSem);           // releae serial port
}

void think(uint8 me) KCREENTRANT
{
unsigned char error,sfrsave;

    OSSemPend(UARTSem,0,&error);
    sfrsave = SFRPAGE;
    SFRPAGE=UART0_PAGE;
    printf("hm %bd",me);
    SFRPAGE=sfrsave;
    OSSemPost(UARTSem);

    OSTimeDly((5-me)*50);  // ((rand()&0x00FF));  //think for 0-255 ticks
    
}

void pickup(uint8 id) KCREENTRANT
{
//uint8 error;
//pick up stick id and id+1 modulo NUM_PHILS

//OSSemPend(ChopSticks(me),0,&error);
//OSSemPend(ChopSticks((me+1)%NUM_PHILS),0,&error);
}

void putdown(uint8 id) KCREENTRANT
{
//drop stick id and (id+1 modulo NUM_PHILS)
}

