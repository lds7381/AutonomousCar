/*
 * File:        led.c
 * Purpose:     Provide UART routines for serial IO
 * Author:			Andre Da Costa
 * Notes:		
 *
 */

#include "msp.h"
#include "Common.h"  // from Lab1 code

void LED1_Init(void)
{
	// configure PortPin for LED1 as port I/O 
	P1->SEL0 &= ~BIT0;
	
	// make built-in LED1 LED high drive strength
	P1->DS |= BIT0;

	// make built-in LED1 out	 
	P1->DIR |= BIT0;

	// turn off LED
	P1->OUT &= ~BIT0;
}

void LED2_Init(void)
{
	// configure PortPin for LED2 as port I/O 
	P2->SEL0 &= ~BIT0;
	P2->SEL1 &= ~BIT0;
	P2->SEL0 &= ~BIT1;
	P2->SEL1 &= ~BIT1;
	P2->SEL0 &= ~BIT2;
	P2->SEL1 &= ~BIT2;
	
	// make built-in LED2 LEDs high drive strength
	P2->DS |= BIT0;
	P2->DS |= BIT1;
	P2->DS |= BIT2;


	// make built-in LED2 out	 
	P2->DIR |= BIT0;
	P2->DIR |= BIT1;
	P2->DIR |= BIT2;

	// turn off LED
	P2->OUT &= ~BIT0;
  P2->OUT &= ~BIT1;
	P2->OUT &= ~BIT2;
}

void Change_LED1(int on){
		if(on){
				// turn on LED1
				P1->OUT |= BIT0;
		}
		else
		{
			// turn off LED1
				P1->OUT &= ~BIT0;
		}
}

void Change_LED2(int color){
	if (color & BIT0){ // if red bit = 1
		P2->OUT |= BIT0;  //set red bit
	}
	else{
		P2->OUT &= ~BIT0;
	}
	if (color & BIT1){
		P2->OUT |= BIT1;
	}
	else{
		P2->OUT &= ~BIT1;
	}
	if (color & BIT2){
		P2->OUT |= BIT2;
	}
	else{
		P2->OUT &= ~BIT2;
	}
}

BOOLEAN Led1_State(){
	return (P1->OUT & BIT0);
}

