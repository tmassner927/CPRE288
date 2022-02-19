/**
 * lab6_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */

#define _RESET 0
#define _PART1 0
#define _PART2 0
#define _PART3 1

#include <stdio.h>
#include "timer.h"
#include "lcd.h"
#include "uart.h"

#if _RESET
#include "Simulation_User Files_resetSimulation.h"
#endif

// Adding global volatile varibles for communcating between 
// your Interupt Service Routine, and your non-interupt code.

volatile  char uart_data;  // Your UART interupt code can place read data here
volatile  char flag;       // Your UART interupt can update this flag
                           // to indicate that it has placed new data
                           // in uart_data                     


void main()
{
#if _RESET
    resetSimulationBoard();
#else

    timer_init();
    lcd_init();
    uart_init(115200);

#if _PART1      // Receive and display text
    char dataa[21];
    int i;
    int j;
    while(1) {
        for(i = 0; i < 20; i++){
            dataa[i] = uart_receive();
            if (dataa[i] == '\r') {
                break;
            }
        }
        for(j = 0; j < i; j++) {
            lcd_putc(dataa[j]);
        }

    }

#endif

#if _PART2      // Echo Received Character
	char data;
    while(1) {
        data = uart_receive();
        uart_sendChar(data);
	}

#endif

#if _PART3 // Receive and send characters using interrupts.
    uart_interrupt_init();
    while(1) {
        if(flag == 1) {
            UART1_DR_R = uart_data;

        }
    }
 
#endif

#endif
}

