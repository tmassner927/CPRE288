/*
 * main.c
 *
 *  Created on: Nov 30, 2021
 *      Author: tmassner
 */

#include <stdio.h>
#include <setjmp.h>
#include "math.h"
#include "uart.h"
#include "lcd.h"
#include "cyBot_Scan.h"
#include "open_interface.h"
#include "movement.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "move.h"


volatile char uart_data;

int main(void)
{
    lcd_init();
    ping_inits();
    button_init();
    init_button_interrupts();
    uart_init(115200);


    int direction = 0;                  //North = 0; East = 1; South = 2; West = 3;
    char input;
    int dist = 0;


    while(1) {
        scanf("%c", &input);

        if(input == '8'){
            while(direction != 0) {
                turn_clockwises();
                direction = (direction + 1) % 4;
            }
            dist = move();
        }
        else if(input == '6'){
            while(direction != 1) {
                turn_clockwises();
                direction = (direction + 1) % 4;
            }
            dist = move();
        }
        else if(input == '4'){
            while(direction != 2) {
                turn_clockwises();
                direction = (direction + 1) % 4;
            }
            dist = move();
        }
        else if(input == '2'){
            while(direction != 3) {
                turn_clockwises();
                direction = (direction + 1) % 4;
            }
            dist = move();
        }

        if(dist != 0) {

        }
    }


}
