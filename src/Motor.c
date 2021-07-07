/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Jennifer Finaldi
* Student ID: 920290420
* Github ID: jfinaldi
* Project: Drive On
*
* File: Motor.c
*
* Description: Implementation file for all motor routines in
*           Motor.h. Handles basic functionality such as
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
#include "Include.h"

void motor_init() {

    //I2C config
    PCA9685_Init(0x40); // initialize the PCA9685 with address 40
    PCA9685_SetPWMFreq(MAX_FREQ); // Set the pulse frequency to max

    printf("Motor successfully initialized.\n");

}

/*******************************************************
setupForward is a routine that takes a speed integer
representing a percentage of top speed, and a motor
identifier as parameters, and sets the PWM to allow the
motor to spin in the forward direction at the speed
specified.
********************************************************/
void setupForward(int speed, int motor) {
    // if motor is a, all PCA channels are 0, 1, 2
    if(motor == MOTOR_A) {
        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, speed); // pulse at speed rate
        PCA9685_SetLevel(PCA_CHANNEL_1, OFF); // Set channel 1 to off
        PCA9685_SetLevel(PCA_CHANNEL_2, ON); // Set channel 2 to on
        currentPowerA = speed;
        currentDirectionA = FORWARD;
    }

    // if motor is motor b, all PCA channels change to 5, 3, 4
    else if(motor == MOTOR_B) {
        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, speed); // pulse at speed rate
        PCA9685_SetLevel(PCA_CHANNEL_3, OFF); // Set channel 1 to off
        PCA9685_SetLevel(PCA_CHANNEL_4, ON); // Set channel 2 to on
        currentPowerB = speed;
        currentDirectionB = FORWARD;
    }
}

/*******************************************************
setupBackward is a routine that takes a speed integer
representing a percentage of top speed, and a motor
identifier as parameters, and sets the PWM to allow the
motor to spin in the backward direction at the speed
specified.
********************************************************/
void setupBackward(int speed, int motor) {
    // if motor is a, all PCA channels are 0, 1, 2
    if(motor == MOTOR_A) {
        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, speed); // set the PWM cycle 
        PCA9685_SetLevel(PCA_CHANNEL_1, ON); // Set channel 1 to on
        PCA9685_SetLevel(PCA_CHANNEL_2, OFF); // Set channel 2 to off
        currentPowerA = speed;
        currentDirectionA = BACKWARD;
    }
    // if motor is motor b, all PCA channels change to 5, 3, 4
    if(motor == MOTOR_B) {
        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, speed); // set the PWM cycle 
        PCA9685_SetLevel(PCA_CHANNEL_3, ON); // Set channel 1 to on
        PCA9685_SetLevel(PCA_CHANNEL_4, OFF); // Set channel 2 to off
        currentPowerB = speed;
        currentDirectionB = BACKWARD;
    }
}

/*******************************************************
accelerate takes a target speed as an integer representing a
percentage of top speed, and gradually increases it until
that speed is achieved.

If direction parameter does not match the current direction
while the motor is currently running, error handling will
avoid rapid direction changes that will burn the motor
hardware.

Return value: 0 for error, 1 for success
********************************************************/
int accelerate(int motor, int targetSpeed, int direction) {

    int power;
    int cur_direction;

    if(motor == MOTOR_A) {
        power = currentPowerA;
        cur_direction = currentDirectionA;
    }
    if(motor == MOTOR_B) {
        power = currentPowerB;
        cur_direction = currentDirectionB;
    }

    // validate our input values
    if(targetSpeed > MAX_FREQ) {
        printf("Error Motor.c, accelerate(): targetSpeed parameter exceeds max\n");
        return 0;
    }
    if(power >= MAX_FREQ) {
        printf("Error Motor.c, accelerate(): currentSpeed exceeds or is at max, cannot speed up\n");
        return 0;
    }
    if((direction < 0) || (direction > 1)) {
        printf("Error Motor.c, accelerate(): invalid direction parameter. Must be 0 or 1\n");
        return 0;
    }
    if((motor != MOTOR_A) && (motor != MOTOR_B)) {
        printf("Error Motor.c, accelerate(): invalid motor parameter. Must be 0 or 1\n");
        return 0;        
    }

    // make sure not to flip directions and fry the motor
    if((power > MIN_FREQ) && (cur_direction != direction)) {
        printf("Error Motor.c, accelerate(): Cannot change wheel direction while in motion!\n");
        return 0;
    }

    // start a loop to increase speed to targetSpeed
    for(int i = power; i <= targetSpeed; i++) {
        power = i; // document the current power

        if(direction == BACKWARD) setupBackward(i, motor);
        else setupForward(i, motor);
        delay(50);
    }

    return 1;
}

/*******************************************************
decelerate takes a target speed as an integer representing a
percentage of top speed, and gradually dereases it until
that speed is achieved.

If direction parameter does not match the current direction
while the motor is currently running, error handling will
avoid rapid direction changes that will burn the motor
hardware.

Return value: 0 for error, 1 for success
********************************************************/
int decelerate(int motor, int targetSpeed, int direction) {
    int power;
    int cur_direction;

    if(motor == MOTOR_A) {
        power = currentPowerA;
        cur_direction = currentDirectionA;
    }
    if(motor == MOTOR_B) {
        power = currentPowerB;
        cur_direction = currentDirectionB;
    }

    // validate our input values
    if(targetSpeed < MIN_FREQ) {
        printf("Error Motor.c, decelerate(): targetSpeed parameter less than 0\n");
        return 0;
    }
    if(power < MIN_FREQ) {
        printf("Error Motor.c, decelerate(): currentSpeed less than 0, cannot slow down\n");
        return 0;
    }
    if((direction < 0) || (direction > 1)) {
        printf("Error Motor.c, decelerate(): invalid direction parameter. Must be 0 or 1\n");
        return 0;
    }
    if((motor != MOTOR_A) && (motor != MOTOR_B)) {
        printf("Error Motor.c, decelerate(): invalid motor parameter. Must be 0 or 1\n");
        return 0;        
    }

    // make sure not to flip directions and fry the motor
    if((power > MIN_FREQ) && (cur_direction != direction)) {
        printf("Error Motor.c, decelerate(): Cannot change wheel direction while in motion!\n");
        return 0;
    }

    // start a loop to decrease speed to targetSpeed
    for(int i = power; i > targetSpeed; --i) {
        power = i; // document the current power

        if(direction == BACKWARD) setupBackward(i, motor);
        else setupForward(i, motor);
        //printf(" Speed: %d%\r", i);
        //fflush(stdout);
        delay(50);
    }

    return 1;
}

void stopMotor(int motor) {
    if((motor != MOTOR_A) && (motor != MOTOR_B)) {
        printf("Error Motor.c, stopMotor(): invalid motor parameter. Must be 0 or 1\n");
    }
            
    // turn off pulse cycle
    if(motor == MOTOR_A) {
        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, OFF);
        currentPowerA = MIN_FREQ;
    }
    if(motor == MOTOR_B) {
        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, OFF);
        currentPowerB = MIN_FREQ;
    }

}