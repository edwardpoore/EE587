/*
*********************************************************************************************************
*                                          
*									        EE599 F07 Example 
*                               (c) Copyright 2007, University of Kentucky
*                                         
*
*                                              
*********************************************************************************************************
*/

#include "OS_main.h"
#include "..\ideanix\ideanix_init.h"
#include "..\ideanix\ideanix_isr.h"
#include <stdio.h>
sbit  LED = P1^6;                         // green LED: '1' = ON; '0' = OFF



/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

// OS task definitions of stacks and such
	#define TASK_STK_SIZE 512
	#define NUM_TASKs   4
// stack for tasks      
OS_STK           TaskStk[NUM_TASKs][TASK_STK_SIZE];     /* Tasks stacks                                  */



/*********************************************************************************************************/
//                                              OS EVENTS                           
/*********************************************************************************************************/


// Semaphores
OS_EVENT         *UARTSem;
OS_EVENT 		 *MYSem;


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void TaskFirst_task (void *dataa) KCREENTRANT;

void  task1(void *dataa) KCREENTRANT;
void  task2(void *dataa) KCREENTRANT; 
void  task3(void *dataa) KCREENTRANT;


/*****************************************  QUEUES  ***************************************************/  

OS_EVENT *ANGRY_BOB_Q;
OS_EVENT *CALM_BOB_Q;

void *ANGRY_Q[10];
void *CALM_Q[10];
/******************************************* End of Queue    *******************************************/

/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/



void main (void) 
{
  	unsigned char error_ck =0 ; 

	   EA = 0;   						   // disable global interrupts 
	
	   WDTCN = 0xde;                       // disable watchdog timer
	   WDTCN = 0xad;
	
	   init_all ();
	   OSInit();   /* Initialize uC/OS-II*/
	 
	
// IT is important to note as is stated on page of 484 of the uC OS II book under the OSTaskCreate() section
//ONE SHOULD NOT USE task priorities 0,1,2,3,OS_LOWEST_PRIO-3, OS_LOWEST_PRIO -2, OS_LOWEST_PRIO-1, OS_LOWEST_PRIO because they are reserved
//for use by uC/OS-II.  

//THEREFORE 4 is the highest priority task allowable by a USER THUS : 

		error_ck=   OSTaskCreate(task1, (void *) 0,&TaskStk[0][TASK_STK_SIZE], 4);
		error_ck=   OSTaskCreate(task2, (void *) 0,&TaskStk[1][TASK_STK_SIZE], 5);
		error_ck=   OSTaskCreate(task3, (void *) 0,&TaskStk[2][TASK_STK_SIZE], 6);


 
// DECLARE  OS EVENT OBJECTS 

		///////////////////////////////  SEMAPHORES ////////////////////////////////////////////

	   		UARTSem  = OSSemCreate(1);    // create a UART semaphore
			MYSem= OSSemCreate(1);
		///////////////////////////  END OF SEMAPHORE DECLARATIONS ///////////////////////////
	
		/////////////////////////////// QUEUE DECLARATIONS ///////////////////////////////////
			ANGRY_BOB_Q = OSQCreate(&ANGRY_Q[0],10);
			CALM_BOB_Q  = OSQCreate(&CALM_Q[0],10);

		/////////////////////////////END OF QUEUE DECLARATIONS ///////////////////////////////

		EA = 1;     // enable global interrupts
	
		SFRPAGE=UART0_PAGE;
        LED = 0;
		printf("\f\r");
	   	printf("###################### STARTING OS ########################\r\n");   	
   		printf("#############   Welcome to EE599 F07 Example  ##############\r\n");
    	printf("######### The OS version %u is up!, All processes started #\r\n",OSVersion());
    	printf("###########################################################\r\n");
        LED = 1;



		// start OS
	   	OSStart();  // Start multitasking    

}

/*
*********************************************************************************************************
*                                              First (STARTUP) TASK
*********************************************************************************************************
*/





void  task1(void *dataa) KCREENTRANT
{
unsigned char error,sfrsave;

	for(;;)
		{
			
		
			    OSSemPend(MYSem,0,&error);

			    OSSemPend(UARTSem,0,&error);
					sfrsave = SFRPAGE;
					SFRPAGE=UART0_PAGE;
			      	printf("1");
                    SFRPAGE=sfrsave;
			    OSSemPost(UARTSem); 
			    OSSemPost(MYSem); 
	
   		

	   			OSTimeDly(2); 

		}
}

void  task2(void *dataa) KCREENTRANT
{
unsigned char error,sfrsave;
	
	for(;;)
		{
					
			OSSemPend(UARTSem,0,&error);
		    OSTimeDly(1); 

			OSSemPend(MYSem,0,&error);
				sfrsave = SFRPAGE;
				SFRPAGE=UART0_PAGE;		      	
				printf("2");
				SFRPAGE = sfrsave;
		    OSSemPost(MYSem); 
		    OSSemPost(UARTSem); 

   			OSTimeDly(5); 

		}
}

void  task3(void *dataa) KCREENTRANT
{
unsigned char error,sfrsave;

	dataa=dataa;

		for(;;)
		{

   					OSSemPend(UARTSem,0,&error);
					    sfrsave = SFRPAGE;
						SFRPAGE=UART0_PAGE;
				      	printf("super3");
						SFRPAGE = sfrsave;
				    OSSemPost(UARTSem); 			

  					OSTimeDly(250); 	
		}
}

