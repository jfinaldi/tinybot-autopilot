/**************************************************************
* Class: CSC-615-01 Spring 2021
* Name: 
* Student ID: 
* Github ID: 
* Project: Drive On
*
* File: Globals.c
*
* Description: Implementation file for storing and initializing
*           all global constants used throughout the program
*
**************************************************************/
#include "Include.h"

// Sensors
int LINESENSOR_A;  // Extreme Left 
int LINESENSOR_B;  // Mid Left     
int LINESENSOR_C;  // Mid Center
int LINESENSOR_D;  // Mid Right
int LINESENSOR_E;  // Extreme Right 

// PCA9685 GPIO config
unsigned char SUBADR1;             
unsigned char SUBADR2;
unsigned char SUBADR3;
unsigned char MODE1;
unsigned char PRESCALE;
unsigned char LED0_ON_L;
unsigned char LED0_ON_H;
unsigned char LED0_OFF_L;
unsigned char LED0_OFF_H;
unsigned char ALLLED_ON_L;
unsigned char ALLLED_ON_H;
unsigned char ALLLED_OFF_L;
unsigned char ALLLED_OFF_H;
int PCA_CHANNEL_0;
int PCA_CHANNEL_1;
int PCA_CHANNEL_2;
int PCA_CHANNEL_3;
int PCA_CHANNEL_4;
int PCA_CHANNEL_5;
int PCA_CHANNEL_6;
int PCA_CHANNEL_7;
int PCA_CHANNEL_8;
int PCA_CHANNEL_9;
int PCA_CHANNEL_10;
int PCA_CHANNEL_11;
int PCA_CHANNEL_12;
int PCA_CHANNEL_13;
int PCA_CHANNEL_14;
int PCA_CHANNEL_15;

// MOTOR
int MOTOR_A;
int MOTOR_B;
int ON;                         // general on/off triggers
int OFF;
int HIGH_PULSE;
int LOW_PULSE;
int FORWARD;
int BACKWARD;
int MIN_FREQ;
int MAX_FREQ;                   // for motor speed
int DEFAULT_POWER_A;            // default driving speed for A
int DEFAULT_POWER_B;            // default driving speed for B
int BUTTON;
int US_MULTIPLIER;              // For calculating dist equation

// SPEEDOMETER
int CE0;                        // Chip Enable 0 channel 0
int CE1;                        // Chip Enable 1 channel 1
int SPI_CLK_SPD;                // 1MHz clock speed
unsigned char WRITE;            // command to write to register
unsigned char READ;             // command to read from register
unsigned char CLEAR;            // command to clear register
unsigned char MDR0;             // MDR0 register
unsigned char MDR1;             // MDR1 register
unsigned char CNTR;             // Counter register
unsigned char BYTES_4;          // 4 byte counter mode (was 0x00)
unsigned char ENCNT;            // to enable counter
double N;                       // 540 ticks per revolution of motor
double PI;                      // Pi 
double RADIUS;                  // Radius of wheel in cm
int MAX_CYCLES;                 // 25 counts before calc speed
int VEER;
int AGGRESSIVE;

// Lidar angle distances
volatile double a_30;           // 30 degrees to the right
volatile double a_60;           // 60 degrees to the right
volatile double a_90;           // 90 degrees right
volatile double a_270;          // 90 degrees left
volatile double a_300;          // 60 degrees to the left
volatile double a_330;          // 30 degrees to the left
volatile double a_359;          // ~0 degrees straight ahead

// All of our global variables
volatile char* fifo_path;       // path to fifo file for pipe
volatile double cur_speed_a;    // speed of motor A wheel
volatile double cur_speed_b;    // speed of motor B wheel
volatile int currentPowerA;     // range 0-100 (percent) Motor.c
volatile int currentPowerB;     
volatile int currentDirectionA; // 1 for forwards, 0 for back Motor.c
volatile int currentDirectionB;
volatile int halted;            // 1 for vehicle halted for obstacle
volatile int onLine_A;          // 0 for off the line, 1 for on line
volatile int onLine_B;          // 0 for off the line, 1 for on line
volatile int onLine_C;          // 0 for off the line, 1 for on line
volatile int onLine_D;          // 0 for off the line, 1 for on line
volatile int onLine_E;          // 0 for off the line, 1 for on line
volatile int car_activated;     // 0 for car off, 1 for car on
volatile int distance;          // distance from front to obstacle
volatile int side_distance;     // distance from side to obstacle
volatile int atSafeDistance;

/**************************************************************
Initialize all global variables and constants
**************************************************************/
void globals_init() {
    // Sensor GPIO mappings
    LINESENSOR_A = 21;     //Extreme Left 
    LINESENSOR_B = 26;     //Mid Left     
    LINESENSOR_C = 16;     //Mid Center
    LINESENSOR_D = 20;     //Mid Right
    LINESENSOR_E = 19;     //Extreme Right 

    // Sensor initialization values
    onLine_A = 0;          // 0 for off the line, 1 for on line
    onLine_B = 0;          // 0 for off the line, 1 for on line
    onLine_C = 0;          // 0 for off the line, 1 for on line
    onLine_D = 0;          // 0 for off the line, 1 for on line
    onLine_E = 0;          // 0 for off the line, 1 for on line

    US_MULTIPLIER = 17150; // for calculating distance equation
    BUTTON = 17;

    // PCA9685 GPIO Config
    SUBADR1 = 0x02;
    SUBADR2 = 0x03;
    SUBADR3 = 0x04;
    MODE1 = 0x00;
    PRESCALE = 0xFE;
    LED0_ON_L = 0x06;
    LED0_ON_H = 0x07;
    LED0_OFF_L = 0x08;
    LED0_OFF_H = 0x09;
    ALLLED_ON_L = 0xFA;
    ALLLED_ON_H = 0xFB;
    ALLLED_OFF_L = 0xFC;
    ALLLED_OFF_H = 0xFD;
    PCA_CHANNEL_0 = 0;
    PCA_CHANNEL_1 = 1;
    PCA_CHANNEL_2 = 2;
    PCA_CHANNEL_3 = 3;
    PCA_CHANNEL_4 = 4;
    PCA_CHANNEL_5 = 5;
    PCA_CHANNEL_6 = 6;
    PCA_CHANNEL_7 = 7;
    PCA_CHANNEL_8 = 8;
    PCA_CHANNEL_9 = 9;
    PCA_CHANNEL_10 = 10;
    PCA_CHANNEL_11 = 11;
    PCA_CHANNEL_12 = 12;
    PCA_CHANNEL_13 = 13;
    PCA_CHANNEL_14 = 14;
    PCA_CHANNEL_15 = 15;

    // Motor
    MOTOR_A = 0;
    MOTOR_B = 1;
    ON = 1;                  // general on/off triggers
    OFF = 0;
    HIGH_PULSE = 1;
    LOW_PULSE = 0;
    FORWARD = 1;
    BACKWARD = 0;
    MIN_FREQ = 0;
    MAX_FREQ = 100;          // for motor speed
    DEFAULT_POWER_A = 44;    // default driving speed for A
    DEFAULT_POWER_B = 40;    // default driving speed for B

    // Speedometer
    CE0 = 0;                 // Chip Enable 0 channel 0
    CE1 = 1;                 // Chip Enable 1 channel 1
    SPI_CLK_SPD = 1000000;   // 1MHz clock speed
    WRITE = 0x80;            // command to write to register
    READ = 0x40;             // command to read from register
    CLEAR = 0x00;            // command to clear register
    MDR0 = 0x80;             // MDR0 register
    MDR1 = 0x10;             // MDR1 register
    CNTR = 0x20;             // Counter register
    BYTES_4 = 0x03;          // 4 byte counter mode (was 0x00)
    ENCNT = 0x10;            // to enable counter
    N = 540.0;               // 540 ticks per revolution of motor
    PI = 3.14159;            // Pi 
    RADIUS = 3.25;           // Radius of wheel in cm
    MAX_CYCLES = 25;         // 25 counts before calc speed
    VEER = 10;               // add/sub 10 power for turn
    AGGRESSIVE = 20;         // add/sub 20 power for turn

    // Lidar angle distances
    a_30 = 9999999.0;       // 30 degrees to the right
    a_60 = 9999999.0;       // 60 degrees to the right
    a_90 = 9999999.0;       // 90 degrees right
    a_270 = 9999999.0;      // 90 degrees left
    a_300 = 9999999.0;      // 60 degrees to the left
    a_330 = 9999999.0;      // 30 degrees to the left
    a_359 = 9999999.0;      // ~0 degrees straight ahead

    // All of our global variables
    fifo_path = NULL;       // path to fifo for pipe
    cur_speed_a = 0.0;      // speed of motor A wheel
    cur_speed_b = 0.0;      // speed of motor B wheel  
    currentPowerA = 0;      // range 0-100 (percent) Motor.c
    currentPowerB = 0;
    currentDirectionA = 0;  // 1 for forwards, 0 for back Motor.c
    currentDirectionB = 0;
    halted = 1;             // vehicle starts off halted initially
    car_activated = 0;      // vehicle is initially off
    distance = 9999999;     // distance set to a huge value initially
    side_distance = 9999999;// distance on side set to huge value
    atSafeDistance = 1;     // assume at safe distance initially
}