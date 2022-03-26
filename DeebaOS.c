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

int main(void) {

    // **** Initalization of Peripherals ****
    uart0_init();
    ADC0_InitSWTriggerCh6();
    

    // Main Loop to run the car
    while(1){

    }
}