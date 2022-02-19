/**
 * lab4_template.c
 * 
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {
    cyBot_uart_init();
    button_init();
	lcd_init();


	int num;
	char putty[100];
	int i;
	while(1)
	{
	    num = button_getButton();
	    lcd_printf("%d", num);
	    sprintf(putty, "\rYou pushed button %d\n", num);

	    for(i = 0; putty[i] != '\0'; i++) {
	        cyBot_sendByte(putty[i]);
	    }

	}
	
}
