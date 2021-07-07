/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Jennifer Finaldi
* Student ID: 920290420
* Github ID: jfinaldi
* Project: Drive On
*
* File: Motor.h
*
* Description: Header file for all motor routines in
*           Motor.c. Handles basic functionality such as
*           rotate forwards and backwards, accelerate, 
*           decelerate, and stop.
*
* Code Attributions: This code was modified from demo code
*               for the motor driver HAT from Waveshare
*          
*           Motor code: https://www.waveshare.com/wiki/
*               File:Motor_Driver_HAT_Code.7z         
*
**************************************************************/
#ifndef _MOTOR_H
#define _MOTOR_H

// // libraries
// #include <stdio.h>      
// #include <stdlib.h>

// // other header files
// #include "PCA9685.h"
// #include "DEV_Config.h"
#include "Include.h"

// Global Constants
// #define MOTOR_A 1
// #define MOTOR_B 0

// #define ON 1                  // general on/off triggers
// #define OFF 0

// #define FORWARD 1
// #define BACKWARD 0

// #define MAX_FREQ 100          // for motor speed
// #define MIN_FREQ 0

// Function prototypes
void motor_init();
void setupForward(int, int);
void setupBackward(int, int);
int accelerate(int, int, int);
int decelerate(int, int, int);
void stopMotor(int);

#endif