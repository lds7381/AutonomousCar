/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: C file for DCMotor helper class.
                 Will Init and Run DCMotors.
*/

// Initalization Function with paramters (will start the motor at required cycle)
// Initalizes TimerA0
void DCMotor_Init_A0(uint16_t period, double dutyCycle, uint16_t pin){
    int ret = TIMER_A0_PWM_Init(period, dutyCycle, pin);
        if (ret != 0){
            uart0_put("Warning: TIMERA0 Failed to Initialize!");
		}
}

// Initalization Function with paramters (will start the motor at required cycle)
// Initalizes TimerA2
void DCMotor_Init_A2(uint16_t period, double dutyCycle, uint16_t pin){
    int ret = TIMER_A2_PWM_Init(period, dutyCycle, pin);
        if (ret != 0){
            uart0_put("Warning: TIMERA2 Failed to Initialize!");
		}
}

// Changes the Active Duty Cycle of the PWM for the DC Motor on TimerA0
void DCMotor_Modify_A0(double dutyCycle, uint16_t pin){
    TIMER_A0_PWM_DutyCycle(dutyCycle, pin);
}

// Changes the Active Duty Cycle of the PWM for the DC Motor on TimerA2
void DCMotor_Modify_A2(double dutyCycle, uint16_t pin){
    TIMER_A2_PWM_DutyCycle(dutyCycle, pin);
}
