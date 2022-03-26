/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: Main file for Operating system for Deeba OS.
                 Runs the entire autnomous car.
*/

#include "msp.h"
#include "uart.h"
#include "TimerA.h"
#include "Timer32.h"
#include "ControlPins.h"
#include "CortexM.h"
#include "i2c.h"
#include "oled.h"
#include "Switches.h"
#include "SysTickTimer.h"
#include "ADC14.h"
#include "DCMotor.h"

int main(void) {
    // Main Variables
    
    // DCMotor Variables
    double dutyCycle = 0;       // Duty Cycle (0 so motors do not move, needed for both motors, will run at same duty cycle always)
    uint16_t period = 10000;    // Run at 10kHz Period
    uint16_t motor1Pin = 1;     // Motor 1 on Pin 1 (P2.4)
    uint16_t motor2Pin = 4;     // Motor 2 on Pin 4 (P2.7)


    // **** Initalization of Peripherals ****
    // Init Uart0
    uart0_init();
    // Init ADC
    ADC0_InitSWTriggerCh6();
    // Init DC Motors
    DCMotor_Init_A0(period, dutyCycle, motor1Pin);  // Motor One
    DCMotor_Init_A0(period, dutyCycle, motor1Pin);  // Motor Two
    // Init Servo Motor

    // Init Camera

    // Init OLED


    // Main Loop to run the car
    while(1){

    }
}