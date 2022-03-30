/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: Header file for Camera helper class.
*/

#include <stdio.h>
#include <stdint.h>

void LineScanCamera_Init(void);
uint16_t* getCameraData();
uint16_t getLeftAverage(uint16_t* line);
uint16_t getRightAverage(uint16_t* line);
int compareLeftRight(uint16_t* line);
uint16_t getMidAverage(uint16_t* line);
uint16_t getTotalAverage(uint16_t* line);
int checkOnCarpet(uint16_t* line);