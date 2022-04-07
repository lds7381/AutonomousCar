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
		uint16_t dcPeriod = 10000;    // Run at 10kHz Period
    uint32_t sPeriod  = CalcPeriodFromFrequency(50);
		uint16_t *lineData;
		uint16_t *smoothLine;
		int carpetCount 	= 0;
		int onCarpet 			= 0;
		int turnDir;
		BOOLEAN move = TRUE;
    
    // Motor Control Variables
    double dcDutyCycle     = 0;       		// Duty Cycle (0 so motors do not move, needed for both motors, will run at same duty cycle always)
		double dcDutyCycleTurn = 0.34;				// Turn speed for DC motors
    double sDutyCycleMid 	 = 0.0497;
		double slightLeft      = 0.0517;
		double slightRight     = 0.0473; 
		
		// Current unused Vars
		double sDutyCycleR  = 0.0471;
    double sDutyCycleL = 0.0521;
//		double currentTurn;
//		int avgLineData = 0;
//		char turnstr[10];
		double leftAvg;
		double rightAvg;
		double midAvg;
//		uint16_t totAvg;
//    int sum = 0;
    char str[32];				// String used for uart printing
		int i;
		
    // **** Initalization of Peripherals ****
    // Init Uart0
    uart0_init();
    // Init ADC
    ADC0_InitSWTriggerCh6();
    // Init DC Motors
    DCMotor_Init(dcPeriod, dcDutyCycle);  // Motor One
    // Init Servo Motor
    Servo_Init(sPeriod, 0);  // Servo One (only one)
    // Init Camera
    LineScanCamera_Init();
    // Init OLED
    //OLED_Init();
		//OLED_Print(1, 1, (char *)"Hello World?");
    // **************************************
		
//		while(1){
//			lineData = getCameraData();
//			smoothLine = smoothData(lineData);
//			leftAvg = getLeftAverage(smoothLine);
//			rightAvg = getRightAverage(smoothLine);
//			midAvg = getMidAverage(smoothLine);
//			sprintf(str, "Left Avg: %f\t", leftAvg);
//			uart0_put(str);
//			sprintf(str, "Mid Avg: %f\t", midAvg);
//			uart0_put(str);
//			sprintf(str, "Right Avg: %f\n\r", rightAvg);
//			uart0_put(str);
//			for(i=0;i<1000500;i++);
//		}
		
		uart0_put("Deeba is going!\n\r");
		//Set duty 
		dcDutyCycle = 0.27;
		
		// **** Test Area ****
		// Test Motor Foward Functionality
		//testMotorForward(dcDutyCycle);
//		while(1){		// hold here
//			lineData = getCameraData();
//			displayCameraData(lineData);
//			for(i=0;i<100000;i++);
//		}
		
		///*** MAIN CODE ****
		
		// Set servo striaght
		Servo_Modify(sDutyCycleMid);
		
		// Start Motors
		DCMotor_On();
		DCMotor_Modify(dcDutyCycle);
		
		
		// Main Loop
		while(move){
			// Get the Camera Data
			lineData = getCameraData();
			// Compare Left and Right Camera data to see if turn needed
			turnDir = computeTurn(lineData);
			// Use Compare to decide turn
			switch (turnDir){
				case 0:			// Staight
					DCMotor_Modify(dcDutyCycle);
					Servo_Modify(sDutyCycleMid);
					break;
				case 1:			// Slight Left
					DCMotor_Modify(dcDutyCycleTurn);
					Servo_Modify(slightLeft);
					break;
				case 2:			// Slight Right
					DCMotor_Modify(dcDutyCycleTurn);
					Servo_Modify(slightRight);
					break;
				case 3:			// Hard Left
					DCMotor_Modify(dcDutyCycleTurn);
					Servo_Modify(sDutyCycleL);
					break;
				case 4:			// Hard Right
					DCMotor_Modify(dcDutyCycleTurn);
					Servo_Modify(sDutyCycleR);
					break;
				case 5:			// Bad Data
					DCMotor_Modify(dcDutyCycle);
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
