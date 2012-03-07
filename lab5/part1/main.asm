/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////
#include <compiler_defs.h>
#include <C8051F020_defs.h>   

// Peripheral specific initialization functions,
// Called from the Init_Device() function
void Reset_Sources_Init()
{
    WDTCN     = 0xDE;
    WDTCN     = 0xAD;
}

void Port_IO_Init()
{
    // P0.0  -  TX0 (UART0), Open-Drain, Digital
    // P0.1  -  RX0 (UART0), Open-Drain, Digital
    // P0.2  -  INT0 (Tmr0), Push-Pull,  Digital
    // P0.3  -  INT1 (Tmr1), Push-Pull,  Digital
    // P0.4  -  Unassigned,  Open-Drain, Digital
    // P0.5  -  Unassigned,  Open-Drain, Digital
    // P0.6  -  Unassigned,  Open-Drain, Digital
    // P0.7  -  Unassigned,  Open-Drain, Digital

    // P1.0  -  Unassigned,  Open-Drain, Digital
    // P1.1  -  Unassigned,  Open-Drain, Digital
    // P1.2  -  Unassigned,  Open-Drain, Digital
    // P1.3  -  Unassigned,  Open-Drain, Digital
    // P1.4  -  Unassigned,  Open-Drain, Digital
    // P1.5  -  Unassigned,  Open-Drain, Digital
    // P1.6  -  Unassigned,  Open-Drain, Digital
    // P1.7  -  Unassigned,  Open-Drain, Digital

    // P2.0  -  Unassigned,  Open-Drain, Digital
    // P2.1  -  Unassigned,  Open-Drain, Digital
    // P2.2  -  Unassigned,  Open-Drain, Digital
    // P2.3  -  Unassigned,  Open-Drain, Digital
    // P2.4  -  Unassigned,  Open-Drain, Digital
    // P2.5  -  Unassigned,  Open-Drain, Digital
    // P2.6  -  Unassigned,  Open-Drain, Digital
    // P2.7  -  Unassigned,  Open-Drain, Digital

    // P3.0  -  Unassigned,  Open-Drain, Digital
    // P3.1  -  Unassigned,  Open-Drain, Digital
    // P3.2  -  Unassigned,  Open-Drain, Digital
    // P3.3  -  Unassigned,  Open-Drain, Digital
    // P3.4  -  Unassigned,  Open-Drain, Digital
    // P3.5  -  Unassigned,  Open-Drain, Digital
    // P3.6  -  Unassigned,  Open-Drain, Digital
    // P3.7  -  Unassigned,  Open-Drain, Digital

    P0MDOUT   = 0x0C;
    XBR0      = 0x04;
    XBR1      = 0x14;
    XBR2      = 0x40;
}

void Interrupts_Init()
{
    IE        = 0x85;

}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    Reset_Sources_Init();
    Port_IO_Init();
    Interrupts_Init();
}


void main()
{
  Init_Device();
  int x =0;  


  P0^3 = 0x1;//init
  P0^2 = 0x1;//init
  P0^2 = 0x0; //clear 0.2 to trigger an interrupt 
  x = 5/1; //divide instruction for the interrupt to come back to 


  while(1)
  {
   
   
   
    
  }
}

void first_ISR (void) interrupt 0
{
  P0^2 = 0x1; //reset the pin
  P0^3 = 0x0; //trigger the second interrupt
}

void second_ISR (void) interrupt 2
{
  P0^3 = 0x1;
}









