/*
 * File:        switches.c
 * Purpose:     Provide switches routine
 * Author:			Andre Da Costa
 * Notes:		
 *
 */

#include "msp.h"
#include "Common.h"  // from Lab1 code
#include "CortexM.h"
#include "leds.h"

int speedMode = 1; //Speed Mode: 0-Slow 1-Medium 2-FAST

void Switches_Init(void)
{
	// configure PortPin for Switch 1 and Switch2 as port I/O 
	P1->SEL0 &= ~BIT1; // Port 1 Pin 1 (switch 1) input
	P1->SEL1 &= ~BIT1; // Port 1 Pin 1 (switch 1) input
	P1->SEL0 &= ~BIT4; // Port 1 Pin 1 (switch 1) input
	P1->SEL1 &= ~BIT4; // Port 1 Pin 4 (switch 2) input
	// configure as input
	P1->DIR &= ~BIT1; // sets it as input
	P1->REN |= BIT1; // sets resitor enable
	P1->OUT |= BIT1; // sets it as pulldown
	P1->DIR &= ~BIT4; // sets port 1 pin 4 as input 
	P1->REN |= BIT4; // sets resitor enable
	P1->OUT |= BIT4; // sets it as pulldown
  // init leds
	LED2_Init();
}


//------------Switch_Input------------
// Read and return the status of Switch1
// Input: none
// return: TRUE if pressed
//         FALSE if not pressed
BOOLEAN Switch1_Pressed(void)
{
	BOOLEAN retVal = FALSE;
	// check if pressed
	retVal = P1->IN & BIT1;
	if (retVal){
		retVal = FALSE;
	}
	else{
		retVal = TRUE;
	}
	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}
//------------Switch_Input------------
// Read and return the status of Switch2
// Input: none
// return: TRUE if pressed
//         FALSE if not pressed
BOOLEAN Switch2_Pressed(void)
{
	BOOLEAN retVal = FALSE;
	// check if pressed
	retVal = P1->IN & BIT4;
	if (retVal){
		retVal = FALSE;
	}
	else{
		retVal = TRUE;
	}
	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}

void Switch1_Interrupt_Init(void)
{
	// disable interrupts
	DisableInterrupts();
	// initialize the Switch as per previous lab
	Switches_Init();
	
	// *****************************
	// *** Pre-Lab 04-1          ***
	//7-0 PxIFG RW 0h Port X interrupt flag
	//0b = No interrupt is pending.
	//1b = Interrupt is pending.
	// clear flag1 (reduce possibility of extra interrupt)	
  	P1->IFG &= ~BIT1; 

	//7-0 PxIE RW 0h Port X interrupt enable
	//0b = Corresponding port interrupt disabled
	//1b = Corresponding port interrupt enabled	
	// arm interrupt on  P1.1	
  	P1->IE |= BIT1;  

	//7-0 PxIES RW Undefined Port X interrupt edge select
  	//0b = PxIFG flag is set with a low-to-high transition.
 	 //1b = PxIFG flag is set with a high-to-low transition
	// now set the pin to cause falling edge interrupt event
	// P1.1 is falling edge event
	P1->IES |= BIT1;
	
	// now set the pin to cause falling edge interrupt event
 	NVIC_IPR8 = (NVIC_IPR8 & 0x00FFFFFF)|0x40000000; // priority 2
	
	// enable Port 1 - interrupt 35 in NVIC	
	NVIC_ISER1 = 0x00000008;
	
	// enable interrupts  (// clear the I bit	)
  EnableInterrupts();              
}

void Switch2_Interrupt_Init(void)
{
	// disable interrupts
	DisableInterrupts();
	
	// initialize the Switch as per previous lab
	
	// *****************************
	// *** Pre-Lab 04-2          ***
	// clear flag4 (reduce possibility of extra interrupt)
	P1->IFG &= ~BIT4; 
	
	// arm interrupt on P1.4 
	P1->IE |= BIT4;     
	
	// now set the pin to cause falling edge interrupt event
	// P1.4 is falling edge event
  	P1->IES |= BIT4;
  	// *****************************

	// now set the pin to cause falling edge interrupt event
  	NVIC_IPR8 = (NVIC_IPR8&0x00FFFFFF)|0x40000000; // priority 2
	
	// enable Port 1 - interrupt 35 in NVIC
 	 NVIC_ISER1 = 0x00000008;
	
	// enable interrupts  (// clear the I bit	)
  	EnableInterrupts();              
}

void PORT1_IRQHandler(void)
{
	//uart0_put("Inside of IRQ Handler\r\n");

	// First we check if it came from Switch1 ?
  	if(P1->IFG & BIT1)  // we start a timer to toggle the LED1 1 second ON and 1 second OFF
	{
		// acknowledge P1.1 is pressed, by setting BIT1 to zero - remember P1.1 is switch 1
		// clear flag, acknowledge
    	P1->IFG &= ~BIT1;
		// Change speed mode
	  speedMode = 0;
		Change_LED2(2);	// Change light to green
  }

	// Now check to see if it came from Switch2 ?
  	if(P1->IFG & BIT4)
		{
		// acknowledge P1.4 is pressed, by setting BIT4 to zero - remember P1.4 is switch 2
    P1->IFG &= ~BIT4;     // clear flag4, acknowledge
		// Change speed mode
		speedMode = 2;
		Change_LED2(1); // Change light to red
  }
}

