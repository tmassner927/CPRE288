/**
 * lab5_template.c
 * 
 * Template file for CprE 288 lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"
#include "math.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#include "cyBot_Scan.h"  // For scan sensors 


#warning "Possible unimplemented functions"
#define REPLACEME 0


// Defined in button.c : Used to communicate information between the
// the interupt handeler and main.
extern volatile int button_event;
extern volatile int button_num;


int main(void) {
	button_init();
	lcd_init();
	cyBot_uart_init();            // Don't forget to initialze the cyBot UART before trying to use it
	init_button_interrupts();

    cyBot_uart_init_clean();  // Clean UART initialization, before running your UART GPIO init code

	// Complete this code for configuring the  (GPIO) part of UART initialization
    SYSCTL_RCGCGPIO_R |= 0b000010;
    timer_waitMillis(1);            // Small delay before accessing device after turning on clock
    GPIO_PORTB_AFSEL_R |= 0b00000011;
    GPIO_PORTB_PCTL_R &= 0xffffff00;     // Force 0's in the disired locations
    GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the disired locations
    GPIO_PORTB_DEN_R |= 0b00000011;
    GPIO_PORTB_DIR_R &= 0b11111110;      // Force 0's in the disired locations
    GPIO_PORTB_DIR_R |= 0b00000010;      // Force 1's in the disired locataions
    
    cyBot_uart_init_last_half();  // Complete the UART device initialization part of configuration
	
	// Initialze scan sensors
    cyBOT_init_Scan();


/*                                                      THIS IS THE PART 2 STUFF
	char test = 'h';
	//cyBot_sendByte(test);
	char tester[100] = "";
	int i;
	while(1)
	{
	


	    test = cyBot_getByte_blocking();
	    sprintf(tester, "\n\rI got a %c\n", test);

	    for(i = 0; tester[i] != '\0'; i++) {
	        cyBot_sendByte(tester[i]);
	    }



	
	}

*/


    char IR[100];
    cyBOT_Scan_t scan;
    int j;

    //cyBOT_SERVO_cal();        CALIBRATION FOR SCAN
    right_calibration_value = 290500;
    left_calibration_value = 1267000;



    while(button_getButton() != 2) {

    }


    while(1) {
        cyBOT_Scan(90, &scan);

        sprintf(IR, "\r%d %.1f\n", scan.IR_raw_val, scan.sound_dist);

        for(j = 0; IR[j] != '\0'; j++) {
            cyBot_sendByte(IR[j]);
        }

        if(button_getButton() == 1) {
            break;
        }
    }

/*
    double irToDist;
    while(1) {
        cyBOT_Scan(90, &scan);

        irToDist = 4000000.0 * (pow((double)scan.IR_raw_val, -1.616));
        sprintf(IR, "\r%d %.3f %.3f\n", scan.IR_raw_val, irToDist, scan.sound_dist);

        for(j = 0; IR[j] != '\0'; j++) {
            cyBot_sendByte(IR[j]);
        }

        if(button_getButton() == 1) {
            break;
        }
    }

*/
}
