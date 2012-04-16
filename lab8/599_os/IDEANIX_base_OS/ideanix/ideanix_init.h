#ifndef ideanix_init_h
#define ideanix_init_h

#define UART0_BAUDRATE 9600
#define UART1_BAUDRATE 9600
#define SYSCLK       24500000          // Internal oscillator frequency in Hz



void init_all(void);
void SYSCLK_Init (void);
void Port_IO_Init(void);
void UART0_init(void);
void UART1_init(void);

void TimerInitiate(void);



#endif
