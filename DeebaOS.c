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
    BOOLEAN fastMode = TRUE;
    BOOLEAN speedChange = TRUE;  // Car will need to Change speed of start (starts stopped)
    
    // Motor Control Variables
    double dcDutyCycle = 0;       // Duty Cycle (0 so motors do not move, needed for both motors, will run at same duty cycle always)
    uint16_t dcPeriod = 10000;    // Run at 10kHz Period
    uint32_t sPeriod = CalcPeriodFromFrequency(50);
		char str[32];
    double sDutyCycleMid = .6;
    double sDutyCycleCW  = .205;
    double sDutyCycleCCW = .99;
		double turnIncrement = 0.0471;
		int i;
		uint16_t *lineData;
	  int avgLineData = 0;
	  char turnstr[10];
		char oled[6] = "Deeba";

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
		dcDutyCycle = 0.3;
		//OLED_display_on();
		//OLED_PrintLine(oled);
		
		//DCMotor_Modify(dcDutyCycle);  // Run the car forward at a 20% duty Cycle
		//Servo_Modify(turnIncrement);
		//for(i=0; i<1000000; i++);
		//Servo_Modify(sDutyCycleCCW); 
		//for(i=0; i<1000000; i++);
		//Servo_Modify(sDutyCycleMid);
		//for(i=0; i<1000000; i++);
		//Servo_Modify(sDutyCycleCW);
		
		//DCMotor_On();
		//uart0_put("****DCMotor**** ON\n\r");
		
    // Main Loop to run the car
    while(1){
			while (turnIncrement <= .0522){
					Servo_Modify(turnIncrement);
					sprintf(turnstr, "Duty Cycle=%f\n\r", turnIncrement);
					uart0_put(turnstr);
					for(i=0;i<1000000;i++);
					turnIncrement += .0001;
			}
			Servo_Modify(.0496);
				//			for(i=0; i<10000000; i++){
//				if (i % 100000 == 0){
//					 uart0_put("waiting on\n\r");
//				}
//				if (i == 10000000-1){
//						uart0_put("waiting over\n\r");
//				}
//			}
//			DCMotor_Off();
//			uart0_put("****Motor off****\n\r");
			for(i=0; i<10000000; i++);
//		  lineData = getCameraData();
//			for (i=0; i<128; i++){
//					avgLineData += lineData[i];
//			}
//			avgLineData /= 128;
//			sprintf(str, "Avg: %d\n\r", avgLineData);
//			uart0_put(str);
		}
}
