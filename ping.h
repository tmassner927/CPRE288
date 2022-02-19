/*
 * ping.h
 *
 *  Created on: Oct 28, 2021
 *      Author: tmassner
 */

#ifndef PING_H_
#define PING_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "timer.h"
#include <inc/tm4c123gh6pm.h>

//volatile enum {LOW, HIGH, DONE} state;
//volatile int start_time;
//volatile int end_time;


//Initializes the timer registers to setup ping
void ping_inits(void);


//Timer interrupt handler
void TIMER3B_HANDLER(void);


//Sends the initial pulse to trigger the PING sensor
void send_pulse(void);

//Sends out a pulse and records the time it takes to recieve it back to calculate distance
//Returns the distance
int ping_read(void);

#endif /* PING_H_ */
