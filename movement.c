/*
 * movement.c
 *
 *  Created on: Sep 7, 2021
 *      Author: tzuchien
 */

#include "open_interface.h"
#include "lcd.h"
#include "cyBot_Scan.h"
#include "uart.h"
#include "adc.h"
#include "servo.h"

extern volatile char uart_data;

//Moves the iRobot forward "centimeters" amount in centimeters
void move_backward(oi_t *sensor, int centimeters) {

    int millimeters = centimeters * 10;
    lcd_init();
    oi_t *sensor_data = sensor;
    int offset = 5;
    double sum = 0.0;
    oi_setWheels(-207, -200); // move backward; full speed      NOTE: ADDING OFFSET INTO SETWHEELS() CAUSES A GLITCH IN SIM
    while (sum < millimeters) {
        oi_update(sensor_data);

        sum -= sensor_data->distance;
        //lcd_printf("%lf", sum);
        lcd_printf("%lf    %d", sum, sensor_data->batteryCharge);
    }

    oi_setWheels(0, 0); // stop

}


//Turns the iRobot clockwise "degrees" angle
void turn_clockwise(oi_t *sensor, int degrees) {
    lcd_init();
    oi_t *sensor_data = sensor;
    double sum = 0.0;
    const double offset = 6.0;
    if (degrees > 0) {  //if degrees is positive
        oi_setWheels(-50, 50); // turn clockwise
        while (-sum < degrees - offset + 1.0) { //until it reaches degrees       USUALLY IT IS "degrees - offset" EXCEPT FOR SIM
            oi_update(sensor_data);
            sum += sensor_data->angle;
            //lcd_printf("%d", sensor_data->batteryCurrent);
            lcd_printf("%lf    %d", sum, sensor_data->batteryCharge);
        }


    }
    if (degrees < 0) {  //if degrees is negative
        oi_setWheels(50, -50); // move forward; full speed
        while (-sum > degrees + offset) {                                  //USUALLY IT IS "degrees + offset" EXCEPT FOR SIM
            oi_update(sensor_data);
            sum += sensor_data->angle;
            //lcd_printf("%lf", sum);
            lcd_printf("%lf    %d", sum, sensor_data->batteryCharge);
        }


    }


    oi_setWheels(0, 0); // stop


}

//Moves the iRobot forward "centimeters" amount in centimeters
void move_forward(oi_t *sensor, int centimeters) {


    lcd_init();
    oi_t *sensor_data = sensor;

    int millimeters = centimeters * 10;
    int offset = 5;
    double sum = 0.0;

    oi_setWheels(208, 200); // move forward; full speed       NOTE: ADDING OFFSET INTO SETWHEELS() CAUSES A GLITCH IN SIM
    while (sum < millimeters) {
        oi_update(sensor_data);

        if(sensor_data->bumpLeft) {
            move_backward(sensor_data, 15);
            turn_clockwise(sensor_data, 90);
            move_forward(sensor_data, 25);
            turn_clockwise(sensor_data, -90);
            sum -= 150;
            oi_setWheels(200, 200);
        }

        if(sensor_data->bumpRight) {
            move_backward(sensor_data, 15);
            turn_clockwise(sensor_data, -90);
            move_forward(sensor_data, 25);
            turn_clockwise(sensor_data, 90);
            sum -= 150;
            oi_setWheels(200, 200);
        }
        sum += sensor_data->distance;

        lcd_printf("%lf    %d", sum, sensor_data->batteryCharge);
    }

    oi_setWheels(0, 0); // stop

}

//Moves the iRobot forward "centimeters" amount in centimeters
void move_around(oi_t *sensor, int centimeters) {


    lcd_init();
    oi_t *sensor_data = sensor;

    int millimeters = centimeters * 10;
    int offset = 5;
    double sum = 0.0;
    int isTurn = 0;                 //isTurn = 1 means it hs bumped left and needs to correct right. isTurn = 1 means it has bumped right and needs to correct left
    oi_setWheels(208, 200); // move forward; full speed       NOTE: ADDING OFFSET INTO SETWHEELS() CAUSES A GLITCH IN SIM
    while (sum < millimeters) {
        oi_update(sensor_data);

        if(sensor_data->bumpLeft) {
            move_backward(sensor_data, 15);
            turn_clockwise(sensor_data, 90);
            move_around(sensor_data, 25);
            turn_clockwise(sensor_data, -90);
//            move_around(sensor_data, 55);
//            turn_clockwise(sensor_data, -90);
//            move_around(sensor_data, 25);
//            turn_clockwise(sensor_data, 90);
            isTurn = 1;
            sum -= 150;
            oi_setWheels(200, 200);
        }

        if(sensor_data->bumpRight) {
            move_backward(sensor_data, 15);
            turn_clockwise(sensor_data, -90);
            move_forward(sensor_data, 25);
            turn_clockwise(sensor_data, 90);
//            move_around(sensor_data, 55);
//            turn_clockwise(sensor_data, 90);
//            move_around(sensor_data, 25);
//            turn_clockwise(sensor_data, -90);
            isTurn = 2;
            sum -= 150;
            oi_setWheels(200, 200);
        }
        sum += sensor_data->distance;
        lcd_printf("%lf    %d", sum, sensor_data->batteryCharge);
    }

    if (isTurn == 1) {
        move_around(sensor_data, 10);
        turn_clockwise(sensor_data, -90);
        move_around(sensor_data, 5);
    }
    else if (isTurn == 2) {
        move_around(sensor_data, 10);
        turn_clockwise(sensor_data, 90);
        move_around(sensor_data, 5);
    }

    oi_setWheels(0, 0); // stop

}

int scanner(cyBOT_Scan_t scanner, oi_t *sensor, int Manual) {

    cyBOT_Scan_t scan = scanner;
    int isManual = Manual;
    int i;
    oi_t *sensor_data = sensor;
    char toPutty[100];
    int count = 0;          //To keep track of which object you are on
    int angle;              //To keep track of what angle you are currently scanning at
    int isObject = 0;       //If an object is currently detected it will be 1
    int firstAngle = 0;     //Used to calculate width
    int lastAngle = 0;      //Used to calculate width
    int width = 0;          //Keeps track of width
    float dist;             //Distance to the object
    double smallObjWidth = 500.0;//Smallest width object
    int smallObjAngle = 0;  //Smallest width object angle
    float smallObjDist = 0; //Smallest object distance to
    int isBackside = 0;     //Used to detect if the object is on its backside or not
    int IR_Data = 0;

    sprintf(toPutty, "\n\rObject#\tAngle\tDistance\tWidth\n");
    for (i = 0; toPutty[i] != '\0'; i++) {
        uart_sendChar(toPutty[i]);
    }

    for (angle = 0; angle <= 180; angle += 2) {     //Scans for every angle on the front side

        cyBOT_Scan(angle, &scan);
        //servo_moves(angle);
        IR_Data = adc_read();
        lcd_printf("Angle: %d\nDistance: %.0f\nIR: %d", angle, scan.sound_dist, scan.IR_raw_val);
        if (IR_Data > 1200) {           //If it detects something
            if(isObject == 0) {                 //If there is not already an object
                dist = scan.sound_dist;         //Get the distance to the object
                isObject = 1;                   //Remember that there is an object
                firstAngle = angle;             //Remember the first angle you saw it at
            }

        }

        else {
            if(isObject == 1) {

                isObject = 0;
                lastAngle = angle;
                width = lastAngle - firstAngle;
                cyBOT_Scan(firstAngle + width /2.0, &scan);
                //servo_moves(angle);
                double radWidth = width/2.0 * 0.0174533;
                double realWidth = sin(radWidth) * dist * 2;

                if (realWidth < smallObjWidth && realWidth > 3.0 && dist < 50.0) {
                    smallObjWidth = realWidth;
                    smallObjAngle = firstAngle + (width / 2);
                    smallObjDist = dist;
                }



                sprintf(toPutty, "\r%d\t%d\t%.0f\t\t%.3f\n", count + 1, firstAngle + (width / 2), dist, realWidth);           //FOR PUTTY

                for (i = 0; toPutty[i] != '\0'; i++) {
                    uart_sendChar(toPutty[i]);
                }

                count++;
            }
        }
        if (uart_data == 't') {             //if the user toggles into manual mode
            isManual = (isManual + 1) % 2;      //toggle the Manual vs Auto mode
            return isManual;
        }


    }
    if (isManual == 1) {
        return 1;
    }
    turn_clockwise(sensor_data, 180);               //Turns the robot around to scan the backside

    for (angle = 0; angle <= 180; angle += 2) {     //Scans for every angle on the back side
            cyBOT_Scan(angle, &scan);
            //servo_moves(angle);
            IR_Data = adc_read();
            lcd_printf("Angle: %d\nDistance: %.0f\nIR: %d", angle, scan.sound_dist, scan.IR_raw_val);
            if (IR_Data > 1200) {           //If it detects something
                if(isObject == 0) {                 //If there is not already an object
                    //dist = scan.sound_dist;         //Get the distance to the object
                    isObject = 1;                   //Remember that there is an object
                    firstAngle = angle;             //Remember the first angle you saw it at
                }

            }

            else {
                if(isObject == 1) {


                    isObject = 0;
                    lastAngle = angle;
                    width = lastAngle - firstAngle;
                    cyBOT_Scan(firstAngle + width /2.0, &scan);
                    //servo_moves(angle);
                    double radWidth = width/2.0 * 0.0174533;
                    double realWidth = sin(radWidth) * dist * 2;

                    if (realWidth < smallObjWidth && realWidth > 3.0 && dist < 50.0) {
                        smallObjWidth = realWidth;
                        smallObjAngle = firstAngle + (width / 2);
                        smallObjDist = dist;
                        isBackside = 1;
                    }



                    sprintf(toPutty, "\r%d\t%d\t%.0f\t\t%.3f\n", count + 1, -(firstAngle + (width / 2)), dist, realWidth);
                    for (i = 0; toPutty[i] != '\0'; i++) {
                        uart_sendChar(toPutty[i]);
                    }

                    count++;
                }
            }
            if (uart_data == 't') {         //if the user toggles into manual mode
                isManual = (isManual + 1) % 2;          //toggle the Manual vs Auto mode
                return isManual;
            }
    }

    cyBOT_Scan(smallObjAngle, &scan);                          //Turns its sensor to face the smallest object
    if(isBackside == 0) {                                       //If the object was on the other side then it must turn around before moving
        turn_clockwise(sensor_data, 180);
    }

    turn_clockwise(sensor_data, (-smallObjAngle + 90) );       //Turns the robot to face the object; must account for sensor and robot having different swivels
    move_around(sensor_data, (int)smallObjDist - 10);
    return isManual;
}



