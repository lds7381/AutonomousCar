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
#include "Camera.h"
#include "ControlPins.h"
#include "CortexM.h"
#include "msp432p4011.h"
#include "uart.h"
#include "oled.h"

// Camera varaibles (externed in Control Pins, will be modified there)
uint16_t line[128];
uint16_t smoothLine[128];
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

uint16_t * smoothData(uint16_t* line){
		int i;
		for (i=2; i<127; i++){
			smoothLine[i] = line[i-2] + line[i-1] + line[i] + line[i+1] + line[i+2];
			smoothLine[i] = smoothLine[i]/5;
		}
		return smoothLine;
	
}

double getLeftAverage(uint16_t* line){
		double averageLeft=0;
		int i;
		for(i=0; i<= 16; i++){
			averageLeft += line[i];
		}
		return averageLeft/17;
}

double getRightAverage(uint16_t* line){
		double averageRight=0;
		int i;
		for(i=112; i<= 128; i++){
			averageRight += line[i];
		}
		return averageRight/17;
}

double getMidAverage(uint16_t* line){
		double averageMid=0;
		int i;
		for(i=50; i<= 78; i++){
			averageMid += line[i];
		}
		return averageMid/29;
}

uint16_t getTotalAverage(uint16_t* line){
		uint16_t averageTotal=0;
		int i;
		for(i=0; i<= 128; i++){
			averageTotal += line[i];
		}
		return averageTotal/128;
}

edges_t getPostionFromLineData(uint16_t* lineData){
	int i;
	int edgeTolerance = 6000;	// minimum value for the edge to be detected
	edges_t edges;
	BOOLEAN leftEdge = FALSE;
	BOOLEAN rightEdge = FALSE;

	// Check Left and Right sides to check if edges are there
//	if(lineData[0] > 2400){
//		edges.leftPos = 0;
//		leftEdge = TRUE;

//	}
//	if(lineData[127] > 2400){
//		edges.rightPos = 127;
//		rightEdge = TRUE;
//	}

//	// If no edges detected mid pos is the center (64)
//	if(leftEdge && rightEdge){
//		edges.midPos = 64;
//		return edges;	
//	}
//	else{
		// Iterate though the full line data to get left edge
		if(!leftEdge){
//			uart0_put("L: ");
			for(i=0;i<128;i++){
//				if(i<10){
//					sprintf(str, "%d-", lineData[i]);
//					uart0_put(str);
//				}
				if ((lineData[i] > edgeTolerance) && !leftEdge){
					edges.leftPos = (i+1); 
					leftEdge = TRUE;
					break;
				}
			}
			if (!leftEdge){
				edges.leftPos = 0;
			}
		}
		// Iterate backwards through line data to get right edge
		if(!rightEdge){
//			uart0_put("R: ");
			for(i=126;i>=0;i--){
//				if(i>117){
//					sprintf(str, "%d-", lineData[i]);
//					uart0_put(str);
//				}
				if ((lineData[i] > edgeTolerance) && !rightEdge){
					edges.rightPos = (i); 
					rightEdge = TRUE;
					break;
				}
			}
			if (!rightEdge){
				edges.rightPos = 127;
			}
		}
		
		// Right edge overflow
		if(edges.leftPos > edges.rightPos){
				edges.rightPos = 2;
				edges.leftPos = edges.rightPos-2;
				
		}
		
		edges.midPos = (edges.leftPos+edges.rightPos)/2;
		return edges;
}

int computeTurn(uint16_t* line){
		double leftAvg = getLeftAverage(line);
		double rightAvg = getRightAverage(line);
		double midAvg   = getMidAverage(line);
		int turnDir = 0;
//		int i;
//		sprintf(str, "Left Avg: %f\t", leftAvg);
//		uart0_put(str);
//		sprintf(str, "Right Avg: %f\n\r", rightAvg);
//		uart0_put(str);
//		for(i=0;i<100000;i++);
//
		// Control Logics
		if((rightAvg > 4750) && (leftAvg > 4750)){ 
				turnDir = 0;
				return turnDir;
		}
		else if(/*(leftAvg > 5000) &&*/ (rightAvg < 2400)) {   // was != 2047
				turnDir = 1;
				return turnDir;
		}
		else if (/*(rightAvg > 5000) &&*/ (leftAvg < 2400)){
				turnDir = 2;
				return turnDir;
		}
		else if(/*(leftAvg > 5000) &&*/ (rightAvg < 1600)){
				turnDir = 3;
				return turnDir;
		}
		else if (/*(rightAvg > 5000) &&*/ (leftAvg < 1800)){
				turnDir = 4;
				return turnDir;
		}
		else {
				turnDir = 5;
				return turnDir;
		}
}

int checkOnCarpet(uint16_t* line){
		uint16_t carpet;
		carpet = getMidAverage(line);
		//sprintf(str, "Mid Avg: %d\t", mid);
		//uart0_put(str);
		if (carpet<1400){
				return 1;
		}
		return 0;
}

void displayCameraData(uint16_t* line){
		OLED_DisplayCameraData(line);
}
