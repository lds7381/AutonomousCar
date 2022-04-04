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
    uint32_t sPeriod = CalcPeriodFromFrequency(50);
    
    // Motor Control Variables
    double dcDutyCycle = 0;       		// Duty Cycle (0 so motors do not move, needed for both motors, will run at same duty cycle always)
		double dcDutyCycleTurn = 0.16;		// Turn speed for DC motors
    double sDutyCycleMid = 0.0497;
		double slightLeft = 0.0512;
		double slightRight = 0.0473;
		int carpetCount = 0;
		int i;
		uint16_t *lineData;
		int compare;
		int onCarpet = 0;
		
		// Current unused Vars
//		double sDutyCycleR  = 0.0471;
//    double sDutyCycleL = 0.0521;
//		double currentTurn;
//		int avgLineData = 0;
//		char turnstr[10];
//		uint16_t leftAvg;
//		uint16_t rightAvg;
//		uint16_t midAvg;
//		uint16_t totAvg;
//    int sum = 0;
//    char str[32];				// String used for uart printing
		
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
    OLED_Init();
    // **************************************
		
		uart0_put("Deeba is going!\n\r");
		dcDutyCycle = 0.2;
		
		DCMotor_On();
		LeftMotorForward(dcDutyCycle);
		for(i=0;i<10000000;i++);
		RightMotorForward(dcDutyCycle);
		for(i=0;i<10000000;i++);
		DCMotor_Modify(dcDutyCycle);
		for(i=0;i<10000000;i++);
		DCMotor_Off();
		
		while(1);		// hold here
		
		
		///*** MAIN CODE ****
		
		// Set servo striaght
		Servo_Modify(sDutyCycleMid);
		// Start Motors
		DCMotor_Modify(dcDutyCycle);
		DCMotor_On();
		
		// Main Loop
		while(1){
			// Get the Camera Data
			lineData = getCameraData();
			// Compare Left and Right Camera data to see if turn needed
			compare = compareLeftRight(lineData);
			// Use Compare to decide turn
			switch (compare){
				case -1:		// Left Heavy, Turn right
					DCMotor_Modify(dcDutyCycleTurn);
					Servo_Modify(slightRight);
					break;
				case 1:			// Right Heavy, Turn left
					DCMotor_Modify(dcDutyCycleTurn);
					Servo_Modify(slightLeft);
					break;
				case 0:			// Equal sides, keep straight
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
					if(carpetCount >= 3){
						DCMotor_Off();
					}
					break;
				case 0:
					// Reset carpet count if dont see carpet
					carpetCount = 0;
					break;
			}
		} /* End Main Loop */
}
