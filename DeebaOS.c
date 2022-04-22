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
    
    // Motor Control Variables
    double dcDutyCycle     = 0.27;       		// Duty Cycle (0.27 best for turns)
	double maxDCDutyCycle  = 0.30;	
		double dcWantedDuty    = 0.32;
		int wantedServoPos     = 64;			// Desired Servo Position (64 is straight)
		double sDutyCycle;
		double sDutyCycleMid   = 0.0497;
	 double percent;
		int servoPos;
		pid_t  pid_controlDC;		// Pid Control Variables for DC Motors
		pid_t  pid_controlServo;	// Pid Control Variables for Servo
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
//		int i;
//    char str[32];				// String used for uart printing
		
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
		// Initalize PID Servo Motor Control (min=-2.5, max=2.5, ki=0.12, kp=0.97, kd=0.08)
		PID_Init(&pid_controlServo, -2.5, 2.5, 0.25, 2.0, 0.7);
		// Start Running the Car
		uart0_put("Deeba is going!\n\r");
		// Set servo striaght
		Servo_Modify(sDutyCycleMid);
		// Move Forward
		DCMotor_Modify(dcDutyCycle, dcDutyCycle);
		// Start Motors
		DCMotor_On();
			
		// Main Loop
		while(move){
			/* ***** SERVO PID ***** */
			// Get the Camera Data
			lineData = getCameraData();
			// Get new servo postition
			servoPos = runServo_PID(&pid_controlServo, wantedServoPos, lineData);
			// get duty cyle from servoPos
			sDutyCycle = getDutyCycleFromPos(servoPos);
			// Update servo with new duty cycle
			Servo_Modify(sDutyCycle);	
			
			/* ***** MOTOR CONTROL ***** */
			percent = getDCSpeedFromAngle(sDutyCycle);
			dcDutyCycle = maxDCDutyCycle * percent;
			if(sDutyCycle < .0497){ // turning right
				DCMotor_Modify(dcDutyCycle + .20, dcDutyCycle);
			}
			else if(sDutyCycle > .0497){
				DCMotor_Modify(dcDutyCycle, dcDutyCycle + .20);
			}
			else{
				DCMotor_Modify(dcDutyCycle, dcDutyCycle);
			}


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
