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
void PID_Init(pid_t* pidControl, double min, double max, double ki, double kp, double kd){
	pidControl->min   	 = min;
	pidControl->max      = max;
	pidControl->ki       = ki;
	pidControl->kp    	 = kp;
	pidControl->kd    	 = kd;
	pidControl->error[0] = 0.0;
	pidControl->error[1] = 0.0;
	pidControl->error[2] = 0.0;
	pidControl->integ    = 0.0;
}

double runMotors_PID(pid_t* pidControl, double desiredDutyCycle, int servoPos){
	// Set up
	double newDutyCyle;
	double baseDutyCycle = 0.20;
	double actualDutyCycle;
	double err;
	char str[50];
	
	if(servoPos > 55 && servoPos < 75){
			actualDutyCycle = baseDutyCycle;
	}
	else {
			actualDutyCycle = desiredDutyCycle;
	}
	
	// Get error and set to pidControl
	err = desiredDutyCycle - actualDutyCycle;	
	pidControl->error[0] = err;
	// Add to integration value
	if (err <= pidControl->max || err >= pidControl->min){		// Check to make sure the error isnt above the max value
		pidControl->integ = pidControl->integ + err;
	}
	else{
		pidControl->integ = 0;
	}
	// Calculate new duty cycle
	newDutyCyle = baseDutyCycle + (pidControl->kp*err) + (pidControl->ki*pidControl->integ) + (pidControl->kd*(err-pidControl->error[1]));
	//            Offset						Propotional            Integrate                            Derivative
	// Clip newDutyCycle
	if(newDutyCyle > desiredDutyCycle){
			newDutyCyle = desiredDutyCycle;
	}
	else if (newDutyCyle < baseDutyCycle){
			newDutyCyle = baseDutyCycle;
	}
	// Print testing
	sprintf(str, "ServoPos: %d, New: %f\r\n", servoPos, newDutyCyle);
	uart0_put(str);
	// Set old errors
	pidControl->error[2] = pidControl->error[1];
	pidControl->error[1] = err;
	pidControl->error[0] = 0;
	// Return new duty cycle to motor
	return newDutyCyle;
}

double runServo_PID(pid_t* pidControl, double desiredPos, uint16_t* lineData){
	// Set up
	double newPos;
	edges_t edges;   // This willl come from camera data
	int actualPos; 
	double err;
	char str[32];
	
	// Get actual Position
	edges = getPostionFromLineData(lineData);
	actualPos = edges.midPos;
	//sprintf(str, "Left: %d, Mid: %d, Right: %d    ", edges.leftPos, edges.midPos, edges.rightPos);
	//uart0_put(str);
	// Get error and set pidControl
	err = desiredPos - actualPos;
	pidControl->error[0] = err;
	// Add to integration value
	if (err <= pidControl->max && err >= pidControl->min){		// Check to make sure the error isnt above the max value
		pidControl->integ = pidControl->integ + err;
	}
	else {
		pidControl->integ = 0;
	}
	// Calculate the new servo position
	newPos = desiredPos + (pidControl->kp*err)+ (pidControl->ki*pidControl->integ) + (pidControl->kd*(err-pidControl->error[1]));
	//       Offset				Propotional            Integrate                            Derivative
	// Set old errors
	pidControl->error[2] = pidControl->error[1];
	pidControl->error[1] = err;
	pidControl->error[0] = 0;
	// Return new duty cycle to motor
	if(newPos > 127){
		newPos = 127;
	} else if (newPos < 0){
		newPos = 0;
	}
	return newPos;
}

double getDutyCycleFromPos(int servoPos){
	// Max Right: 0.0471 Max Left: 0.0521 Mid: 0.0496
	double a = 0.0471;
	double b = 0.0521;
	double x = (float)servoPos;
	double xmax = 128;
	double xmin = 0;
	double sDutyCycle;
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

double getDCSpeedFromAngle(double sDutyCycle){
	double centerServo = .0497; // from DeebaOS since sDutyCycleMid isnt global
	double maxLeft = .0521; 
	double maxRight = .0471;
	double percent;
	double leftRange = maxLeft - centerServo; 
	double rightRange = centerServo - maxRight; // should probably move all these out of function to reduce runtime
	if (angle < centerServo){ // servo right
		percent = angle - maxRight;
		percent = percent / rightRange;
	}
	else if (angle > centerServo){ // Servo left
		percent = maxLeft - angle;
		percent = percent / leftRange; 
	}
	else{
		percent = 1;
	}
	return percent;
}
// How to Use:
// in main: first do the PID for servo and pass in sDutycycle obtained
//  		from getDutyCycleFromPos into SetDCSpeedFromAngle
//			next, multiply the percent returned from this function
//			by a maxDCPWM and set the DC speed to that.
 

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
		TIMER_A0_PWM_DutyCycle(dutyCycle, 2);
		TIMER_A0_PWM_DutyCycle(dutyCycle, 4);
		TIMER_A0_PWM_DutyCycle(0, 3);
		TIMER_A0_PWM_DutyCycle(0, 1);
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
//		DCMotor_Modify(dutyCycle);
		for(i=0;i<10000000;i++);
		DCMotor_Off();
}
