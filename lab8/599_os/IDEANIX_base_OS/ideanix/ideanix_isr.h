#ifndef isr_h 
#define isr_h



void UART0_ISR (void)  ;
void UART1_ISR (void)  ;

void PCA_ISR(void);

char putchar (char c);

char _getkey (void); 



#endif
