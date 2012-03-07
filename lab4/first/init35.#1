/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////

#include <c8051f020.h>                 // SFR declarations


// Peripheral specific initialization functions,
// Called from the Init_Device() function
void Timer_Init()
{
    CKCON     = 0x10;
    TMOD      = 0x20;
    TH1       = 0xF3;
    TMR3CN    = 0x04;
    TMR3RLL   = 0x72;
    TMR3RLH   = 0xDF;
}

void UART_Init()
{
    SCON0     = 0x50;
}

void SMBus_Init()
{
    SMB0CN    = 0x41;
    SMB0CR    = 0xEE;
}

void Port_Init()
{
    // P0.0  -  TX0 (UART0), Open-Drain, Digital
    // P0.1  -  RX0 (UART0), Open-Drain, Digital
    // P0.2  -  SDA (SMBus), Open-Drain, Digital
    // P0.3  -  SCL (SMBus), Open-Drain, Digital
    // P0.4  -  SYSCLK,      Open-Drain, Digital
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

    XBR0      = 0x05;
    XBR1      = 0x80;
    XBR2      = 0x40;
}

void Oscillator_Init()
{
    OSCICN    = 0x05;
}

void Interrupts_Init()
{
    EIE1      = 0x02;
    EIE2      = 0x01;
}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    Timer_Init();
    UART_Init();
    SMBus_Init();
    Port_Init();
    Oscillator_Init();
    Interrupts_Init();
}
