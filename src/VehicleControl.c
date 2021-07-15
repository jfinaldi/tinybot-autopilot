/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Team Shartcar
* Student ID: 
* Github ID: 
* Project: Drive On
*
* File: VehicleControl.c
*
* Description: Control interface for the vehicle that handles
*           the processing of incoming data and controls the
*           car's maneuvering.
*
**************************************************************/
#include "Include.h"

/**************************************************************
Initialize all components and structures by calling their
init functions in each module
**************************************************************/
#define ECHO 22             // Echo sensor for GPIO 22
#define TRIGGER 27          // Echo sensor for GPIO 27
#define ECHO_SIDE 24        // Echo sensor for GPIO 24
#define TRIGGER_SIDE 18     // Echo sensor for GPIO 18
void init(char* path) {
    
    // initialize wiringPi and button gpio
    if (wiringPiSetupGpio() < 0)
        printf("error with wiringPiSetupGpio()!\n");
    pinMode(BUTTON, INPUT); 

    // Initialize echo sensors
    pinMode(ECHO, INPUT);
    pinMode(ECHO_SIDE, INPUT);
    pinMode(TRIGGER, OUTPUT);
    pinMode(TRIGGER_SIDE, OUTPUT);

    globals_init();     // initialize all global variables
    sensors_init();     // initialize all sensors
    motor_init();       // initialize motors
    speedometer_init(); // initialize SPI for speedometer

    // get the argument and pass to lidar
    if(path != NULL)
        init_lidar(path);
    else    
        printf("Warning: fifo path for lidar pipe not provided.\n");
        
}

/**************************************************************
Initiate a change in direction of vehicle travel. Operates by
changing the speed of one front wheel to be faster 

return: 1 for success, 0 for failure
***************************************************************/
int veer(int goLeft, int speedA, int speedB) {
    resume();

    // Veer right
    if(goLeft == OFF) {
        setupForward(speedA, MOTOR_A); // slow down right wheel
        setupForward(speedB, MOTOR_B); // speed up left wheel
    }
    // Veer left
    else if(goLeft == ON) {
        setupForward(speedA, MOTOR_A); // slow down right wheel
        setupForward(speedB, MOTOR_B); // speed up left wheel
    }
    delay(250);
    resume(); // continue straight
    return 1;
}

/**************************************************************
Evaluate the IR input and determine if an obstacle is 
within x-distance of the front of the vehicle. 

return: 1 for obstacle in the vehicle's path, 0 for all-clear
***************************************************************/
// int obstacleIsAhead() {
//     if((obstacleIR_B == ON) || (obstacleIR_C == ON))
//         return 1;
//     return 0;
// }

/**************************************************************
If the car gets stuck on a line via edge case, this is the
redundancy method to seek out the line via rotation.

Edge case: Line is positioned between the left or right-most
middle line sensor and one of the side lateral line sensors
***************************************************************/
int getUnstuck() {
    printf("getUnstuck\n");
    /**********************
    RIGO ALGORITHM
    **********************/
    // twist right a tiny bit
    setupForward(DEFAULT_POWER_B ,MOTOR_B);
    setupBackward(DEFAULT_POWER_A, MOTOR_A);
    delay(250);
    halt();

    // Check status of SENSOR_D
    if((onLine_D == ON) || (onLine_B == ON)) 
        return 1;

    if(onLine_D != ON) {
        // twist left a tiny bit
        setupBackward(DEFAULT_POWER_B ,MOTOR_B);
        setupForward(DEFAULT_POWER_A, MOTOR_A);
        delay(1000);
        halt();  
        if(onLine_B == ON) 
            return 1; 
    }
    else {
        while(car_activated == ON) {
            setupBackward(DEFAULT_POWER_B, MOTOR_B);
            setupForward(DEFAULT_POWER_A, MOTOR_A);
            delay(250);
            halt();

            if(onLine_D == ON) 
                return 1;
        }
    }
        
    return 0;
}

/**************************************************************
Evaluate the group of three line sensors on the front middle
bumper and if we don't have "ON ON ON" then steering is 
required. This method will trigger turn() function to steer
the car depending on the configuration of the sensors
input.

If all three display "0 0 0" meaning that all three have left
the line, this is indicative of either the end of the course,
or a 90 degree left or right turn required, and this
method will look for the line continuing on either side by
looking at the left and right lateral line sensor data. If 
either is found, the method will trigger sharpTurn() for that
direction.

return: 1 for success, 0 for error
***************************************************************/
#define ECHO_MIN_DIST 20
int correctHeading() {

    // Completely off the line: 0 0 0
    if((onLine_B == OFF) && (onLine_C == OFF) && (onLine_D == OFF)) { 
        printf("\n\nOff the line\n\n");
        halt(); 

        // make a hard right turn
        if(onLine_E == ON)
            sharpTurn(OFF, DEFAULT_POWER_A - VEER, DEFAULT_POWER_B + VEER + 4);
        
        // make a hard left turn
        else if(onLine_A == ON) 
            sharpTurn(ON, (DEFAULT_POWER_A + VEER + 8), (DEFAULT_POWER_B - VEER));

        // Check the Echo distance 
        if(distance > ECHO_MIN_DIST){
            // If the above are also OFF, then call the stuck function
            getUnstuck();
        } 
    }

    // Almost completely left of the line: 0 0 1
    // Initiate a medium veer right maneuver
    while((onLine_B == OFF) && (onLine_C == OFF) && (onLine_D == ON)) 
        veer(OFF, (DEFAULT_POWER_A - AGGRESSIVE - 2), (DEFAULT_POWER_B + AGGRESSIVE + 6));

    // Slightly left of the line: 0 1 1
    // Initiate a slight veer right maneuver
    while((onLine_B == OFF) && (onLine_C == ON) && (onLine_D == ON))
        veer(ON, (DEFAULT_POWER_A - VEER), (DEFAULT_POWER_B + VEER));

    // Slightly right of the line: 1 1 0
    // Initiate a slight veer left maneuver
    while((onLine_B == ON) && (onLine_C == ON) && (onLine_D == OFF))
        veer(ON, (DEFAULT_POWER_A + VEER), (DEFAULT_POWER_B - VEER));

    // Almost completely right of the line: 1 0 0
    // Initiate a medium veer left maneuver
    while((onLine_B == ON) && (onLine_C == OFF) && (onLine_D == OFF))
        veer(ON, (DEFAULT_POWER_A + AGGRESSIVE + 7), (DEFAULT_POWER_B - AGGRESSIVE - 4));

    // Error Case: 1 0 1
    // This indicates two thin lines. Bad  
    if((onLine_B == ON) && (onLine_C == OFF) && (onLine_D == ON)) {
        printf("Error, front line sensors reading 101\n");
        //resume();
    }

    // Error Case: 0 1 0
    // This indicates a thin line. Bad. But continue on LOL
    else if((onLine_B == OFF) && (onLine_C == ON) && (onLine_D == OFF)) {
        printf("Error, front line sensors reading 010\n");
        //resume(); 
    }

    // Perfectly centered on the line: 1 1 1
    // Trigger car to resume straight path travel
    else if((onLine_B == ON) && (onLine_C == ON) && (onLine_D == ON))
        resume();

    return 1;
}

/**************************************************************
The car has detected an obstacle and needs to initiate a
sequence to go around the object. It uses echo sensors on the
front and left side of the car. The left sensor monitors the
initial distance and the difference between the current and
last reading. If the change is huge, this means the middle of
the car has passed the obstacle and it can begin to turn for 
the next leg of the pattern:

Leg 1: turn right and move parallel to the front of the object
Leg 2: turn left and move parallel to the side of the object
Leg 3: turn left and find the line
Leg 4: turn right to get back on the line

Note: strikes is a variable intended to prevent problems with
      random wild readings on the echo sensor. It will 
      increment to 3 for each sequential wild reading and if
      so, we will know that it's a genuine large delta that
      should trigger the next step of the sequence.
***************************************************************/
int goAround() {
    int prevSideDist = 999999;
    int curSideDist = 999999;
    int thresh = 20;            // stop 20cm in front of object
    int strikes = 0;  
    int maxStrikes = 3;          

    // Since we are halted, resume vehicle movement
    resume();

    // Make the first right turn
    setupForward(DEFAULT_POWER_B + VEER + 4, MOTOR_B); //speed up left wheel
    setupBackward(DEFAULT_POWER_A - VEER, MOTOR_A); //reverse right wheel
    delay(1250); //give time to turn

    // Continue Straight for First Leg
    resume(); 
    delay(750);
    prevSideDist = side_distance;
    while(ON) {
        curSideDist = side_distance; // look at the current side distance
        int delta = abs(curSideDist - prevSideDist);
        printf("curSideDist: %d  |  ", curSideDist);
        printf("prevSideDist: %d  |  ", prevSideDist);
        printf("delta: %d\n", delta);
        if(delta > thresh) {
            strikes++;
            if(strikes >= maxStrikes) {
                strikes = 0;
                delay(1250); // clear the rest of the vehicle before turning
                break; 
            }
        }
    }

    // Make the first left turn
    setupBackward(0, MOTOR_B); //back the left wheel
    setupForward((DEFAULT_POWER_A + VEER + 14), MOTOR_A); //speed up the right wheel
    delay(1400); 

    // Continue Straight For Second Leg
    resume(); 
    delay(1100); 
    prevSideDist = side_distance;
    while(ON) {
        curSideDist = side_distance; // look at the current side distance
        int delta = abs(curSideDist - prevSideDist);
        printf("curSideDist: %d  |  ", curSideDist);
        printf("prevSideDist: %d  |  ", prevSideDist);
        printf("delta: %d\n", delta);
        if(delta > thresh) {
            strikes++;
            if(strikes >= maxStrikes) {
                strikes = 0;
                delay(1850); // clear the rest of the vehicle before turning
                printf("Second Leg Complete!\n");
                break; 
            }
        }
    }
            
    // Make the second left turn
    setupBackward(0, MOTOR_B); //back the left wheel
    setupForward((DEFAULT_POWER_A + VEER + 14), MOTOR_A); //speed up the right wheel
    delay(1000); 
    prevSideDist = side_distance;

    // Continue Straight until we find the line
    resume();
    while((onLine_B == OFF) && (onLine_C == OFF) && (onLine_D == OFF)) {}

    // turn right 90 degrees back onto the line
    sharpTurn(OFF, DEFAULT_POWER_A - VEER - 8, DEFAULT_POWER_B + VEER + 4);

    return 1;
}

/**************************************************************
Make a 90 degree turn left or right, depending on the 
arguments passed.

Parameters:
    goLeft: boolean. If true, vehicle turns left. If false,
        vehicle will turn right.

return: 1 for success, 0 for error
***************************************************************/
int sharpTurn(int goLeft, int speedA, int speedB) {
    // Sharp Right Turn 90 degrees
    if(goLeft == OFF) {
        setupForward(speedB, MOTOR_B); //left wheel forwards
        setupBackward(speedA, MOTOR_A); //back the right wheel
        delay(800); // give it a moment to correct
    }

    // Sharp Left Turn 90 degrees
    else if(goLeft == ON) {
        setupBackward(speedB, MOTOR_B); //back the left wheel
        setupForward(speedA, MOTOR_A); //speed up the right wheel
        delay(950); // give it a moment to correct
    }

    resume();   //continue straight

    return 1;
}

/**************************************************************
Halt stops the car periodically for different reasons, such
as to evaluate a situation before continuing, or prior to
termination of program.
***************************************************************/
void halt() {
    stopMotor(MOTOR_A);
    stopMotor(MOTOR_B);
    halted = ON;
}

/**************************************************************
Resume triggers the car to continue traveling forward at
its default speed.
***************************************************************/
void resume() {
    setupForward(DEFAULT_POWER_A, MOTOR_A);
    setupForward(DEFAULT_POWER_B, MOTOR_B);
    halted = OFF;
}

/**************************************************************
THREAD FUNCTIONS
***************************************************************/
//button thread function
void* buttonThread(void* args){
    //runs forever until return or breakout
    while (ON){
        //If the button gets pressed
        if(digitalRead(BUTTON) == ON) {
            //If the car is activated
            if(car_activated == ON) {
                car_activated = OFF;
                printf("Lets exit our threads now...\n");
                break;
            }
            //If the car is not activated
            else 
                car_activated = ON;
        }
        delay(500);
    }
    halted = ON;
    stopMotor(MOTOR_A);
    stopMotor(MOTOR_B);
}

/**************************************************************
Line thread continuously gets readings for one individual
line sensor and updates its respective global variable
***************************************************************/
void* lineThread(void* args) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    
    // unpack arguments
    Args* arg = (Args*)args;
    int sensor = arg->lineSensor;

    // call sensor readings method
    getLineSensorData(sensor);
}

/**************************************************************
Speed thread takes readings from the LS7366R quadrature
encoder chip connected to one motor and then calculates the
speed that wheel is traveling, and updates its global variable. 
All speed readings are in the units cm/s
***************************************************************/
void* speedThread(void* args) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    Args* arg = (Args*)args;
    int motor = arg->motor;
    int chip_enable;

    if(motor == MOTOR_B) 
        chip_enable = CE0;
    else 
        chip_enable = CE1;

    readCounter(chip_enable, motor);
}

/**************************************************************
Lidar thread is intended to take distance readings from the
Lidar unit.
***************************************************************/
void* lidarThread(void* args) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    // unpack arguments
    // call readLidar mmm,m,mm,m
}

/**************************************************************
Drive thread is the main driving thread that handles the
maneuvering of the vehicle based on data read from sensors.
Operates in an infinite loop unless the car is deactivated.
Drive thread is the only thread that does not get canceled but
rather quits on its own.
***************************************************************/
void* driveThread(void* args) {
    int distanceThresh = 20; // the min distance we'll get to an obstacle ahead

    resume(); // initially start the car going straight at default speeds

    while(car_activated == ON) {
        // output all the sensors
        printf("Line sensors A->E: %d  %d%d%d  %d\n", onLine_A, onLine_B, onLine_C, onLine_D, onLine_E);
        //printf("Distance: %d cm  |  Side Distance: %d cm\n ", distance, side_distance);

        // if our front echo sensor flags an obstacle, stop the motors
        if(distance <= distanceThresh) {
            printf("obstacle in way! stopping now...\n");
            stopMotor(MOTOR_A);
            stopMotor(MOTOR_B);
            delay(3000); // Wait 3 sec for obstacle to pass, if its mobile
            
            // Object is stationary so go around
            if(distance <= distanceThresh) 
                goAround();    
        }
        else if(halted == OFF)
            resume();
        correctHeading();
    }//while
}//driveThread

/**************************************************************
Distance thread is responsible for keeping track of the distance
readings between one echo sensor and the nearest obstacle in
front of it. It updates its respective global variable
***************************************************************/
void* distanceThread(void* args) {
    printf("starting distanceThread..\n");
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    clock_t start, end; // for timing the echo response
    double cpu_time_used, local_distance; // time elapsed and distance recorded
    int isFrontSensor;
    int trigger;
    int echo;

    // get arguments
    Args* arg = (Args*)args;
    isFrontSensor = arg->isFrontSensor;

    // if this is for front sensor use front sensor GPIO
    if(isFrontSensor) {
        trigger = TRIGGER;
        echo = ECHO;
    }
    // otherwise use side sensor GPIO
    else {
        trigger = TRIGGER_SIDE;
        echo = ECHO_SIDE;
    }

    // trigger an initial low pulse to settle the sensor
    digitalWrite(trigger, LOW);
    delay(2000); // wait 2 seconds
    
    while(ON) {
        // Send a high/low pulse sequence
        digitalWrite(trigger, HIGH);
        delay(0.01); 
        digitalWrite(trigger, LOW); 
    
        // start timer 
        while(digitalRead(echo) == LOW) 
            start = clock();

        // stop timer
        while(digitalRead(echo) == HIGH)
            end = clock();

        // calculate duration
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; // duration = end time - start time
    
        // calculate distance
        local_distance = cpu_time_used * US_MULTIPLIER; // distance = duration * 17150

        // write distance to global shared distance variable
        if(isFrontSensor)
            distance = (int)local_distance; // truncate distance for simplicity
        else
            side_distance = (int)local_distance;
        delay(100);
    }   
}

