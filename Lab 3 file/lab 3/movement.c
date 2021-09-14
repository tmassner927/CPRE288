/*
 * movement.c
 *
 *  Created on: Sep 7, 2021
 *      Author: tzuchien
 */

#include "open_interface.h"
#include "lcd.h"

//Moves the iRobot forward "centimeters" amount in centimeters
void move_forward(oi_t *sensor, int centimeters) {

    lcd_init();
    oi_t *sensor_data = sensor;
    double sum = 0;
    oi_setWheels(475, 500); // move forward; full speed
    while (sum < centimeters) {
        oi_update(sensor_data);
        sum += sensor_data->distance;
        lcd_printf("%lf", sum);
    }

    oi_setWheels(0, 0); // stop

}

//Turns the iRobot clockwise "degrees" angle
void turn_clockwise(oi_t *sensor, int degrees) {
    lcd_init();
    oi_t *sensor_data = sensor;
    double sum = 0.0;
    const double offset = 3.7;
    if (degrees > 0) {  //if degrees is positive
        oi_setWheels(100, -100); // turn clockwise
        while (sum < degrees - offset) { //until it reaches degrees
            oi_update(sensor_data);
            sum += sensor_data->angle;
            lcd_printf("%lf", sum);
        }


    }
    if (degrees < 0) {  //if degrees is negative
        oi_setWheels(-100, 100); // move forward; full speed
        while (sum > degrees + offset) {
            oi_update(sensor_data);
            sum += sensor_data->angle;
            lcd_printf("%lf", sum);
        }


    }


    oi_setWheels(0, 0); // stop


}



