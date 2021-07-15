/**************************************************************
* Class: CSC-615-01 Spring 2021
* Name: 
* Student ID: 
* Github ID: 
* Project: Drive On
*
* File: Include.h
*
* Description: Main header file to consolidate all include
*           statements, global variables, and structures into 
*           one file.
*
**************************************************************/
#ifndef _INCLUDE_H
#define _INCLUDE_H

// Libraries
#include <pthread.h>      // for all threads
#include <stdint.h>       // for DEV_Config.c
#include <stdio.h>        // for printf output
#include <stdlib.h>       // for absolute value function
#include <sys/types.h>    // for mkfifo pipe
#include <sys/stat.h>     // for mkfifo pipe
#include <time.h>         // for echo sensor
#include <wiringPi.h>     // for DEV_Config.c, and everything else
#include <wiringPiI2C.h>  // for DEV_Config.c
#include <wiringPiSPI.h>  // for Speedometer.c SPI comms

// Headers
#include "Debug.h"
#include "DEV_Config.h"
#include "Lidar.h"
#include "Motor.h"
#include "PCA9685.h"
#include "Sensors.h"
#include "Speedometer.h"

// Sensors
extern int LINESENSOR_A;           // Extreme Left 
extern int LINESENSOR_B;           // Mid Left     
extern int LINESENSOR_C;           // Mid Center
extern int LINESENSOR_D;           // Mid Right
extern int LINESENSOR_E;           // Extreme Right 
extern int BUTTON;
extern int US_MULTIPLIER;          // for calculating distance

// Lidar angle distances
extern volatile double a_30;       // 30 degrees to the right
extern volatile double a_60;       // 60 degrees to the right
extern volatile double a_90;       // 90 degrees right
extern volatile double a_270;      // 90 degrees left
extern volatile double a_300;      // 60 degrees to the left
extern volatile double a_330;      // 30 degrees to the left
extern volatile double a_359;      // ~0 degrees straight ahead

// PCA9685 GPIO Config
extern unsigned char SUBADR1;
extern unsigned char SUBADR2;
extern unsigned char SUBADR3;
extern unsigned char MODE1;
extern unsigned char PRESCALE;
extern unsigned char LED0_ON_L;
extern unsigned char LED0_ON_H;
extern unsigned char LED0_OFF_L;
extern unsigned char LED0_OFF_H;
extern unsigned char ALLLED_ON_L;
extern unsigned char ALLLED_ON_H;
extern unsigned char ALLLED_OFF_L;
extern unsigned char ALLLED_OFF_H;
extern int PCA_CHANNEL_0;
extern int PCA_CHANNEL_1;
extern int PCA_CHANNEL_2;
extern int PCA_CHANNEL_3;
extern int PCA_CHANNEL_4;
extern int PCA_CHANNEL_5;
extern int PCA_CHANNEL_6;
extern int PCA_CHANNEL_7;
extern int PCA_CHANNEL_8;
extern int PCA_CHANNEL_9;
extern int PCA_CHANNEL_10;
extern int PCA_CHANNEL_11;
extern int PCA_CHANNEL_12;
extern int PCA_CHANNEL_13;
extern int PCA_CHANNEL_14;
extern int PCA_CHANNEL_15;

// Motor Constants
extern int MOTOR_A;               // right wheel
extern int MOTOR_B;               // left wheel
extern int ON;                    // general on/off triggers
extern int OFF;
extern int LOW_PULSE;
extern int HIGH_PULSE;
extern int FORWARD;
extern int BACKWARD;
extern int MAX_FREQ;              // for motor speed
extern int MIN_FREQ;
extern int DEFAULT_POWER_A;       // default driving speed for A
extern int DEFAULT_POWER_B;       // default driving speed for B

// Speedometer Constants
extern int CE0;                   // Chip Enable 0 channel 0
extern int CE1;                   // Chip Enable 1 channel 1
extern int SPI_CLK_SPD;           // 1MHz clock speed
extern unsigned char WRITE;       // command to write to register
extern unsigned char READ;        // command to read from register
extern unsigned char CLEAR;       // command to clear register
extern unsigned char MDR0;        // MDR0 register
extern unsigned char MDR1;        // MDR1 register
extern unsigned char CNTR;        // Counter register
extern unsigned char BYTES_4;     // 4 byte counter mode (was 0x00)
extern unsigned char ENCNT;       // to enable counter
extern double N;                  // 540 ticks per revolution of motor
extern double PI;                 // Pi 
extern double RADIUS;             // Radius of wheel in cm
extern int MAX_CYCLES;            // 25 counts before calc speed
extern int VEER;                  // scalar for minor maneuvers
extern int AGGRESSIVE;            // scalar for major maneuvers

// Global Variables
extern volatile char* fifo_path;       // path to fifo file for pipe
extern volatile int distance;          // distance of car to obstacle
extern volatile int side_distance;     // distance of left side of car to obstacle
extern volatile double cur_speed_a;    // speed of motor A wheel
extern volatile double cur_speed_b;    // speed of motor B wheel
extern volatile int currentPowerA;     // range 0-100 (percent) Motor.c
extern volatile int currentPowerB;
extern volatile int currentDirectionA; // 1 for forwards, 0 for back Motor.c
extern volatile int currentDirectionB; // 1 for forwards, 0 for backwards
extern volatile int halted;            // 1 for vehicle stopped for obstacle
extern volatile int onLine_A;          // 0 for off the line, 1 for on line
extern volatile int onLine_B;          // 0 for off the line, 1 for on line
extern volatile int onLine_C;          // 0 for off the line, 1 for on line
extern volatile int onLine_D;          // 0 for off the line, 1 for on line
extern volatile int onLine_E;          // 0 for off the line, 1 for on line
extern volatile int car_activated;     // 0 for car off, 1 for car on
extern volatile int atSafeDistance;    // 1 for safe distance, 0 for too close

// structure definitions
typedef struct Args {
    int motor;          // motor A or motor B
    int lineSensor;     // one of the 5 line sensors
    int chipEnable;     // 0 = CE0, 1 = CE1
    int isFrontSensor;  // 1 means front echo sensor, 0 means side
} Args;

// Thread function prototypes
void* buttonThread(void* args);
void* lineThread(void* args);
void* speedThread(void* args);
void* lidarThread(void* args);
void* driveThread(void* args); 
void* distanceThread(void* args);

// Other prototypes
void init(char* path);
void globals_init();
void halt();
void resume();
int veer(int, int, int);
int sharpTurn(int, int, int);
int goAround();
int getUnstuck();

#endif