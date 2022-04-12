#pragma once
/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: Header file for Motor Control helper class.
*/

#include "msp.h"
#include "uart.h"
#include "TimerA.h"

//typedef struct pit_struct pid_t;
typedef struct{
	double max;
	double min;
	double error[3];		// Mapping: {n, n-1, n-2}
	double integ;
	double kp;
	double ki;
	double kd;
} pid_t;

void DCMotor_Init(uint16_t period, double dutyCycle);
void Servo_Init(uint32_t period, double dutyCycle);
void DCMotor_Modify(double dutyCycle);
void Servo_Modify(double dutyCycle);
void RightMotorForward(double dutyCycle);
void RightMotorBackward(double dutyCycle);
void LeftMotorForward(double dutyCycle);
void LeftMotorBackward(double dutyCycle);
void LeftMotorOff(void);
void RightMotorOff(void);
void DCMotor_On(void);
void DCMotor_Off(void);
void testMotorForward(double dutyCycle);
void PID_Init(pid_t* pidControl, double min, double max, double ki, double kp, double kd);
double runServo_PID(pid_t* pidControl, double desiredPos, uint16_t* lineData);
double getDutyCycleFromPos(int servoPos);
double runMotors_PID(pid_t* pidControl, double desiredDutyCycle);
