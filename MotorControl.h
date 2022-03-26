/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: Header file for Motor Control helper class.
*/

#include "msp.h"
#include "uart.h"
#include "TimerA.h"

void DCMotor_Init(uint16_t period, double dutyCycle, uint16_t pin);
void Servo_Init(uint16_t period, double dutyCycle, uint16_t pin);
void DCMotor_Modify(double dutyCycle, uint16_t pin);
void Servo_Modify(double dutyCycle, uint16_t pin);