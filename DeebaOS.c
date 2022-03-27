/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: Main file for Operating system for Deeba OS.
                 Runs the entire autnomous car.
*/
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "oled.h"
#include "ADC14.h"
#include "MotorControl.h"
#include "Camera.h"
#include "msp432p4011.h"
#include "CortexM.h"

int main(void) {
    // Main Variables
    BOOLEAN fastMode = TRUE;
    BOOLEAN speedChange = TRUE;  // Car will need to Change speed of start (starts stopped)
    
    // Motor Control Variables
    double dcDutyCycle = 0;       // Duty Cycle (0 so motors do not move, needed for both motors, will run at same duty cycle always)
    uint16_t dcPeriod = 10000;    // Run at 10kHz Period
    uint16_t sPeriod = 5000;
    double sDutyCycleMid = .6025;
    double sDutyCycleCW  = .205;
    double sDutyCycleCCW = .99;
		int i;
		uint16_t *lineData;
	  int avgLineData = 0;
	  char str[10];
		char oled[6] = "Deeba";

    // **** Initalization of Peripherals ****
    // Init Uart0
    uart0_init();
    // Init ADC
    ADC0_InitSWTriggerCh6();
    // Init DC Motors
    DCMotor_Init(dcPeriod, dcDutyCycle);  // Motor One
    // Init Servo Motor
    Servo_Init(sPeriod, sDutyCycleMid);  // Servo One (only one)
    // Init Camera
    LineScanCamera_Init();
    // Init OLED
    OLED_Init();
    // **************************************
		
		uart0_put("Deeba is going!\n\r");
		dcDutyCycle = 0.2;
		OLED_display_on();
		OLED_PrintLine(oled);
		
    // Main Loop to run the car
    while(1){
			DCMotor_Modify(dcDutyCycle);  // Run the car forward at a 20% duty Cycle
			Servo_Modify(sDutyCycleMid); 
			for(i=0; i<10000000; i++);
			dcDutyCycle = 0.5;
			DCMotor_Modify(dcDutyCycle);  // Run the car forward at a 50% duty Cycle
			Servo_Modify(sDutyCycleCCW); 
      for(i=0; i<10000000; i++);
			dcDutyCycle = 0.9;
			DCMotor_Modify(dcDutyCycle);  // Run the car forward at a 50% duty Cycle
			Servo_Modify(sDutyCycleCW); 
			for(i=0; i<10000000; i++);
		  lineData = getCameraData();
			for (i=0; i<128; i++){
					avgLineData += lineData[i];
			}
			avgLineData /= 128;
			sprintf(str, "Avg: %d\n\r", avgLineData);
			uart0_put(str);
		}
}
