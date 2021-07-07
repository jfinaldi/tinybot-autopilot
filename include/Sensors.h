/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: 
* Student ID: 
* Github ID: 
* Project: Drive On
*
* File: Sensors.h
*
* Description: Header file for manipulating sensors in
*           Sensor.c. Handles Infrared Obstacle Avoidance 
*           Sensor and Line Sensor. 
* Code Attributions:         
*
**************************************************************/
#ifndef SENSORS_H
#define SENSORS_H
// #include <stdint.h>
#include "Include.h"

// #define LINESENSOR1        21  //Extreme Left 
// #define LINESENSOR2	      26  //Mid Left     
// #define LINESENSOR3        16  //Mid Center
// #define LINESENSOR4        20  //Mid Right
// #define LINESENSOR5        19  //Extreme Right 

// #define IRSENSOR1           13  //Extreme Left 
// #define IRSENSOR2           5  //Mid Left
// #define IRSENSOR3           6  //Mid Right 
// #define IRSENSOR4           12 //Extreme Right 

// typedef struct{
//     int lineFarLeft;
//     int lineLeft;
//     int lineMiddle; 
//     int lineRight;
//     int lineFarRight;
    
//     int obstacleFarLeft;
//     int obstacleLeft;
//     int obstacleRight;
//     int obstacleFarRight;
// }sensor_data;
// sensor_data * data;

void sensors_init();
void getIrSensorData(int);
void getLineSensorData(int);

#endif