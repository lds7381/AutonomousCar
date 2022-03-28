// TimerA.c

/*  LJBeato
    2021
    TimerA functionality to drive DC motor
    and Servo Motor
 */


#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include "TimerA.h"
#include "uart.h"

// Make these arrays 5 deep, since we are using indexes 1-4 for the pins
static uint32_t DEFAULT_PERIOD_A0[5] = {0,0,0,0,0};
static uint32_t DEFAULT_PERIOD_A2[5] = {0,0,0,0,0};
static uint16_t dutyCycle_A0;
static uint16_t dutyCycle_A2;
//***************************PWM_Init*******************************
// PWM output on P2.4, P2.5, P2.6, P2.7
// Inputs:  period of P2.4...P2.7 is number of counts before output changes state
//          percentDutyCycle (0 -> 1.0)
//          pin number (1,2,3,4)
// Outputs: none


int TIMER_A0_PWM_Init(uint16_t period, double percentDutyCycle, uint16_t pin)
{
	// Timer A0.1
	if (pin == 1)
	{
    	P2->SEL0 |= BIT4;		// Initailize for GPIO
		P2->SEL1 &= ~BIT4;
		P2->DIR  |=  BIT4;		// Set direction to output
		P2->DS  |=  BIT4;		// High Drive stregnth
	}
    // Timer A0.2
	else if (pin == 2)
	{
    	P2->SEL0 |= ~BIT5;		// Initailize for GPIO
		P2->SEL1 &= ~BIT5;
		P2->DIR  |=  BIT5;		// Set direction to output
		P2->DS  |=  BIT5;		// High Drive stregnth
	}	
    // Timer A0.3
	else if (pin == 3)
	{
    	P2->SEL0 |= BIT6;		// Initailize for GPIO
		P2->SEL1 &= ~BIT6;
		P2->DIR  |=  BIT6;		// Set direction to output
		P2->DS  |=  BIT6;		// High Drive stregnth
	}	
    // Timer A0.4
	else if (pin == 4)
	{
    	P2->SEL0 |= BIT7;		// Initailize for GPIO
		P2->SEL1 &= ~BIT7;
		P2->DIR  |=  BIT7;		// Set direction to output
		P2->DS  |=  BIT7;		// High Drive stregnth   
	}
	else return -2;
	
	// save the period for this timer instance
	// DEFAULT_PERIOD_A0[pin] where pin is the pin number
	DEFAULT_PERIOD_A0[pin] = period;

	// Stop the Timer For Initialization
	if (pin == 1)
		TIMER_A0->CCR[0] = 0;

	// TIMER_A0->CCTL[pin]
	// Sync compare input high, Output mode Reset/Set 
  	TIMER_A0->CCTL[pin] = 0x04e1;
	
	// set the duty cycle
	dutyCycle_A0 = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A0[pin]);

	// CCR[n] contains the dutyCycle just calculated, where n is the pin number
  	//TIMER_A0->CCR[pin]
  	TIMER_A0->CCR[pin] = dutyCycle_A0;
	
	// Timer CONTROL register
	// TIMER_A0->CTL
	TIMER_A0->CTL |= 0x211; 

	// Start the timer by inputting the counter value
	if (pin == 1)
		TIMER_A0->CCR[0] = DEFAULT_PERIOD_A0[pin];

	return 0;
}
//***************************PWM_Duty1*******************************
// change duty cycle of PWM output on pin
// Inputs:  dutycycle, pin
// Outputs: none
// percentDutyCycle is a number between 0 and 1  (ie. 0.5 = 50%)
void TIMER_A0_PWM_DutyCycle(double percentDutyCycle, uint16_t pin)
{
		uint16_t dutyCycle = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A0[pin]); 		// Find period that the duty cycle will be high for.
		TIMER_A0->CCR[pin] = dutyCycle;
}

//***************************PWM_Init*******************************
// PWM output on P5.6
// Inputs:  period of P5.6 is number of counts before output changes state
//          percentDutyCycle (0 -> 1.0)//          duty cycle
//          pin number (1,2,3,4), but always 1
// Outputs: none
int TIMER_A2_PWM_Init(uint32_t period, double percentDutyCycle, uint16_t pin)
{

	// NOTE: Timer A2 only exposes 1 PWM pin
	// TimerA2.1
	if (pin == 1)
	{
		P5->SEL0 |=  BIT6;		// Initailize for GPIO
		P5->SEL1 &= ~BIT6;
		P5->DIR  |=  BIT6;		// Set direction to output
		P5->DS   |=  BIT6;		// High Drive stregnth
	}
	else return -2; 
	
	// save the period for this timer instance
	// DEFAULT_PERIOD_A0[pin] where pin is the pin number
	DEFAULT_PERIOD_A2[pin] = period;

	// Stop the Timer For Initialization
	if (pin == 1)
		TIMER_A2->CCR[0] = 0;

	// TIMER_A0->CCTL[pin]
	// Sync compare input high, Output mode Reset/Set 
  	TIMER_A2->CCTL[pin] = 0x04e1;
	
	// set the duty cycle
	dutyCycle_A2 = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A2[pin]);

	// CCR[n] contains the dutyCycle just calculated, where n is the pin number
  	//TIMER_A0->CCR[pin]
  	TIMER_A2->CCR[pin] = dutyCycle_A2;
	
	// Timer CONTROL register
	// TIMER_A0->CTL
	TIMER_A2->CTL |= 0x261; 
	
	TIMER_A2->EX0 |= 0x6;

	// Start the timer by inputting the counter value
	if (pin == 1)
		TIMER_A2->CCR[0] = DEFAULT_PERIOD_A2[pin];

	return 0;
}
//***************************PWM_Duty1*******************************
// change duty cycle of PWM output on P5.6
// Inputs:  percentDutyCycle, pin  (should always be 1 for TimerA2.1)
//         
// Outputs: none
// 
void TIMER_A2_PWM_DutyCycle(double percentDutyCycle, uint16_t pin)
{
	uint16_t dutyCycle = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A2[pin]); 		// Find period that the duty cycle will be high for.	// Find period that the duty cycle will be high for.
	TIMER_A2->CCR[pin] = dutyCycle;	
}

