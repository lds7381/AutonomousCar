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
#include "Common.h"
#include "MotorControl.h"
#include "Camera.h"

#define PID (TRUE)

// Initalization Funtion for PID Control
void PID_Init(pid_t* pidControl, float min, float max, float ki, float kp, float kd){
	pidControl->min   	 = min;
	pidControl->max      = max;
	pidControl->ki       = ki;
	pidControl->kp    	 = kp;
	pidControl->kd    	 = kd;
	pidControl->error[0] = 0;
	pidControl->error[1] = 0;
	pidControl->error[2] = 0;
	pidControl->integ    = 0;
}

float runMotors_PID(pid_t* pidControl, float desiredDutyCycle){
	// Set up
	float newDutyCyle;
	float actualDutyCycle = 0;  // what is this going to be???
	float err;
	// Get error and set to pidControl
	err = desiredDutyCycle - actualDutyCycle;	
	pidControl->error[0] = err;
	// Add to integration value
	if (err <= pidControl->max){		// Check to make sure the error isnt above the max value
		pidControl->integ = pidControl->integ + err;
	}
	else{
		pidControl->integ = 0;
	}
	// Calculate new duty cycle
	newDutyCyle = (pidControl->kp*err) + (pidControl->ki*pidControl->integ) + (pidControl->kd*(err-pidControl->error[1]));
	//            Propotional            Integrate                            Derivative
	// Might need a clip here from (-1.0 to 1.0)
	// Set old errors
	pidControl->error[2] = pidControl->error[1];
	pidControl->error[1] = err;
	// Return new duty cycle to motor
	return newDutyCyle;
}

float runServo_PID(pid_t* pidControl, float desiredPos, uint16_t* lineData){
	// Set up
	float newPos;
	edges_t edges;   // This willl come from camera data
	int actualPos; 
	float err;
	
	// Get actual Position
	edges = getPostionFromLineData(lineData);
	actualPos = edges.midPos;
	// Get error and set pidControl
	err = desiredPos - actualPos;
	pidControl->error[0] = err;
	// Add to integration value
	if (err <= pidControl->max){		// Check to make sure the error isnt above the max value
		pidControl->integ = pidControl->integ + err;
	}
	else {
		pidControl->integ = 0;
	}
	// Calculate the new servo position
	newPos = (pidControl->kp*err) + (pidControl->ki*pidControl->integ) + (pidControl->kd*(err-pidControl->error[1]));
	//       Propotional            Integrate                            Derivative
	// Might need a clip here from (0 to 127)
	// Set old errors
	pidControl->error[2] = pidControl->error[1];
	pidControl->error[1] = err;
	// Return new duty cycle to motor
	return newPos;
}

float getDutyCycleFromPos(int servoPos){
	// Max Right: 0.0471 Max Left: 0.0521 Mid: 0.0496
	float a = 0.0471;
	float b = 0.0521;
	float x = (float)servoPos;
	float xmax = 127;
	float xmin = 0;
	float sDutyCycle;
	// Calculate the normalized duty cycle from position
	sDutyCycle = (b-a)*((x - xmin)/(xmax - xmin))+a;
	// Return new servo duty cycle from pid position
	if (sDutyCycle > b){
		sDutyCycle = b;
	}
	else if (sDutyCycle < a) {
		sDutyCycle = a;
	}
	return sDutyCycle;
}

// Initalization Function with paramters (will start the motor at required cycle)
// Initalizes TimerA0
void DCMotor_Init(uint16_t period, double dutyCycle){
		// Initalize Motors
    int ret = TIMER_A0_PWM_Init(period, dutyCycle, 1);
        if (ret != 0){
            uart0_put("Warning: DC Motor 1 Failed to Initialize!");
		}
		ret = TIMER_A0_PWM_Init(period, dutyCycle, 2);
        if (ret != 0){
            uart0_put("Warning: DC Motor 1 Failed to Initialize!");
		}
		ret = TIMER_A0_PWM_Init(period, dutyCycle, 3);
        if (ret != 0){
            uart0_put("Warning: DC Motor 2 Failed to Initialize!");
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

void LeftMotorBackward(double dutyCycle){
	TIMER_A0_PWM_DutyCycle(dutyCycle,3); 
	TIMER_A0_PWM_DutyCycle(0,2);
	TIMER_A0_PWM_DutyCycle(0,1);
	TIMER_A0_PWM_DutyCycle(0,4);
}

void LeftMotorForward(double dutyCycle){
	TIMER_A0_PWM_DutyCycle(dutyCycle, 4); 
	TIMER_A0_PWM_DutyCycle(0,2);
	TIMER_A0_PWM_DutyCycle(0,3);
	TIMER_A0_PWM_DutyCycle(0,1);
}

void RightMotorBackward(double dutyCycle){
	TIMER_A0_PWM_DutyCycle(dutyCycle, 1); 
	TIMER_A0_PWM_DutyCycle(0,2);
	TIMER_A0_PWM_DutyCycle(0,3);
	TIMER_A0_PWM_DutyCycle(0,4);
}

void RightMotorForward(double dutyCycle){
	TIMER_A0_PWM_DutyCycle(dutyCycle, 2); // M2B
	TIMER_A0_PWM_DutyCycle(0,3);
	TIMER_A0_PWM_DutyCycle(0,4);
	TIMER_A0_PWM_DutyCycle(0,1);
}

void LeftMotorOff(){
	P3->OUT &= ~BIT7; // Set M1EN
	TIMER_A0_PWM_DutyCycle(0, 1); // M1A 0% pwm
	TIMER_A0_PWM_DutyCycle(0, 2); // M1B 0% pwm
}

void RightMotorOff(){
	P3->OUT &= ~BIT6; // Set M1EN
	TIMER_A0_PWM_DutyCycle(0, 3); // M1A 0% pwm
	TIMER_A0_PWM_DutyCycle(0, 4); // M1B 0% pwm
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

void testMotorForward(double dutyCycle){
		int i = 0;
		DCMotor_On();
		LeftMotorForward(dutyCycle);
		for(i=0;i<10000000;i++);
		RightMotorForward(dutyCycle);
		for(i=0;i<10000000;i++);
		DCMotor_Modify(dutyCycle);
		for(i=0;i<10000000;i++);
		DCMotor_Off();
}
