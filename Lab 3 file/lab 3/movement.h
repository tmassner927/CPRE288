/*
 * movement.h
 *
 *  Created on: Sep 7, 2021
 *      Author: tzuchien
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

//Moves the iRobot forward "centimeters" amount in centimeters
void move_forward(oi_t *sensor, int centimeters);

//Turns the iRobot clockwise "degrees" angle
void turn_clockwise(oi_t *sensor, int degrees);



#endif /* MOVEMENT_H_ */
