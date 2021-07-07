/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Team Shartcar
* Student ID: 
* Github ID: 
* Project: Drive On
*
* File: Speedometer.c
*
* Description: Implementation file for all routine prototypes 
*           in Speedometer.h. Handles the SPI communications 
*           between the Raspberry Pi and the 
*           quadrature counter. Calculates the speed in 
*           cm/s    
*
**************************************************************/
#include "Include.h"

/**************************************************************
Initialize all speedometer variables, such as chip enable pins
**************************************************************/
void speedometer_init() {
    //SPI config
    int fd = wiringPiSPISetup(CE0, SPI_CLK_SPD);
    int fd2 = wiringPiSPISetup(CE1, SPI_CLK_SPD);
    if ((fd == -1) || (fd2 == -1)) {
        printf("SPI setup failed!.\n");
    }
    printf("SPI successfully setup.\n");
}

/**************************************************************
Read the counter register from one quadrature counter. 
**************************************************************/
void readCounter(int chip_enable, int motor) {
    //clear MDR1
    unsigned char arg1 = (CLEAR | MDR1);
    unsigned char arg2 = ( 0x00 );
    unsigned char data[] = { arg1, arg2 }; // pass in 0x10 = 0001 0000
    wiringPiSPIDataRW(chip_enable, data, 1);
    delay(100);

    //set MDR0
    unsigned char arg3 = (WRITE | MDR0);
    unsigned char arg4 = (BYTES_4);
    unsigned char data2[] = { arg3, arg4 }; // send 0x88,0x03
    wiringPiSPIDataRW(chip_enable, data2, 2);
    delay(100);

    //clear counter
    unsigned arg6 = (CLEAR | CNTR);
    unsigned char cbuf[] = { arg6 };
    wiringPiSPIDataRW(chip_enable, cbuf, 1);
    delay(100);

    int delta = 0;                      // delta count between reads
    int count = 0;                      // current count
    int lastCount = 0;                  // last pulse count
    int d_lay = 250;                    // milliseconds to delay
    float speed;                        // speed in cm/s
    double t = (double) (d_lay) / 1000.0;// time elapsed in seconds

    //read from Counter
    while(car_activated == ON) { // if we turn off the car, stop this
        int bufSize = 5;
        unsigned char arg5 = 0x60;

        // setup the command to send
        unsigned char buf[] = { 0x60, 0x00, 0x00, 0x00, 0x00 };
        unsigned char sbuf[] = { 0x70, 0x00, 0x00, 0x00, 0x00 };

        // do the read
        int ret = wiringPiSPIDataRW(chip_enable, buf, 5);
        delay(d_lay);
        
        // combine the bytes returned into one integer
        int a = (int)buf[1] << 24; 
        int b = (int)buf[2] << 16;
        int c = (int)buf[3] << 8; 
        int d = (int)buf[4];
        count = a | b | c | d; 
        delta = count - lastCount;
        lastCount = count;

        // flip the signs if motor B is spinning
        if(chip_enable == CE0) {
            count = -count;
            delta = -delta;
        }
    
        // calculate the speed = 2*pi*r/Nt
        double omega = (2.0 * PI * delta) / (N * t); 
        speed = omega * RADIUS; //cm per sec
        //printf("omega: %.2f\t", omega);
        //printf("speed: %.2f cm/s\n", (speed));
        if(motor == MOTOR_A) cur_speed_a = speed;
        else cur_speed_b = speed;
            
    }
}