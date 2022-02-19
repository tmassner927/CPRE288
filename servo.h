/*
 * servo.h
 *
 *  Created on: Nov 2, 2021
 *      Author: tmassner
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "timer.h"
#include "button.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>

//Initializes the servo
void servo_inits(void);

//Calibrates the servo to 0 and 180 degrees
void servo_calibrate();

void servo_calibrates();


//Moves the servo whatever degrees specified
double servo_moves(float degrees);



#endif /* SERVO_H_ */
