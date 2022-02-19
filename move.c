/*
 * move.c
 *
 *  Created on: Nov 16, 2021
 *      Author: tmassner
 */

#include "move.h"

//Moves the CyBot forward; Checks for bump detection as it goes
//Returns something representing what it hit
int move() {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    double sum = 0.0;           //Distance that the CyBot has currently traveled

    oi_setWheels(140, 100);     //You'll have to calibrate the Cybot here to get it to go straight
    while(sum < 350) {
        oi_update(sensor_data);


        //If CyBot bumps into an object
        //then it will move back into the previous tile
        //and return something that represents a cliff or a hole
        if(sensor_data->bumpLeft || sensor_data->bumpRight) {
            move_back(sensor_data, sum);
            printf("BUMPP\n");
            return (int)sum + 1000;          //Return the distance it has traveled, plus add 1000 to tell that it was a bump that it hit. You can change this up if you want to return something else. This is just what I had for now.

        }

        else if(sensor_data->cliffLeft || sensor_data->cliffFrontLeft || sensor_data->cliffFrontRight || sensor_data->cliffRight) {
            move_back(sensor_data, sum);
            printf("CLIFFFF\n");
            return (int)sum + 2000;          //Return the distance it has traveled, plus add 2000 to tell that it was a cliff that it hit. You can also change this stuff.
        }
        sum += sensor_data->distance;
    }

    oi_setWheels(0, 0);
    //If CyBot doesn't bump into an object
    //then it will just return a zero
    return 0;
}

//Moves the CyBot backward a distance
void move_back(oi_t *sensor, double distance) {

    oi_t *sensor_data = sensor;
    double sum = 0.0;           //Distance that the CyBot has currently traveled

    oi_setWheels(-140, -100);
    while(sum < distance) {
        oi_update(sensor_data);
        sum -= sensor_data->distance;
    }

    oi_setWheels(0,0);  //Stop
}


//Turns the CyBot 90 degrees clockwise
void turn_clockwises() {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    double sum = 0.0;

    oi_setWheels(-50, 50);
    while(-sum < 90) {
        oi_update(sensor_data);
        sum += sensor_data->angle;
    }

    oi_setWheels(0, 0);
}

//Turns the CyBot 90 degrees counterclockwise
void turn_counterclockwise() {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    double sum = 0.0;

    oi_setWheels(50, -50);
    while(sum < 90) {
        oi_update(sensor_data);
        sum += sensor_data->angle;
    }

    oi_setWheels(0, 0);
}
