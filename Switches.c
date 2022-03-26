/*
 * File:        switches.c
 * Purpose:     Provide switches routine
 * Author:			Andre Da Costa
 * Notes:		
 *
 */

#include "msp.h"
#include "Common.h"  // from Lab1 code

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

