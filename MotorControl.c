/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: C file for DCMotor helper class.
                 Will Init and Run DCMotors.
*/

#include <stdio.h>
#include <stdlib.h>
#include "TimerA.h"
#include "uart.h"

// Initalization Function with paramters (will start the motor at required cycle)
// Initalizes TimerA0
void DCMotor_Init(uint16_t period, double dutyCycle){
		// Initalize Motors
    int ret = TIMER_A0_PWM_Init(period, dutyCycle, 1);
        if (ret != 0){
            uart0_put("Warning: DC Motor 1 Failed to Initialize!");
		}
    ret = TIMER_A0_PWM_Init(period, dutyCycle, 4);
        if (ret != 0){
            uart0_put("Warning: DC Motor 2 Failed to Initialize!");
		}
		// Enable PWM on Motor Controller
		P3->SEL0 &= ~BIT6;
		P3->SEL0 &= ~BIT6;
		P3->DIR  |= BIT6;
		P3->SEL0 &= ~BIT7;
		P3->SEL0 &= ~BIT7;
		P3->DIR  |= BIT7;
//		P3->DS   |= BIT6;
//		P3->DS   |= BIT7;
		// Keep the Motors off
		P3->OUT  &= ~BIT6;
		P3->OUT	 &= ~BIT7;
}

// Initalization Function with paramters (will start the motor at required cycle)
// Initalizes TimerA2 (P5.6)
void Servo_Init(uint32_t period, double dutyCycle){
    int ret = TIMER_A2_PWM_Init(period, dutyCycle, 1);
        if (ret != 0){
            uart0_put("Warning: TIMERA2 Failed to Initialize!");
		}
}


void RightMotorForward(double dutyCycle){
		TIMER_A0_PWM_DutyCycle(dutyCycle,4);
		TIMER_A0_PWM_DutyCycle(0,1);
		TIMER_A0_PWM_DutyCycle(0,2);
		TIMER_A0_PWM_DutyCycle(0,3);
}

void LeftMotorBackward(double dutyCycle){
		TIMER_A0_PWM_DutyCycle(dutyCycle, 2);
		TIMER_A0_PWM_DutyCycle(0,1);
		TIMER_A0_PWM_DutyCycle(0,3);
		TIMER_A0_PWM_DutyCycle(0,4);
}

void RightMotorBackward(double dutyCycle){
		TIMER_A0_PWM_DutyCycle(dutyCycle,3);
		TIMER_A0_PWM_DutyCycle(0,4);
		TIMER_A0_PWM_DutyCycle(0,1);
		TIMER_A0_PWM_DutyCycle(0,2);
}

void LeftMotorForward(double dutyCycle){
		TIMER_A0_PWM_DutyCycle(dutyCycle,1);
		TIMER_A0_PWM_DutyCycle(0,2);
		TIMER_A0_PWM_DutyCycle(0,3);
		TIMER_A0_PWM_DutyCycle(0,4);
}

// Changes the Active Duty Cycle of the PWM for the DC Motor on TimerA0
void DCMotor_Modify(double dutyCycle){
    TIMER_A0_PWM_DutyCycle(dutyCycle, 1);
		TIMER_A0_PWM_DutyCycle(0,2);
    TIMER_A0_PWM_DutyCycle(dutyCycle, 4);
		TIMER_A0_PWM_DutyCycle(0,3);
}

// Changes the Active Duty Cycle of the PWM for the DC Motor on TimerA2
void Servo_Modify(double dutyCycle){
    TIMER_A2_PWM_DutyCycle(dutyCycle, 1);
}

void DCMotor_On(){
		P3->OUT |= BIT6;
		P3->OUT |= BIT7;
}

void DCMotor_Off(){
		P3->OUT &= ~BIT6;
		P3->OUT &= ~BIT7;
}
