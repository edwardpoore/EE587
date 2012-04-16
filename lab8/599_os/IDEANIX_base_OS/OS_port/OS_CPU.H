/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                          All Rights Reserved
*
*                                       Keil C51 6.20c specific code
*                                          LARGE MEMORY MODEL
*
* File : OS_CPU.H
* By   : Jean J. Labrosse
*
* Ported by:       John X. Liu (johnxliu@163.com)
* Now it used by Junmin Zheng (zhengjunm@263.net)for porting uC/OS-II 2.00 to 8051
* Target platform: Keil C51 V6
*********************************************************************************************************
*/

#ifndef __OS_CPU_H
#define __OS_CPU_H

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   int   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */

typedef unsigned char  OS_STK;                   /* Each stack entry is 8-bit wide                     */
typedef unsigned char  OS_CPU_SR;                /* Define size of CPU status register (PSW = 8 bits)  */

#define BYTE           INT8S                     /* Define data types for backward compatibility ...   */
#define UBYTE          INT8U                     /* ... to uC/OS V1.xx.  Not actually needed for ...   */
#define WORD           INT16S                    /* ... uC/OS-II.                                      */
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U

/* 
*********************************************************************************************************
*                              Keil C51 on generic 8051-based microcontroller
*********************************************************************************************************
*/

#define  OS_CRITICAL_METHOD    2

#if      OS_CRITICAL_METHOD == 1
#define  OS_ENTER_CRITICAL()  EA=0		    				/* Disable interrupts                            */
#define  OS_EXIT_CRITICAL()   EA=1			 				/* Enable  interrupts                            */
#endif

#if      OS_CRITICAL_METHOD == 2
/* As an undocumented keyword of keil c. __asm is supported in Keil C v6.20.
. No other means to define assemble language code in a macro, I have to use it here. If your compiler does not support __asm, use method 1 or 3 then. */
/* A2 AF MOV C, EA*/
/* C2 AF CLR EA   */
/* C0 D0 PUSH PSW */
#define  OS_ENTER_CRITICAL()    __asm DB 0A2H, 0AFH, 0C2H, 0AFH, 0C0H, 0D0H

/* D0 D0 POP PSW   */
/* 92 AF MOV EA, C */
#define  OS_EXIT_CRITICAL()     __asm DB 0D0H, 0D0H, 092H, 0AFH
#endif

#if      OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  (cpu_sr = EA, EA=0)    /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   (EA=cpu_sr)    /* Enable  interrupts                        */
#endif
#define  OS_STK_GROWTH        1                     		/* Stack grows from HIGH to LOW memory on for large mode */
#define  OS_TASK_SW()         OSCtxSw() 

//#define OS_ISR_PROTO_EXT	1
void OSCtxSw(void) KCREENTRANT;

#endif
