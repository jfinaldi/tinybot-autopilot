/**************************************************************
* Class: CSC-615-01 Spring 2021
* Name: 
* Student ID: 
* Github ID: 
* Project: Drive On
*
* File: Sensors.c
*
* Description: Implementation file for all sensor routines in
*           Sensors.h. Takes readings that are output from 
*           line and infrared sensors. 
*
**************************************************************/
#include "Include.h"

void sensors_init() {
    // Line Sensors
    pinMode(LINESENSOR_A, INPUT); // A, far left lateral
    pinMode(LINESENSOR_B, INPUT); // B, middle left
    pinMode(LINESENSOR_C, INPUT); // C, middle center
    pinMode(LINESENSOR_D, INPUT); // D, middle right
    pinMode(LINESENSOR_E, INPUT); // E, far right lateral
}

void getLineSensorData(int sensor) {
   // labels for debugging output
    char s = 'Z';
    if(sensor == LINESENSOR_A) s = 'A';
    if(sensor == LINESENSOR_B) s = 'B';
    if(sensor == LINESENSOR_C) s = 'C';
    if(sensor == LINESENSOR_D) s = 'D';
    if(sensor == LINESENSOR_E) s = 'E';

    while(ON) {
        // if low pulse on sensor, we are over a line
        if(digitalRead(sensor) == ON) {
            if(sensor == LINESENSOR_A) onLine_A = ON;
            if(sensor == LINESENSOR_B) onLine_B = ON;
            if(sensor == LINESENSOR_C) onLine_C = ON;
            if(sensor == LINESENSOR_D) onLine_D = ON;
            if(sensor == LINESENSOR_E) onLine_E = ON;
            delay(100);
        }
        else {
            if(sensor == LINESENSOR_A) onLine_A = OFF;
            if(sensor == LINESENSOR_B) onLine_B = OFF;
            if(sensor == LINESENSOR_C) onLine_C = OFF;
            if(sensor == LINESENSOR_D) onLine_D = OFF;
            if(sensor == LINESENSOR_E) onLine_E = OFF;
            delay(100);
        }
    }
}
