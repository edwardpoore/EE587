/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                          All Rights Reserved
*
*
*                                       80x86/80x88 Specific code
*                                          LARGE MEMORY MODEL
*
* File : OS_CPU_C.C
* By   : Jean J. Labrosse
*
*
* Ported date:     MAY 29, 2002
* By:              Junmin Zheng, China, (zhengjunm@263.net)
* Target platform: Keil C51 V6.20
*         
*********************************************************************************************************
*/

#define  OS_CPU_GLOBALS
#include "..\OS_config\includes.h"
#include "..\OS_port\os_kcdef.h"



/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then 
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : Interrupts are enabled when your task starts executing. You can change this by setting the
*              PSW to 0x0002 instead.  In this case, interrupts would be disabled upon task startup.  The
*              application code would be responsible for enabling interrupts at the beginning of the task
*              code.  You will need to modify OSTaskIdle() and OSTaskStat() so that they enable 
*              interrupts.  Failure to do this will make your system crash!
*********************************************************************************************************
*/

/* The stack variable points to the start pointer in hardware stack and is defined in OS_CPU_A */
extern idata unsigned char Stack[1];

OS_STK *OSTaskStkInit (void (*task)(void *pd) KCREENTRANT, void * vd, OS_STK *ptos, INT16U opt) KCREENTRANT
{   
	INT8U * stk;
    opt    = opt;                           /* 'opt' is not used, prevent warning                      */
    stk    = (INT8U *) ptos;                /* Load stack pointer                                      */



	stk				-= sizeof(INT16U);      /* The value should be loaded to PC    */
	*(INT16U*)stk	 = (INT16U) task;       /* next time when this task is running */

	stk				-= sizeof(INT16U);      /* The value should be loaded to PC    */
	*(INT16U*)stk	 = (INT16U) task;       /* next time when this task is running */

/* Following is the registers pushed into hardware stack */
	*--stk			 = 'A';                 /* ACC */
	*--stk			 = 'B';                 /* B   */
	*--stk			 = 'L';                 /* DPL */
	*--stk			 = 'H';                 /* DPH */
	*--stk			 = PSW;					/* PSW */
	*--stk			 = 0;                   /* R0  */

	stk				-= sizeof(void *);      /* Keil C uses R1,R2,R3 to pass the */
	*(void**)stk	 = vd;                  /* arguments of functions.          */

	*--stk			 = 4;                   /* R4  */
	*--stk			 = 5;                   /* R5  */
	*--stk			 = 6;                   /* R6  */
	*--stk			 = 7;                   /* R7  */

    *--stk           = 0x80;                /* IE, EA is enabled  */
/*
    Next is calculating the hardware stack pointer.
*/
    *--stk			 = (INT8U) Stack-1    /* Initial value when main was called    */
	                   +1                   /* IE */
	                   +8                   /* R0-R7, eight registers was saved      */
                       +5                   /* PSW, ACC, B, DPH, DPL, five registers */
                       +sizeof(INT16U)      /* The PC value to be loaded             */
                       +sizeof(INT16U)      /* The PC value to be loaded             */
					   ;

    return ((void *)stk);
}



/* OSTickISR can be writen in c language now, so it is more easy for user to write code for their own */
void OSTickISR(void) KCREENTRANT interrupt 1 
{
#pragma ASM
PUSH IE
#pragma ENDASM


OSIntEnter();
	SFRPAGE=TIMER01_PAGE;
	
OSTimeTick();
	TH0		=  0xb0;
	TL0 	=  0x3f;					
OSIntExit();

#pragma ASM
POP IE
#pragma ENDASM		
}

/* If you want to write ISRs for your own, just do as OSTickISR() */

/*$PAGE*/



/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void) KCREENTRANT
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void) KCREENTRANT
{
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskCreateHook (OS_TCB *ptcb)  KCREENTRANT
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskDelHook (OS_TCB *ptcb) KCREENTRANT
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif

/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
void  OSTaskIdleHook (void) KCREENTRANT
{
}
#endif

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0 
void  OSTaskStatHook (void) KCREENTRANT
{
}
#endif

/*$PAGE*/
/*

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskSwHook (void) KCREENTRANT
{
}
#endif

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb) KCREENTRANT
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTimeTickHook (void) KCREENTRANT
{
  char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
  SFRPAGE = CONFIG_PAGE;              // set SFR page
  SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}
#endif

