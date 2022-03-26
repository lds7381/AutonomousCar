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
#include "ControlPins.h"
#include "CortexM.h"
#include "msp432p4011.h"

// Camera varaibles (externed in Control Pins, will be modified there)
uint16_t line[128];
BOOLEAN dataAvaliable = FALSE;

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
}