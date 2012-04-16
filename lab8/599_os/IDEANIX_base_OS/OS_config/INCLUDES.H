/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                           All Rights Reserved
*
*                                           MASTER INCLUDE FILE
*********************************************************************************************************
*/

/*
*********************************************************************************************************
* Keil C port.
* By:  Junmin Zheng (zhengjunm@263.net) 
*********************************************************************************************************
*/

#ifndef __INCLUDES__
#define __INCLUDES__
     	
#ifndef C8051F040
#define C8051F040
//#include <C8051F040.H>
#include "C8051F040.H"
#endif

#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>


/* These macros are intended to be used to re-declare the uC/OS-II functions to be reentrant */
#define TASK_REENTRANT		large reentrant
#define KCREENTRANT			large reentrant


#include    "..\OS_port\os_cpu.h"
#include    "..\OS_config\os_cfg.h"

/* These Keil keywords are used in uC/OS-II source files. So the next macro definitions are needed 
*/
#define data	ucos51_data
#define pdata	ucos51_pdata
#define xdata	ucos51_xdata

#include    "..\OS_source\ucos_ii.h"


#endif
