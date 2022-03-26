/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: Main file for Operating system for Deeba OS.
                 Runs the entire autnomous car.
*/
#include <stdio.h>
#include <stdlib.h>
#include "msp.h"
#include "uart.h"
#include "oled.h"
#include "ADC14.h"
#include "MotorControl.h"
#include "Camera.h"

int main(void) {
    // Main Variables
    BOOLEAN fastMode = TRUE;
    BOOLEAN speedChange = TRUE;  // Car will need to Change speed of start (starts stopped)
    
    // Motor Control Variables
    double dcDutyCycle = 0;       // Duty Cycle (0 so motors do not move, needed for both motors, will run at same duty cycle always)
    uint16_t dcPeriod = 10000;    // Run at 10kHz Period
    double sDutyCycle = 0;
    uint16_t sPeriod = 5000;

    // **** Initalization of Peripherals ****
    // Init Uart0
    uart0_init();
    // Init ADC
    ADC0_InitSWTriggerCh6();
    // Init DC Motors
    DCMotor_Init_A0(dcPeriod, dcDutyCycle);  // Motor One
    DCMotor_Init_A0(dcPeriod, dcDutyCycle);  // Motor Two
    // Init Servo Motor
    Servo_Init(sPeriod, sDutyCycle);  // Servo One (only one)
    // Init Camera
    LineScanCamera_Init();
    // Init OLED
    OLED_Init();
    // **************************************

    // Main Loop to run the car
    while(1){
        if(speedChange){
            dcDutyCycle = 0.2;
        }

    }
}