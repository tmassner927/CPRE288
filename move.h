/*
 * move.h
 *
 *  Created on: Nov 16, 2021
 *      Author: tmassner
 */

#ifndef MOVE_H_
#define MOVE_H_

#include "open_interface.h"


//Moves the CyBot forward; Checks for bump detection as it goes
int move();

//Moves the CyBot backward a distance
void move_back(oi_t *sensor, double distance);

//Turns the CyBot 90 degrees clockwise
void turn_clockwises();

//Turns the CyBot 90 degrees counterclockwise
void turn_counterclockwise();

#endif /* MOVE_H_ */
