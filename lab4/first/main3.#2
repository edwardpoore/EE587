//-----------------------------------------------------------------------------
// main3.c
//-----------------------------------------------------------------------------
// Main program for EE599 Experiment #4
//
// Author: James E. Lumpp
// Date: 2/21/02
// Target:  8051F020
// Assignment: Experiment #4
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <compiler_defs.h>
#include <C8051F020_defs.h>                 // SFR declarations
#include <stdio.h>

//#include "16sfrs.h"                    // 16 bit SFRs

#include "init3.h"                    // defines for the init functions

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------


#define  EEPROM_SIZE 20

#define  SMB_FREQUENCY  400000L        // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE          0x00           // WRITE direction bit
#define  READ           0x01           // READ direction bit

// Device addresses (7 bits, lsb is a don't care)
#define  EEPROM_ADDR    0xA0           // Device address for slave target
                                       // Note: This address is specified
                                       // in the Microchip 24LC02B
                                       // datasheet.

#define  MY_ADDR        0x00           // Address of this SMBus device
                                       // (dummy value since this device does
                                       // not have any defined slave states)

// SMBus Buffer Size
#define  SMB_BUFF_SIZE  0x08           // Defines the maximum number of bytes
                                       // that can be sent or received in a
                                       // single transfer

#define  SMB_BUS_ERROR  0x00           // (all modes) BUS ERROR
#define  SMB_START      0x08           // (MT & MR) START transmitted
#define  SMB_RP_START   0x10           // (MT & MR) repeated START
#define  SMB_MTADDACK   0x18           // (MT) Slave address + W transmitted;
                                       //    ACK received
#define  SMB_MTADDNACK  0x20           // (MT) Slave address + W transmitted;
                                       //    NACK received
#define  SMB_MTDBACK    0x28           // (MT) data byte transmitted;
                                       //    ACK rec'vd
#define  SMB_MTDBNACK   0x30           // (MT) data byte transmitted;
                                       //    NACK rec'vd
#define  SMB_MTARBLOST  0x38           // (MT) arbitration lost
#define  SMB_MRADDACK   0x40           // (MR) Slave address + R transmitted;
                                       //    ACK received
#define  SMB_MRADDNACK  0x48           // (MR) Slave address + R transmitted;
                                       //    NACK received
#define  SMB_MRDBACK    0x50           // (MR) data byte rec'vd;
                                       //    ACK transmitted
#define  SMB_MRDBNACK   0x58           // (MR) data byte rec'vd;
                                       //    NACK transmitted

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

unsigned char* pSMB_DATA_IN;           // Global pointer for SMBus data
                                       // All receive data is written here

unsigned char SMB_SINGLEBYTE_OUT;      // Global holder for single byte writes.

unsigned char* pSMB_DATA_OUT;          // Global pointer for SMBus data.
                                       // All transmit data is read from here

unsigned char SMB_DATA_LEN;            // Global holder for number of bytes
                                       // to send or receive in the current
                                       // SMBus transfer.

unsigned char WORD_ADDR;               // Global holder for the EEPROM word
                                       // address that will be accessed in
                                       // the next transfer

unsigned char TARGET;                  // Target SMBus slave address

bit SMB_BUSY = 0;                      // Software flag to indicate when the
                                       // EEPROM_ByteRead() or
                                       // EEPROM_ByteWrite()
                                       // functions have claimed the SMBus

bit SMB_RW;                            // Software flag to indicate the
                                       // direction of the current transfer

bit SMB_SENDWORDADDR;                  // When set, this flag causes the ISR
                                       // to send the 8-bit <WORD_ADDR>
                                       // after sending the slave address.

bit SMB_RANDOMREAD;                    // When set, this flag causes the ISR
                                       // to send a START signal after sending
                                       // the word address.
                                       // For the 24LC02B EEPROM, a random read
                                       // (a read from a particular address in
                                       // memory) starts as a write then
                                       // changes to a read after the repeated
                                       // start is sent. The ISR handles this
                                       // switchover if the <SMB_RANDOMREAD>
                                       // bit is set.

bit SMB_ACKPOLL;                       // When set, this flag causes the ISR
                                       // to send a repeated START until the
                                       // slave has acknowledged its address

// 16-bit SFR declarations
//sfr16    TMR3RL   = 0x92;              // Timer3 reload registers
//sfr16    TMR3     = 0x94;              // Timer3 counter registers


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void dump_eeprom(void); 
void block_fill(void); 
void read_eeprom(void); 
void write_eeprom(void); 

void SMBus_ISR(void);
void Timer3_ISR(void);


unsigned char read_byte(unsigned char);
void write_byte(unsigned char, unsigned char);


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void) 
{
    char keypress;
   //unsigned int i;
   
   WDTCN = 0xde;                       // disable watchdog timer
   WDTCN = 0xad;

   SYSCLK_Init ();                     // initialize oscillator

    /*
      while(!SDA)
   {
      // Provide clock pulses to allow the slave to advance out
      // of its current state. This will allow it to release SDA.
      XBR1 = 0x40;                     // Enable Crossbar
      SCL = 0;                         // Drive the clock low
      for(i = 0; i < 255; i++);        // Hold the clock low
      SCL = 1;                         // Release the clock
      while(!SCL);                     // Wait for open-drain
                                       // clock output to rise
      for(i = 0; i < 10; i++);         // Hold the clock high
      XBR1 = 0x00;                     // Disable Crossbar
   }
   */
   PORT_Init ();                       // initialize crossbar and GPIO
   UART0_Init ();                      // initialize UART0

   SMBus_Init();
   Timer3_Init();
   Interrupts_Init();

   SI = 0;


   printf ("EE587 Experiment #3\n");
   printf ("Initialization Complete...\n");
  
   while (1) {

       printf("what would you like to do? (d,b,r,w):");
    
       keypress = getchar();

       printf("\n");

    
       switch(keypress) {
           case 'd': dump_eeprom(); break;
           case 'b': block_fill(); break;
           case 'r': read_eeprom(); break;
           case 'w': write_eeprom(); break;
           case ' ':;
           default:
               printf("Invalid Command '%c'\n",keypress);
               break;
       }
    
    
    
    }  // while(1)
    
}  // main
//////////////////////////////////////////////////////////////
/// dump_eeprom
//////////////////////////////////////////////////////////////
void dump_eeprom()   // dump the contents of the eeprom
{
    unsigned char i;

    printf("EEPROM contents are:\n");
    for(i=0;i<EEPROM_SIZE;i++) {
        printf("%bx ",read_byte(i));
    }
    printf("\n");
}  // dump_eeprom();
//////////////////////////////////////////////////////////////
/// block_fill
//////////////////////////////////////////////////////////////
void block_fill()    // block fill the eeprom
{
    unsigned char i;
    unsigned int data_in;

    printf("EEPROM block fill...\n");

    for(i=0;i<EEPROM_SIZE;i++) {
        printf("eeprom(%bx):",i);
        scanf("%x", &data_in);
        write_byte(i,(unsigned char)data_in);
        printf("\n");
    }
}  // block_fill();
//////////////////////////////////////////////////////////////
/// read_eeprom
//////////////////////////////////////////////////////////////
void read_eeprom()   // read a byte from the eeprom
{
int address;

  printf("address to read?:");
  scanf("%x",&address);  // this is a very poor/dangerous use of scanf. if the 
                        // user input does not match, you are toast.  not to
                        // mention the fact we are ignoring the return code
                        // (do as i say, not as i do)
  printf("\n");

  if(address < 0 || address > EEPROM_SIZE-1) {
     printf("Bad Address (Ignored)\n");
  } else {
    printf("Byte @ %bx is %bx\n",(unsigned char)address,read_byte((unsigned char)address));
  }
}  // read_eeprom();
//////////////////////////////////////////////////////////////
/// write_eeprom
//////////////////////////////////////////////////////////////
void write_eeprom()  // write a byte to the eeprom
{
unsigned int address;
unsigned char databyte;

  printf("address to write?:");
  scanf("%x",&address);  // this is a very poor/dangerous use of scanf. if the 
                        // user input does not match, you are toast.  not to
                        // mention the fact we are ignoring the return code
                        // (do as i say, not as i do)
  printf("\n");

  if(address < 0 || address > 29) {
     printf("Bad Address (Ignored)\n");
  } else {
    printf("value to write?:");
    scanf("%bx",&databyte);          // don't do this in your programs
    write_byte((unsigned char) address, databyte);
  }
}  // write_eeprom();

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMBus Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// SMBus ISR state machine
// - Master only implementation - no slave states defined
// - All incoming data is written starting at the global pointer <pSMB_DATA_IN>
// - All outgoing data is read from the global pointer <pSMB_DATA_OUT>
//
void SMBUS_ISR (void) interrupt 7
{
   bit FAIL = 0;                       // Used by the ISR to flag failed
                                       // transfers

   static char i;                      // Used by the ISR to count the
                                       // number of data bytes sent or
                                       // received

   static bit SEND_START = 0;          // Send a start

   // Status code for the SMBus (SMB0STA register)
   switch (SMB0STA)
   {
      // Master Transmitter/Receiver: START condition transmitted.
      // Load SMB0DAT with slave device (EEPROM) address.
      case SMB_START:
         SMB0DAT = TARGET;             // Load address of the slave.
         SMB0DAT &= 0xFE;              // Clear the LSB of the address for the
                                       // R/W bit
         SMB0DAT |= SMB_RW;            // Load R/W bit
         STA = 0;                      // Manually clear STA bit
         i = 0;                        // Reset data byte counter

         break;

      // Master Transmitter/Receiver: Repeated START condition transmitted.
      // For a READ: This state only occurs during an EEPROM read operation
      //  for the EEPROM's "random read", where a write operation with the
      //  data address must be done before the read operation.
      //
      // For a WRITE: N/A
      case SMB_RP_START:
         SMB0DAT = TARGET;             // Load address of the slave.
         SMB0DAT |= SMB_RW;            // Load R/W bit
         STA = 0;                      // Manually clear START bit.
         i = 0;                        // Reset data byte counter

         break;

      // Master Transmitter: Slave address + WRITE transmitted.  ACK received.
      // For a READ or WRITE: the word (data) address in the EEPROM must now
      //  be sent after the slave address
      case SMB_MTADDACK:

         SMB0DAT = WORD_ADDR;          // Send word address

         if (SMB_RANDOMREAD)
         {
            SEND_START = 1;            // Send a START after the next ACK cycle
            SMB_RW = READ;
         }

         break;

      // Master Transmitter: Slave address + WRITE transmitted.  NACK received.
      // If the EEPROM should be polled, restart the transfer.
      // Otherwise, reset the SMBus
      case SMB_MTADDNACK:
         if(SMB_ACKPOLL)
         {
            STA = 1;                   // Restart transfer
         }
         else
         {
            FAIL = 1;                  // Indicate failed transfer
         }                             // and handle at end of ISR

         break;

      // Master Transmitter: Data byte transmitted.  ACK received.
      // For a READ: the word address has been trasmitted, and a repeated
      //  start should be generated.
      //
      // For a WRITE: each data word should be sent
      case SMB_MTDBACK:

         if (SEND_START)
         {
            STA = 1;
            SEND_START = 0;
            break;
         }

         if (SMB_RW==WRITE)            // Is this transfer a WRITE?
         {
            if (i < SMB_DATA_LEN)      // Is there data to send?
            {
               // Send data byte
               SMB0DAT = *pSMB_DATA_OUT;

               // Increment data out pointer
               pSMB_DATA_OUT++;

               // Increment number of bytes sent
               i++;
            }
            else
            {
              STO = 1;                 // Set STO to terminte transfer
              SMB_BUSY = 0;            // Clear software busy flag
            }
         }
         else {}                       // If this transfer is a READ, then
                                       // take no action, as a repeated
                                       // start will be generated for the
                                       // read operation

         break;

      // Master Transmitter: Data byte transmitted.  NACK received.
      // If the EEPROM should be polled, restart the transfer.
      // Otherwise, reset the SMBus
      case SMB_MTDBNACK:
         if(SMB_ACKPOLL)
         {
            STA = 1;                   // Restart transfer
         }
         else
         {
            FAIL = 1;                  // Indicate failed transfer
         }                             // and handle at end of ISR

         break;

      // Master Transmitter: Arbitration lost.
      case SMB_MTARBLOST:

         FAIL = 1;                     // Indicate failed transfer
                                       // and handle at end of ISR

         break;

      // Master Receiver: Slave address + READ transmitted.  ACK received.
      // For a READ: check if this is a one-byte transfer. if so, set the
      //  NACK after the data byte is received to end the transfer. if not,
      //  set the ACK and receive the other data bytes.
      //
      // For a WRITE: N/A
      case SMB_MRADDACK:
         if (i == SMB_DATA_LEN)
         {
            AA = 0;                    // Only one byte in this transfer,
                                       // send NACK after byte is received
         }
         else
         {
            AA = 1;                    // More than one byte in this transfer,
                                       // send ACK after byte is received
         }
         break;

      // Master Receiver: Slave address + READ transmitted.  NACK received.
      // If the EEPROM should be polled, restart the transfer.
      // Otherwise, reset the SMBus
      case SMB_MRADDNACK:
         if(SMB_ACKPOLL)
         {
            STA = 1;                   // Restart transfer
         }
         else
         {
            FAIL = 1;                  // Indicate failed transfer
         }                             // and handle at end of ISR

         break;

      // Master Receiver: Data byte received.  ACK transmitted.
      // For a READ: receive each byte from the EEPROM.  if this is the last
      //  byte, send a NACK and set the STOP bit.
      //
      // For a WRITE: N/A
      case SMB_MRDBACK:
         if ( i < SMB_DATA_LEN )       // Is there any data remaining?
         {
            *pSMB_DATA_IN = SMB0DAT;   // Store received byte
            pSMB_DATA_IN++;            // Increment data in pointer
            i++;                       // Increment number of bytes received
            AA = 1;                    // Send ACK (may be cleared later
                                       // in the code)

         }

         if (i == SMB_DATA_LEN)        // This is the last byte
         {
            SMB_BUSY = 0;              // Free SMBus interface
            AA = 0;                    // Send NACK to indicate last byte
                                       // of this transfer
            STO = 1;                   // Send STOP to terminate transfer
         }

         break;

      // Master Receiver: Data byte received.  NACK transmitted.
      // Read operation has completed.  Read data register and send STOP.
      case SMB_MRDBNACK:
         *pSMB_DATA_IN = SMB0DAT;      // Store received byte
         STO = 1;
         SMB_BUSY = 0;
         AA = 1;                       // Set AA for next transfer

         break;

      // All other status codes invalid.  Reset communication.
      default:
         FAIL = 1;

         break;
   }

   if (FAIL)                           // If the transfer failed,
   {
      SMB0CN &= ~0x40;                 // Reset communication
      SMB0CN |= 0x40;
      STA = 0;
      STO = 0;
      AA = 0;

      SMB_BUSY = 0;                    // Free SMBus

      FAIL = 0;
   }

   SI = 0;                             // Clear interrupt flag
}

//-----------------------------------------------------------------------------
// Timer3 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled if a timeout occurs.
//
void Timer3_ISR (void) interrupt 14
{
   SMB0CN &= ~0x40;                    // Disable SMBus
   SMB0CN |= 0x40;                     // Re-enable SMBus
   TMR3CN &= ~0x80;                    // Clear Timer3 interrupt-pending flag
   SMB_BUSY = 0;                       // Free bus
}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// write_byte
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) unsigned char addr - address to write in the EEPROM
//                        range is full range of character: 0 to 255
//
//   2) unsigned char dat - data to write to the address <addr> in the EEPROM
//                        range is full range of character: 0 to 255
//
// This function writes the value in <dat> to location <addr> in the EEPROM
// then polls the EEPROM until the write is complete.
//
void write_byte(unsigned char addr, unsigned char dat)
{
   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TARGET = EEPROM_ADDR;               // Set target slave address
   SMB_RW = WRITE;                     // Mark next transfer as a write
   SMB_SENDWORDADDR = 1;               // Send Word Address after Slave Address
   SMB_RANDOMREAD = 0;                 // Do not send a START signal after
                                       // the word address
   SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling (The ISR
                                       // will automatically restart the
                                       // transfer if the slave does not
                                       // acknowledge its address.

   // Specify the Outgoing Data
   WORD_ADDR = addr;                   // Set the target address in the
                                       // EEPROM's internal memory space

   SMB_SINGLEBYTE_OUT = dat;           // Store <dat> (local variable) in a
                                       // global variable so the ISR can read
                                       // it after this function exits

   // The outgoing data pointer points to the <dat> variable
   pSMB_DATA_OUT = &SMB_SINGLEBYTE_OUT;

   SMB_DATA_LEN = 1;                   // Specify to ISR that the next transfer
                                       // will contain one data byte

   // Initiate SMBus Transfer
   STA = 1;
}


//-----------------------------------------------------------------------------
// EEPROM_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//   1) unsigned char data - data read from address <addr> in the EEPROM
//                        range is full range of character: 0 to 255
//
// Parameters   :
//   1) unsigned char addr - address to read data from the EEPROM
//                        range is full range of character: 0 to 255
//
// This function returns a single byte from location <addr> in the EEPROM then
// polls the <SMB_BUSY> flag until the read is complete.
//
unsigned char read_byte(unsigned char addr)
{
   unsigned char retval;               // Holds the return value

   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TARGET = EEPROM_ADDR;               // Set target slave address
   SMB_RW = WRITE;                     // A random read starts as a write
                                       // then changes to a read after
                                       // the repeated start is sent. The
                                       // ISR handles this switchover if
                                       // the <SMB_RANDOMREAD> bit is set.
   SMB_SENDWORDADDR = 1;               // Send Word Address after Slave Address
   SMB_RANDOMREAD = 1;                 // Send a START after the word address
   SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling


   // Specify the Incoming Data
   WORD_ADDR = addr;                   // Set the target address in the
                                       // EEPROM's internal memory space

   pSMB_DATA_IN = &retval;             // The incoming data pointer points to
                                       // the <retval> variable.

   SMB_DATA_LEN = 1;                   // Specify to ISR that the next transfer
                                       // will contain one data byte

   // Initiate SMBus Transfer
   STA = 1;
   while(SMB_BUSY);                    // Wait until data is read

   return retval;

}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
