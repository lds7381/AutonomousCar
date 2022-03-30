/*  Authors:    Liam Sullivan
                Andre DaCosta
    Project:    IDE Autonomous Car
    Creation:   3/25/2022
    Description: C file for Camera helper class.
                 Will Init and Run the Camera, utilizes I2C Protocol.
*/

#include "Common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ControlPins.h"
#include "CortexM.h"
#include "msp432p4011.h"
#include "uart.h"

// Camera varaibles (externed in Control Pins, will be modified there)
uint16_t line[128];
BOOLEAN dataAvaliable = FALSE;
char str[10];

void LineScanCamera_Init(){
    DisableInterrupts();    // Stop Interrupts
	ControlPin_SI_Init();   // SI Pin init on P5.5
	ControlPin_CLK_Init();  // CLK Pin Init on P5.4
    EnableInterrupts();     // Resume Interrupts
}

uint16_t * getCameraData(){
    while(dataAvaliable == FALSE);  // Wait for line data to come in from the camera
    if(dataAvaliable ==TRUE){       // Double Check Data is ready
        dataAvaliable = FALSE;      // Set data Avaiable to false
        return line;                // Send the data out
    }
		return 0;
}

uint16_t getLeftAverage(uint16_t* line){
		uint16_t averageLeft=0;
		int i;
		for(i=0; i<= 32; i++){
			averageLeft += line[i];
		}
		return averageLeft/32;
}

uint16_t getRightAverage(uint16_t* line){
		uint16_t averageRight=0;
		int i;
		for(i=96; i<= 128; i++){
			averageRight += line[i];
		}
		return averageRight/32;
}

uint16_t getMidAverage(uint16_t* line){
		uint16_t averageMid=0;
		int i;
		for(i=33; i<= 95; i++){
			averageMid += line[i];
		}
		return averageMid/32;
}

uint16_t getTotalAverage(uint16_t* line){
		uint16_t averageTotal=0;
		int i;
		for(i=0; i<= 128; i++){
			averageTotal += line[i];
		}
		return averageTotal/128;
}

int compareLeftRight(uint16_t* line){
		int i;
		uint16_t leftAvg = getLeftAverage(line);
		uint16_t rightAvg = getRightAverage(line);
//		sprintf(str, "Left Avg: %d\t", leftAvg);
//		uart0_put(str);
//		sprintf(str, "Right Avg: %d\n\r", rightAvg);
//		uart0_put(str);
//		for(i=0;i<100000;i++);
//		
		if(leftAvg != 510 ){ 
				return -1;
		}
		else if(rightAvg > 900 || rightAvg < 600) {   // was != 2047
				return 1;
		}
		else {
			return 0;
		}
		return 0;
}