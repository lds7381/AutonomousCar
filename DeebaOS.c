/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: Main file for Operating system for Deeba OS.
                 Runs the entire autnomous car.
*/
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "uart.h"
#include "oled.h"
#include "ADC14.h"
#include "MotorControl.h"
#include "Camera.h"
#include "msp432p4011.h"
#include "CortexM.h"

extern long CalcPeriodFromFrequency (double Hz);

int main(void) {
    // Main Variables
		uint16_t dcPeriod 	= CalcPeriodFromFrequency(10000);    // Run at 10kHz Period
    uint32_t sPeriod  	= CalcPeriodFromFrequency(50);
		int carpetCount 	= 0;
		int onCarpet 		= 0;
		BOOLEAN move 		= TRUE;
		uint16_t* lineData;
		int i;
    
    // Motor Control Variables
    double dcDutyCycle     = 0.24;       		// Duty Cycle (0 so motors do not move, needed for both motors, will run at same duty cycle always)
		double dcWantedDuty    = 0.23;
		int wantedServoPos     = 64;			// Desired Servo Position (64 is straight)
		double sDutyCycle;
		double sDutyCycleMid   = 0.0497;
		int servoPos;
		pid_t  pid_controlDC;		// Pid Control Variables for DC Motors
		pid_t  pid_controlServo;	// Pid Control Variables for Servo
	
	
		char str[32];				// String used for uart printing
			
		// Current unused Vars
//		double currentTurn;
//		int avgLineData = 0;
//		char turnstr[10];
//		uint16_t totAvg;
//    int sum = 0;
//		uint16_t smoothLine*;
//		double dcDutyCycleTurn = 0.34;			// Turn speed for DC motors
//		double slightLeft      = 0.0517;
//		double slightRight     = 0.0473; 
//		double sDutyCycleR  = 0.0471;
//   	double sDutyCycleL = 0.0521;	
//		double leftAvg;
//		double rightAvg;
//		double midAvg;	
		
    // **** Initalization of Peripherals ****
    // Init Uart0
    uart0_init();
    // Init ADC
    ADC0_InitSWTriggerCh6();
    // Init DC Motors
    DCMotor_Init(dcPeriod, 0);  
    // Init Servo Motor
    Servo_Init(sPeriod, sDutyCycleMid);  // Servo One (only one)
    // Init Camera
    LineScanCamera_Init();
    // Init OLED
    //OLED_Init();
		//OLED_Print(1, 1, (char *)"Hello World?");
    // **************************************		

		///*** MAIN CODE ****
		// Initalize PID DC Motor Control (min=0.15, max=0.30, ki=0.1, kp=0.4, kd=0.6)
		PID_Init(&pid_controlDC, 0.15, 0.30, 0.1, 0.4, 0.6);
		// Initalize PID DC Motor Control (min=-2.5, max=2.5, ki=0.12, kp=0.97, kd=0.08)
		PID_Init(&pid_controlServo, -2.5, 2.5, 0.12, 0.97, 0.08);
		// Start Running the Car
		uart0_put("Deeba is going!\n\r");
		// Set servo striaght
		Servo_Modify(sDutyCycleMid);
		// Start Motors
		DCMotor_On();
		DCMotor_Modify(dcDutyCycle);
			
		// Main Loop
		while(move){
			/* ***** MOTOR PID ***** */
			// Get PID new Duty Cycle
			//dcDutyCycle = runMotors_PID(pid_controlDC, dcWantedDuty);
			// Update motor with new duty cycle
			// Print PID Values
			//sprintf(str, "Motor PID: %f", dcDutyCycle);
			//DCMotor_Modify(dcDutyCycle);

			/* ***** SERVO PID ***** */
			// Get the Camera Data
			lineData = getCameraData();
			// Get new servo postition
			servoPos = runServo_PID(&pid_controlServo, wantedServoPos, lineData);
			// get duty cyle from servoPos
			sDutyCycle = getDutyCycleFromPos(servoPos);
			// Update servo with new duty cycle
			// Print PID Values
			//sprintf(str, "Servo PID: %f, new Pos: %d\r\n", sDutyCycle, servoPos);
			//uart0_put(str);
			//for(i=0;i<100000;i++);
			Servo_Modify(sDutyCycle);	

			/* ***** CARPET CHECK ***** */
			onCarpet = checkOnCarpet(lineData);
			// Use carpet check data
			switch (onCarpet){
				case 1:
					carpetCount++;
					// Check for atleast 3 carpet checks
					if(carpetCount >= 3){
						move = FALSE;
					}
				case 0:
					// Reset carpet count if dont see carpet
					carpetCount = 0;
			}
		} /* End Main Loop */
		DCMotor_Off();
}
