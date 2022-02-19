/*
 * servo.c
 *
 *  Created on: Nov 2, 2021
 *      Author: tmassner
 */

#include "servo.h"


volatile int calibration;
int left_calibration;
int right_calibration;
float calibrationMultiple;
//Initializes the servo
void servo_inits(void) {

    //GPIO INITIALIZATION
    SYSCTL_RCGCGPIO_R |= 0b10;
    SYSCTL_RCGCTIMER_R |= 0b10;
    timer_waitMillis(5);
    GPIO_PORTB_DIR_R   |= 0b100000;
    GPIO_PORTB_AFSEL_R |= 0b100000;
    GPIO_PORTB_DEN_R   |= 0b100000;
    GPIO_PORTB_PCTL_R  &= 0x011111;
    GPIO_PORTB_PCTL_R  |= 0x700000;

    //TIMER INITIALIZATION
    TIMER1_CTL_R &= 0xEFF;               //Disables TIMER1B
    TIMER1_CFG_R = 0x4;                 //Configures register
    TIMER1_TBMR_R = 0b1010;             //Sets timer mode
    TIMER1_TBILR_R = 0x5F00;        //Loads the start time at 352,000 or a 22 ms period
    TIMER1_TBPR_R = 0x05;           //Loads the last digits of the start time
    TIMER1_TBMATCHR_R = 0x2080;     //Loads the match time at 336,000 or 21 ms low pulse
    TIMER1_TBPMR_R = 0x05;          //Loads the last digits of the match time
    TIMER1_CTL_R |= 0x100;           //Enables TIMER1B

    left_calibration = 315800;
    right_calibration = 345200;
    calibrationMultiple = 1;
}

//Calibrates the servo to 0 and 180 degrees
void servo_calibrate() {

    //Sets to 90 degrees
    TIMER1_TBMATCHR_R = 0x0140;
    TIMER1_TBPMR_R = 0x05;

    bool clockwise = true;
    int buttonNum = 0;
    calibration = 0;
    while(left_calibration == 336000 || right_calibration == 320000){
        buttonNum = 0;

        buttonNum = button_getButton();

        if(clockwise == true) {
            if(buttonNum == 1){
                calibration = (TIMER1_TBMATCHR_R & 0xFFFF) + ((TIMER1_TBPMR_R & 0xFF) << 16);
                calibration += 50;
                TIMER1_TBMATCHR_R = calibration & 0xFFFF;
                TIMER1_TBPMR_R = (calibration >> 16) & 0xFF;


            }
            else if(buttonNum == 2){
                calibration = (TIMER1_TBMATCHR_R & 0xFFFF) + ((TIMER1_TBPMR_R & 0xFF) << 16);
                calibration += 100;
                TIMER1_TBMATCHR_R = calibration & 0xFFFF;
                TIMER1_TBPMR_R = (calibration >> 16) & 0xFF;
            }
            else if(buttonNum == 3){
                clockwise = false;
            }
            else if(buttonNum == 4){
                right_calibration = (TIMER1_TBMATCHR_R & 0xFFFF) + ((TIMER1_TBPMR_R & 0xFF) << 16);
            }
        }
        else if(clockwise == false) {
            if(buttonNum == 1){
                calibration = (TIMER1_TBMATCHR_R & 0xFFFF) + ((TIMER1_TBPMR_R & 0xFF) << 16);
                calibration -= 50;
                TIMER1_TBMATCHR_R = calibration & 0xFFFF;
                TIMER1_TBPMR_R = (calibration >> 16) & 0xFF;
            }
            else if(buttonNum == 2){
                calibration = (TIMER1_TBMATCHR_R & 0xFFFF) + ((TIMER1_TBPMR_R & 0xFF) << 16);
                calibration -= 100;
                TIMER1_TBMATCHR_R = calibration & 0xFFFF;
                TIMER1_TBPMR_R = (calibration >> 16) & 0xFF;
            }
            else if(buttonNum == 3){
                clockwise = true;
            }
            else if(buttonNum == 4){
                left_calibration = (TIMER1_TBMATCHR_R & 0xFFFF) + ((TIMER1_TBPMR_R & 0xFF) << 16);
            }
        }
        lcd_printf("Clockwise: %d\nButtonNum: %d\nCounter: %d\nL:%d  R:%d", clockwise, buttonNum, calibration, left_calibration, right_calibration);

    }
    calibrationMultiple = (right_calibration - left_calibration) / 180;
    //lcd_printf("CALIBRATION DONE!!");
}

//Used to DEMO LAB 10 PART 2
void servo_calibrates() {

    //Sets to 90 degrees
    TIMER1_TBMATCHR_R = 0x0B04;
    TIMER1_TBPMR_R = 0x05;

    bool clockwise = true;
    int buttonNum = 0;
    calibration = 0;
    while(1){
        buttonNum = 0;

        buttonNum = button_getButton();

        if(clockwise == true) {
            if(buttonNum == 1){
                calibration = (TIMER1_TBMATCHR_R & 0xFFFF) + ((TIMER1_TBPMR_R & 0xFF) << 16);
                calibration += 50;
                TIMER1_TBMATCHR_R = calibration & 0xFFFF;
                TIMER1_TBPMR_R = (calibration >> 16) & 0xFF;


            }
            else if(buttonNum == 2){
                calibration = (TIMER1_TBMATCHR_R & 0xFFFF) + ((TIMER1_TBPMR_R & 0xFF) << 16);
                calibration += 100;
                TIMER1_TBMATCHR_R = calibration & 0xFFFF;
                TIMER1_TBPMR_R = (calibration >> 16) & 0xFF;
            }
            else if(buttonNum == 3){
                clockwise = false;
            }
            else if(buttonNum == 4){
                TIMER1_TBMATCHR_R = right_calibration & 0xFFFF;
                TIMER1_TBPMR_R = (right_calibration >> 16) & 0xFF;
            }
        }
        else if(clockwise == false) {
            if(buttonNum == 1){
                calibration = (TIMER1_TBMATCHR_R & 0xFFFF) + ((TIMER1_TBPMR_R & 0xFF) << 16);
                calibration -= 50;
                TIMER1_TBMATCHR_R = calibration & 0xFFFF;
                TIMER1_TBPMR_R = (calibration >> 16) & 0xFF;
            }
            else if(buttonNum == 2){
                calibration = (TIMER1_TBMATCHR_R & 0xFFFF) + ((TIMER1_TBPMR_R & 0xFF) << 16);
                calibration -= 100;
                TIMER1_TBMATCHR_R = calibration & 0xFFFF;
                TIMER1_TBPMR_R = (calibration >> 16) & 0xFF;
            }
            else if(buttonNum == 3){
                clockwise = true;
            }
            else if(buttonNum == 4){
                TIMER1_TBMATCHR_R = left_calibration & 0xFFFF;
                TIMER1_TBPMR_R = (left_calibration >> 16) & 0xFF;
            }
        }
        lcd_printf("Clockwise: %d\nButtonNum: %d\nCounter: %d\nL:%d  R:%d", clockwise, buttonNum, calibration, left_calibration, right_calibration);

    }
    calibrationMultiple = (right_calibration - left_calibration) / 180;
    lcd_printf("CALIBRATION DONE!!");
}

//Moves the servo whatever degrees specified
double servo_moves(float degrees) {
    int point = right_calibration - ((int)(degrees * calibrationMultiple));

    TIMER1_TBMATCHR_R = point & 0xFFFF;
    TIMER1_TBPMR_R = (point >> 16) & 0xFF;
    lcd_printf("Value: %d\nLeft: %d\nRight: %d", point, left_calibration, right_calibration);
    return (352000 - point) / 16000.0;
}

