// isr.c

// Analog to digital ISR to run through all channels 

#include "ideanix_isr.h"
#include "..\OS_config\INCLUDES.H"


 bit RI_0 = 0;
 bit RI_1 = 0;
 bit TI_0 = 0;
 bit TI_1 = 0;




//-------------------------------------------------------------------------------------


void UART0_ISR (void)  interrupt 4

{
	unsigned char SFRPAGE_SAVE  = SFRPAGE;

	OSIntEnter();

    SFRPAGE=UART0_PAGE;

   	if (RI0 == 1)
    {                   // handle receive function 
		RI_0 = 1;
    	RI0 = 0;                         // clear RX complete indicator
	}
	if( TI0 ==1 )
	{
		TI_0 = 1;
		TI0 = 0;
	}
	
	SFRPAGE=SFRPAGE_SAVE;
   
    OSIntExit();

}


void UART1_ISR (void)interrupt 20
{
	unsigned char SFRPAGE_SAVE = SFRPAGE;
	OSIntEnter();
    SFRPAGE=UART1_PAGE;
   	
	if (RI1 == 1) 
    {    RI_1 = 1;               // handle receive function 
	     RI1 = 0;                         // clear RX complete indicator
	} //if(RIO ==1)
//outer:
	if( TI1 ==1 )
	{
		TI_1 = 1;
		TI1 = 0;
	}
SFRPAGE =SFRPAGE_SAVE ;
    OSIntExit();
}


//-------------------------------------------------------------------------------------
#define XON  0x11
#define XOFF 0x13

char putchar (char c)  
{
    if (SFRPAGE == UART1_PAGE)
	{
		  if (RI1)  
			{
			    if (SBUF1 == XOFF)  
				{
				      do  
						{
					        RI1 = 0;
					        while (!RI1);
					    }
				      while (SBUF1 != XON);
				      RI1 = 0; 
			    }
		  	}
		  while (!TI_1);
		  TI_1 = 0;
		  return (SBUF1 = c);
	}
	else
	{
		  if (RI0)  
			{
				    if (SBUF0 == XOFF)  
					{
					      do  
							{
						        RI0 = 0;
						        while (!RI0);
						    }
					      while (SBUF0 != XON);
					      RI0 = 0; 
				    }
			  }
		  while (!TI_0);
		  TI_0 = 0;
		  return (SBUF0 = c);
	}
}

//FUNCTION which Keil uses as base function of scanf  

char _getkey (void)  
{
  char c;
    if (SFRPAGE == UART1_PAGE)
	{
		while(!RI_1)
		{;}
		 c= SBUF1;
		   RI_1 = 0;
		return (c);
	}
	else
	{
		while (!RI_0)
		{;}
		c = SBUF0;	
			RI_0 = 0;
		return (c);
	}

}





