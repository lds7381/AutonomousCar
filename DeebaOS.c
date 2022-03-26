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
    
    // Motor Control Variables
    double dcDutyCycle = 0;       // Duty Cycle (0 so motors do not move, needed for both motors, will run at same duty cycle always)
    uint16_t dcPeriod = 10000;    // Run at 10kHz Period
    uint16_t motor1Pin = 1;     // Motor 1 on Pin 1 (P2.4)
    uint16_t motor2Pin = 4;     // Motor 2 on Pin 4 (P2.7)
    double sDutyCycle = 0;
    uint16_t sPeriod = 5000;
    uint16_t servoPin = 1;

    // **** Initalization of Peripherals ****
    // Init Uart0
    uart0_init();
    // Init ADC
    ADC0_InitSWTriggerCh6();
    // Init DC Motors
    DCMotor_Init_A0(dcPeriod, dcDutyCycle, motor1Pin);  // Motor One
    DCMotor_Init_A0(dcPeriod, dcDutyCycle, motor1Pin);  // Motor Two
    // Init Servo Motor
    Servo_Init(sPeriod, sDutyCycle, servoPin);  // Servo One (only one)
    // Init Camera
    LineScanCamera_Init();
    // Init OLED
    OLED_Init();
    // **************************************

    // Main Loop to run the car
    while(1){

    }
}