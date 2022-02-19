/*
 * movement.h
 *
 *  Created on: Sep 7, 2021
 *      Author: tzuchien
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

extern volatile char uart_data;

//Moves the iRobot forward "centimeters" amount in centimeters
void move_forward(oi_t *sensor, int centimeters);

//Same as move_forward except it moves the iRobot around an object if object is in front
void move_around(oi_t *sensor, int centimeters);

//Moves the iRobot backward "centimeters" amount in centimeters
void move_backward(oi_t *sensor, int centimeters);

//Turns the iRobot clockwise "degrees" angle
void turn_clockwise(oi_t *sensor, int degrees);

//Performs a manual or automatic scan depending on the given isManual
int scanner(cyBOT_Scan_t scann, oi_t *sensor, int isMan);

#endif /* MOVEMENT_H_ */
