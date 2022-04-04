#include "msp.h"

#include "i2c.h"
#include "Common.h"


/*
 * i2c.c
 *
 *
 *  Update to the new register notation
 *	LJBeato
 *	03/23/21
 *
 *  added   listener  09/04/21
 * LJBeato
 */


#
extern uint32_t SystemCoreClock;

//static BOOLEAN OLED_Display_Active;
// This function writes the RECEIVER address to the i2c bus.
// If a RECEIVER chip is at that address, it should respond to
// this with an "ACK".   This function returns TRUE if an
// ACK was found.  Otherwise it returns FALSE.

//// --------------------- I2C0  ---------------------------------------------

void i2c0_Init(int listenerAddress)
{
		// set appropriate Port.Pins for SDA/SCL
    P1->SEL0 |=  BIT6;		// P1.6 SEL0=1  SDA
		P1->SEL1 &= ~BIT6;	  // P1.6 SEL1=0  SDA
    P1->SEL0 |=  BIT7;		// P1.7 SEL0=1	SCL
		P1->SEL1 &= ~BIT7;		// P1.7 SEL1=0  SCL
	
		// make sure module is disabled (in reset mode) 
		EUSCI_B0->CTLW0 |= BIT0;
	
		/*
		// 7 bit LISTENER and self, 0
		EUSCI_B0->CTLW0 &= ~BITE;
		// single MASTER, 0 
		EUSCI_B0->CTLW0 &= ~BITD;
	
		// MASTER mode, 1
		EUSCI_B0->CTLW0 |= BITB;	
		// I2C mode, 11
		EUSCI_B0->CTLW0 |= (BITA | BIT9);
	
		// SMCLK mode, 11
		EUSCI_B0->CTLW0 |= (BIT7 | BIT6);	
		
		// don/t acknowledge
		EUSCI_B0->CTLW0 &= ~BIT5;
		// MASTER 
		EUSCI_B0->CTLW0 |= BIT4;
	
		// ack normal, no STOP, no START, stay RESET
    EUSCI_B0->CTLW0 &= ~(BIT3 | BIT2 | BIT1);
		*/
		// All above makes CTLW0 register = 0x0FD1
		// Set the register to its initilization Hex Number
		EUSCI_B0->CTLW0 = 0x0FD1;
		
		// initialize RECEIVER address 
    EUSCI_B0->I2CSA = listenerAddress;

		// set clock: 400 KHz
    EUSCI_B0->BRW  = (uint16_t)(SystemCoreClock/BR_RATE);
		

}

void i2c0_put(BYTE *data, unsigned int len)
{
	int i;

		// enable i2c module, (remove from RESET)
    EUSCI_B0->CTLW0 &= ~BIT0;

    // generate start condition and wait for the bus 
    EUSCI_B0->CTLW0 |= BIT1;  

	// write data byte by byte to i2c, use putchar
	for (i=0; i<len; i++){	
		i2c0_putchar(data[i]);
	}

    // force stop
    EUSCI_B0->CTLW0 |= BIT2; 
		
		// wait for transmission
		while (!(EUSCI_B0->IFG & BIT3));

		// transmission completed, disable the module (put it back in reset)
    EUSCI_B0->CTLW0 |= BIT0;
}

/* write data on i2c bus */
void i2c0_putchar(BYTE data)
{
    // write data to TXBUF
    EUSCI_B0->TXBUF = data;
    
		// wait until byte is transmitted
    while (!(EUSCI_B0->IFG & BIT1));   
}

