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
		uint16_t dcPeriod 	= 10000;    // Run at 10kHz Period
    	uint32_t sPeriod  	= CalcPeriodFromFrequency(50);
		uint16_t *lineData;
		uint16_t *smoothLine;
		int carpetCount 	= 0;
		int onCarpet 		= 0;
		int turnDir;
		BOOLEAN move 		= TRUE;
		int i;
		char str[32];				// String used for uart printing
    
    // Motor Control Variables
    	double dcDutyCycle     = 0;       		// Duty Cycle (0 so motors do not move, needed for both motors, will run at same duty cycle always)
		double dcWantedDuty    = 0.23;
		double dcDutyCycleTurn = 0.34;			// Turn speed for DC motors
   		double sDutyCycleMid   = 0.0497;
		double slightLeft      = 0.0517;
		double slightRight     = 0.0473; 
		double sDutyCycleR  = 0.0471;
    	double sDutyCycleL = 0.0521;
		double leftAvg;
		double rightAvg;
		double midAvg;
		pid_t pid_control;
			
		// Current unused Vars
//		double currentTurn;
//		int avgLineData = 0;
//		char turnstr[10];
//		uint16_t totAvg;
//    	int sum = 0;
    	
		
    // **** Initalization of Peripherals ****
    // Init Uart0
    uart0_init();
    // Init ADC
    ADC0_InitSWTriggerCh6();
    // Init DC Motors
    DCMotor_Init(dcPeriod, 0);  
	for(i=0;i<100000;i++);		// Give a delay for PWM to be set up
    // Init Servo Motor
    Servo_Init(sPeriod, 0);  // Servo One (only one)
    // Init Camera
    LineScanCamera_Init();
    // Init OLED
    //OLED_Init();
		//OLED_Print(1, 1, (char *)"Hello World?");
    // **************************************		

	///*** MAIN CODE ****
	// Initalize PID Control (min=0.15, max=0.30, ki=0.1, kp=0.4, kd=0.6)
	PID_Init(pid_control, 0.15, 0.30, 0.1, 0.4, 0.6);
	// Start Running the Car
	uart0_put("Deeba is going!\n\r");
	// Set servo striaght
	Servo_Modify(sDutyCycleMid);
	// Start Motors
	DCMotor_On();
	DCMotor_Modify(dcWantedDuty);
		
	// Main Loop
	while(move){
		// Get PID new Duty Cycle
		dcDutyCycle = runMotors_PID(PID_Init, dcWantedDuty);
		// Update motor with new duty cycle
		DCMotor_Modify(dcDutyCycle);

		// Get the Camera Data
		lineData = getCameraData();
		// Compare Left and Right Camera data to see if turn needed
		turnDir = computeTurn(lineData);
		// Use Compare to decide turn
		switch (turnDir){
			case 0:			// Staight
				Servo_Modify(sDutyCycleMid);
				break;
			case 1:			// Slight Left
				Servo_Modify(slightLeft);
				break;
			case 2:			// Slight Right
				Servo_Modify(slightRight);
				break;
			case 3:			// Hard Left
				Servo_Modify(sDutyCycleL);
				break;
			case 4:			// Hard Right
				Servo_Modify(sDutyCycleR);
				break;
			case 5:			// Bad Data
				Servo_Modify(sDutyCycleMid);
				break;
		}
		
		// Carpet Check
		onCarpet = checkOnCarpet(lineData);
		// Use carpet check data
		switch (onCarpet){
			case 1:
				carpetCount++;
				// Check for atleast 3 carpet checks
				if(carpetCount >= 5){
					move = FALSE;
				}
				break;
			case 0:
				// Reset carpet count if dont see carpet
				carpetCount = 0;
				break;
		}
		//for(i=0;i<10000;i++);
		// Display camera data on OLED
		//displayCameraData(lineData);
		
	} /* End Main Loop */
	DCMotor_Off();
}
