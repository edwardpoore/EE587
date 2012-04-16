; 
; Ported date:     MAY 29, 2002
; By:               Junmin Zheng, China, (zhengjunm@263.net)
; Target platform: Keil C51 V6.20, V6.21

; Revision:
;     Now uC/OS V2.00 has been supported by this port.
;     What I port is very similar to that of 80x86 in form.
;     Task switch functions are writen with assemble language.
;     The sequence of registers to save to hardware stack now has been re-arranged to conform to what Keil C does.
;     A decision was made that 

	NAME	OS_CPU_A_ASM

;#include	"reg52.h"

; ?C_XBP is the simulated external stack pointer in large mode, and I use four
;  bytes ?C_XBP to ?C_XBP+3 in this module ,so users can not use them in his functions.
    	
        PUBLIC  ?C_XBP
        PUBLIC  _?OSIntCtxSw 
	    PUBLIC	Stack
	    PUBLIC  _?OSCtxSw
	    PUBLIC  _?OSStartHighRdy
	
	    EXTRN   CODE(_?OSTaskSwHook)
    	EXTRN   XDATA(OSRunning)
        EXTRN   XDATA(OSTCBHighRdy)
 	    EXTRN   XDATA(OSPrioHighRdy)
	    EXTRN   XDATA(OSPrioCur)
        EXTRN   XDATA(OSTCBCur)

; LoadStack MACRO is the assembly code to load the highest priority 
; task context 	
      LoadStack MACRO
          LOCAL LOOPLS
          MOV   DPTR,#OSTCBHighRdy
          INC   DPTR
          MOVX  A,@DPTR
          MOV   ?C_XBP+2,A
          INC   DPTR
          MOVX  A,@DPTR
          MOV   ?C_XBP+3,A
          MOV   DPH,?C_XBP+2
          MOV   DPL,?C_XBP+3
          INC   DPTR
          MOVX  A,@DPTR
          MOV   ?C_XBP,A
          INC   DPTR
          MOVX  A,@DPTR
          MOV   ?C_XBP+1,A
          MOV   DPH,?C_XBP
          MOV   DPL,?C_XBP+1
          MOVX  A,@DPTR
          MOV   SP,A
          INC   DPTR
          MOV   R0,SP
          LOOPLS:
          MOVX  A,@DPTR
          MOV   @R0,A
          INC   DPTR
          DEC   R0
          CJNE  R0,#Stack-1,LOOPLS
          MOV   ?C_XBP,DPH
          MOV   ?C_XBP+1,DPL
	  ENDM

; SaveStack MACRO is the assembly code to save the current priority 
; task context 	
      SaveStack MACRO
	      LOCAL LOOPSS1,LOOPSS2
          MOV DPH,?C_XBP
          MOV DPL,?C_XBP+1 					
	      MOV	A, SP			
	      CLR	C				
	      SUBB	A, #Stack-1		
	      MOV	R0, A				
	      INC	A				
	      CLR	C
       	  XCH    A, DPL				
	      SUBB   A, DPL				
	      JNC	LOOPSS1
	      DEC	DPH
      LOOPSS1:
          MOV	DPL,A				
          MOV	?C_XBP,   DPH
	      MOV	?C_XBP+1, DPL					
		  MOV	A,SP
	      MOVX	@DPTR,  A				
     LOOPSS2:
	      INC	DPTR				
	      POP	ACC				
	      MOVX	@DPTR, A			
	      DJNZ  R0, LOOPSS2			
          MOV   DPTR,#OSTCBCur
          INC   DPTR
          MOVX  A,@DPTR
          MOV   ?C_XBP+2,A
          INC   DPTR
          MOVX  A,@DPTR
          MOV   ?C_XBP+3,A
          MOV   DPH,?C_XBP+2
          MOV   DPL,?C_XBP+3
          INC   DPTR
          MOV   A,?C_XBP
          MOVX  @DPTR,A
          INC   DPTR
          MOV   A,?C_XBP+1
          MOVX  @DPTR,A
	 ENDM

; LoadTCB MACRO is the assembly code to make OSTCBCur point to OSTCBHighRdy
     LoadTCB	MACRO   
        MOV   DPTR,#OSTCBHighRdy
        INC   DPTR
        MOVX  A,@DPTR
        MOV   ?C_XBP+2,A
        INC   DPTR
        MOVX  A,@DPTR
        MOV   ?C_XBP+3,A
        MOV   DPTR,#OSTCBCur
        INC   DPTR
        MOV   A,?C_XBP+2
        MOVX  @DPTR,A
        INC   DPTR
        MOV   A,?C_XBP+3
        MOVX  @DPTR,A 
	ENDM

; LoadTCB MACRO is the assembly code to make OSPrioCur equal to OSPrioHighRdy
  LoadPrio MACRO  
	   MOV   DPTR,#OSPrioHighRdy
	   MOVX  A,@DPTR
 	   MOV   DPTR,#OSPrioCur
	   MOVX  @DPTR,A
   ENDM

; The PUSHA now emulates the pushing sequence what Keil C does.
   PUSHA	MACRO
	   IRP	REG,	<ACC, B, DPH, DPL, PSW, 0, 1, 2, 3, 4, 5, 6, 7>
	   PUSH	REG
	   ENDM
	   ENDM

   POPA	MACRO
	   IRP	REG,	<7, 6, 5, 4, 3, 2, 1, 0, PSW, DPL, DPH, B, ACC>
	   POP	REG
       ENDM
       ENDM


; Declare the external stack pointer by ourself.
DT?C_XBP	SEGMENT	DATA
		RSEG	DT?C_XBP
?C_XBP:					
	DS	4
; Declare a label 'Stack' in the hardware stack segment so that we know where it begins.
?STACK	SEGMENT IDATA
	RSEG	?STACK
Stack:

; Task level context switch entry point, which is intended to be called by task gracefully. 

PR?OSCtxSw      SEGMENT CODE
	RSEG	PR?OSCtxSw
_?OSCtxSw:
	     PUSHA				
         PUSH  IE
       	 SaveStack	 
         LCALL _?OSTaskSwHook
         LoadTCB	 
         LoadPrio
         LoadStack
         POP   ACC
         RLC   A
         MOV   EA,C
         POPA
         RET		

; Interrupt level context switch entry point, which is intended to be called by task gracefully. 
PR?OSIntCtxSw      SEGMENT CODE
	RSEG	PR?OSIntCtxSw
_?OSIntCtxSw:
        MOV   A,SP
	    SUBB  A,#05H
    	MOV   SP,A
        SaveStack
  	    LCALL _?OSTaskSwHook
     	LoadTCB
	    LoadPrio
        LoadStack
	    POP   ACC
	    RLC   A
	    MOV   EA,C
        POPA
        RETI    

 
;The first Task level context switch entry point.
PR?OSStartHighRdy      SEGMENT CODE
	RSEG	PR?OSStartHighRdy
_?OSStartHighRdy:
        LCALL _?OSTaskSwHook
        
	    MOV   DPTR,#OSRunning
        MOV   A,#01H
        MOVX  @DPTR,A
        LoadStack
        POP   ACC
        RLC   A
        MOV   EA,C
        POPA
        RET     
END
