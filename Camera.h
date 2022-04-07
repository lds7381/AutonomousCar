/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: Header file for Camera helper class.
*/

#include <stdio.h>
#include <stdint.h>

void LineScanCamera_Init(void);
uint16_t* getCameraData(void);
uint16_t* smoothData(uint16_t* line);
double getLeftAverage(uint16_t* line);
double getRightAverage(uint16_t* line);
int computeTurn(uint16_t* line);
double getMidAverage(uint16_t* line);
uint16_t getTotalAverage(uint16_t* line);
int checkOnCarpet(uint16_t* line);
void displayCameraData(uint16_t* line);
float getPostionFromLineData(uint16_t* lineData);
float runMotors_PID(pid_t* pidControl, float desiredDutyCycle);
