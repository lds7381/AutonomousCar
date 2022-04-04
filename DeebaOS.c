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
		char str[32];				// String used for uart printing
    
    // Motor Control Variables
    double dcDutyCycle = 0;       		// Duty Cycle (0 so motors do not move, needed for both motors, will run at same duty cycle always)
		double dcDutyCycleTurn = 0.16;		// Turn speed for DC motors
    double sDutyCycleMid = 0.0497;
    double sDutyCycleR  = 0.0471;
    double sDutyCycleL = 0.0521;
		double slightLeft = 0.0512;
		double slightRight = 0.0473;
		double currentTurn;
		int carpetCount = 0;
		int i;
		int avgLineData = 0;
		uint16_t *lineData;
		int compare;
	  char turnstr[10];
		char oled[6] = "Deeba";
		uint16_t leftAvg;
		uint16_t rightAvg;
		uint16_t midAvg;
		uint16_t totAvg;
		int sum = 0;
		int onCarpet = 0;
		
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
    // **************************************
		
		uart0_put("Deeba is going!\n\r");
		dcDutyCycle = 0.14;
		
		// Set servo striaght
		Servo_Modify(sDutyCycleMid);
		
		// Start Motor
		DCMotor_Modify(dcDutyCycle);
		DCMotor_On();
		
		while(1){
			lineData = getCameraData();
			compare = compareLeftRight(lineData);
			if(compare == -1){
					DCMotor_Modify(dcDutyCycleTurn);
					Servo_Modify(slightRight);
			}
			else if (compare == 1) {
					DCMotor_Modify(dcDutyCycleTurn);
					Servo_Modify(slightLeft);
			}
			else {
					DCMotor_Modify(dcDutyCycle);
					Servo_Modify(sDutyCycleMid);
			}
			onCarpet = checkOnCarpet(lineData);
			if(onCarpet == 1){
					carpetCount++;
					if(carpetCount >= 3){
						DCMotor_Off();
					}
			} else {
					carpetCount = 0;
			}
			
		}
		
		
//			midAvg = getMidAverage(lineData);
//			if(midAvg <= 1000){
//						DCMotor_Off();
//			} 
//************************			
//			totAvg = getTotalAverage(lineData);
//			if ( totAvg < 450){
//					carpetCount++;
//					if (carpetCount >= 5){
//						DCMotor_Off();
//					}
//			}
			
//			for (i=0; i<128; i++){
//					avgLineData += lineData[i];
//			}
//			avgLineData /= 128;
//			sprintf(str, "Avg: %d\n\r", avgLineData);
//			uart0_put(str);
//			
//			if (avgLineData >= 16000){
//				Servo_Modify(sDutyCycleMid);
//			}
//			else if(avgLineData < 16000 && avgLineData > 15500){
////				if(currentTurn + 0.004 >= sDutyCycleL){	
////					currentTurn += 0.001;	
////					Servo_Modify(currentTurn);
////				}
//				Servo_Modify(.0516);
//			}
//			else if (avgLineData < 15000){
//					DCMotor_Off();
//			}
		}
