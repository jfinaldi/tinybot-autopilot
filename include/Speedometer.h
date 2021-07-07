/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Team Shartcar
* Student ID: 
* Github ID: 
* Project: Drive On
*
* File: Speedometer.h
*
* Description: Header file for all routines in
*           Speedometer.c. Handles the SPI communications 
*           between the Raspberry Pi and the 
*           quadrature counter. Calculates the speed in 
*           cm/s    
*
**************************************************************/
#ifndef _SPEEDOMETER_H
#define _SPEEDOMETER_H

#include "Include.h"

void speedometer_init();
void readCounter(int, int);

#endif
