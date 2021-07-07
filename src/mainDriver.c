/**************************************************************
* Class: CSC-615-01 Spring 2021
* Name: 
* Student ID: 
* Github ID: 
* Project: Drive On
*
* File: mainDriver.c
*
* Description: Main driver for vehicle operation. Interfaces with
*           VehicleControl.c 
*
**************************************************************/
#include "Include.h"

// other prototypes
void intro();

int main(int argc, char** argv) {
    //initialize
    init();
    intro();

    // thread variables
    pthread_t line_A, line_B, line_C, line_D, line_E; // line sensor threads
    //pthread_t obs_A, obs_B, obs_C, obs_D;             // ir sensor threads
    pthread_t distance, side_distance;                // distance measuring
    pthread_t button;                                 //button
    pthread_t drive;                                  
    pthread_t speedA, speedB;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // declare arg structs for sensors A->E
    Args args_A;
    Args args_B;
    Args args_C;
    Args args_D;
    Args args_E;
    Args distanceArgs;
    Args side_distanceArgs;

    // initialize arguments to each struct
    args_A.motor = MOTOR_A;
    args_B.motor = MOTOR_B;
    args_A.lineSensor = LINESENSOR_A;
    args_B.lineSensor = LINESENSOR_B;
    args_C.lineSensor = LINESENSOR_C;
    args_D.lineSensor = LINESENSOR_D;
    args_E.lineSensor = LINESENSOR_E;
    distanceArgs.isFrontSensor = 1;
    side_distanceArgs.isFrontSensor = 0;

    //button thread
    int createButton = pthread_create(&button, &attr, &buttonThread, NULL);
    if(createButton != 0) printf("Error creating button thread!\n");
    
    //checks if car is activated - once it is, threads are created
    while(ON){
        if (car_activated == ON)
            break;
    }
    printf("Car has been activated. Byebye!...\n");

    // create all line sensor threads
    int createA = pthread_create(&line_A, &attr, &lineThread, (void*)&args_A);
    int createB = pthread_create(&line_B, &attr, &lineThread, (void*)&args_B);
    int createC = pthread_create(&line_C, &attr, &lineThread, (void*)&args_C);
    int createD = pthread_create(&line_D, &attr, &lineThread, (void*)&args_D);
    int createE = pthread_create(&line_E, &attr, &lineThread, (void*)&args_E);

    // create the main drive thread
    int createJ = pthread_create(&drive, &attr, &driveThread, NULL);

    // create the distance measuring threads for echo sensor
    int createK = pthread_create(&distance, &attr, &distanceThread, (void*)&distanceArgs);
    int createL = pthread_create(&side_distance, &attr, &distanceThread, (void*)&side_distanceArgs);

    // create the speedometer threads
    int createM = pthread_create(&speedA, &attr, &speedThread, (void*)&args_A);
    int createN = pthread_create(&speedB, &attr, &speedThread, (void*)&args_B);

    // handle errors from pthread_create
    if(createA != 0) printf("Error creating line_A thread!\n");
    if(createB != 0) printf("Error creating line_B thread!\n");
    if(createC != 0) printf("Error creating line_C thread!\n");
    if(createD != 0) printf("Error creating line_D thread!\n");
    if(createE != 0) printf("Error creating line_E thread!\n");
    if(createJ != 0) printf("Error creating drive thread!\n");
    if(createK != 0) printf("Error creating distance thread!\n");
    if(createL != 0) printf("Error creating side_distance thread!\n");
    if(createM != 0) printf("Error creating speedA thread!\n");
    if(createN != 0) printf("Error creating speedB thread!\n");

    // join button thread first
    int joinButton = pthread_join(button, NULL);    
    if(joinButton != 0) printf("Error joining button thread!\n");
    
    // cancel all threads
    int cancelA = pthread_cancel(line_A);
    int cancelB = pthread_cancel(line_B);
    int cancelC = pthread_cancel(line_C);
    int cancelD = pthread_cancel(line_D);
    int cancelE = pthread_cancel(line_E);
    int cancelK = pthread_cancel(distance);
    int cancelL = pthread_cancel(side_distance);
    int cancelM = pthread_cancel(speedA);
    int cancelN = pthread_cancel(speedB);

    // join all threads
    int joinA = pthread_join(line_A, NULL);
    int joinB = pthread_join(line_B, NULL);
    int joinC = pthread_join(line_C, NULL);
    int joinD = pthread_join(line_D, NULL);
    int joinE = pthread_join(line_E, NULL);
    int joinK = pthread_join(distance, NULL);
    int joinL = pthread_join(side_distance, NULL);
    int joinM = pthread_join(speedA, NULL);
    int joinN = pthread_join(speedB, NULL);
    int joinJ = pthread_join(drive, NULL);

    // handle errors from join
    if(joinA != 0) printf("Error joining line_A thread!\n");
    if(joinB != 0) printf("Error joining line_B thread!\n");
    if(joinC != 0) printf("Error joining line_C thread!\n");
    if(joinD != 0) printf("Error joining line_D thread!\n");
    if(joinE != 0) printf("Error joining line_E thread!\n");
    if(joinJ != 0) printf("Error joining drive thread!\n");
    if(joinK != 0) printf("Error joining distance thread!\n");
    if(joinL != 0) printf("Error joining side_distance thread!\n");
    if(joinM != 0) printf("Error joining speedA thread!\n");
    if(joinN != 0) printf("Error joining speedB thread!\n");

    // errors from cancelling threads
    if(cancelA != 0) printf("Error cancelling line_A thread!\n");
    if(cancelB != 0) printf("Error cancelling line_B thread!\n");
    if(cancelC != 0) printf("Error cancelling line_C thread!\n");
    if(cancelD != 0) printf("Error cancelling line_D thread!\n");
    if(cancelE != 0) printf("Error cancelling line_E thread!\n");
    if(cancelK != 0) printf("Error cancelling distance thread!\n");
    if(cancelL != 0) printf("Error cancelling side_distance thread!\n");
    if(cancelM != 0) printf("Error cancelling speedA thread!\n");
    if(cancelN != 0) printf("Error cancelling speedB thread!\n");
    
    // just in case the motors are still running for some reason
    stopMotor(MOTOR_A);
    stopMotor(MOTOR_B);

    printf("All threads successfully terminated. \n");
    
    return 0;

}

void intro() {
    printf("Hello world from Raspberry Pi!\n");
    printf("Please press the green button on the car to start.\n");
    printf("If you'd like to stop the car at any time, press the green button firmly a second time.\n");
    printf("---------------------------------------------------------------------------------------\n\n");
}

