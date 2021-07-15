/**************************************************************
* Class: CSC-615-01 Spring 2021
* Name: Jennifer Finaldi
* Student ID: 
* Github ID: 
* Project: Drive On
*
* File: Lidar.h
*
* Description: Header file for the Lidar driver module
*
**************************************************************/
#ifndef _LIDAR_H
#define _LIDAR_H

#include "Include.h"

typedef struct Lidar {
    float angles[180][180];
} Lidar;

void init_lidar(char* path); // initialize lidar variables (might not need)
void update();     // update the lidar angle distances from pipe
void visualize();  // output a visualization of obstacles near car

#endif