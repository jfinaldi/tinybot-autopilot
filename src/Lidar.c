/**************************************************************
* Class: CSC-615-01 Spring 2021
* Name: Jennifer Finaldi
* Student ID: 
* Github ID: 
* Project: Drive On
*
* File: Lidar.c
*
* Description: Implementation file for the Lidar driver module
*
**************************************************************/
#include "Include.h"

void init_lidar(char* path) {
    // update fifo path global variable
    fifo_path = path;
}