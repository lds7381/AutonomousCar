/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: Header file for DCMotor helper class.
*/

#include "msp.h"
#include "uart.h"
#include "TimerA.h"

void DCMotor_Init_A0(uint16_t period, double dutyCycle, uint16_t pin);
void DCMotor_Init_A2(uint16_t period, double dutyCycle, uint16_t pin);
void DCMotor_Modify_A0(double dutyCycle, uint16_t pin);
void DCMotor_Modify_A2(double dutyCycle, uint16_t pin);