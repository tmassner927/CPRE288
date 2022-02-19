/*
*
*   uart_extra_help.c
* Description: This is file is meant for those that would like a little
*              extra help with formatting their code, and followig the Datasheet.
*/
#include <stdio.h>
#include "uart.h"
#include "timer.h"

#define REPLACE_ME 0x00

extern volatile char uart_data;

//extern volatile  char flag;

void uart_init(int baud)
{
    ///*
    SYSCTL_RCGCGPIO_R |= 0b000010;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0b00000010;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R = 0b00000011;      // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R  = 0x00000011;       // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R   = 0b00000011;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R   = 0b00000001;        // sets pb0 as output, pb1 as input
//*/
/*
    SYSCTL_RCGCGPIO_R |= 0b000010;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0b00000010;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R |= 0b00000011;      // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R &= 0xffffff00;
    GPIO_PORTB_PCTL_R |= 0x00000011;
    GPIO_PORTB_DEN_R |= 0b00000011;         // enables pb0 and pb1
    GPIO_PORTB_DIR_R &= 0b11111101;         // sets pb0 as output, pb1 as input
    GPIO_PORTB_DIR_R |= 0b00000001;       // pmc0 and pmc1       (page 688)  also refer to page 650
*/

    //compute baud values [UART clock= 16 MHz] 
    double brd;
    int    ibrd;
    int    fbrd;

    brd = 16000000.0 / (16.0 * baud); // page 903
    ibrd = (int)brd;
    fbrd = (int)((brd - ibrd) * 64 + .5);

    UART1_CTL_R &= 0b110011111110;      // disable UART1 (page 918)
    UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = fbrd;   // write fractional portion of BRD to FBRD
    UART1_LCRH_R = 0b01100000;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R   = 0b0000;          // use system clock as clock source (page 939)
    UART1_CTL_R |= 0b001100000001;        // enable UART1

}

void uart_sendChar(char data)
{
   while(UART1_FR_R & 0b00100000){
   }

   UART1_DR_R = data;
   
   if (data == '\r') {
       uart_sendChar('\n');
   }
}

char uart_receive(void)
{
    char data = 0;

    while (UART1_FR_R & 0b00010000){
    }

    data = (char)(UART1_DR_R & 0xFF);
    return data;
}

void uart_sendStr(const char *data)
{
    int i;
    for(i = 0; data[i] != '\0'; i++) {
        UART1_DR_R = data[i];
    }
	
}

// _PART3


void uart_interrupt_init()
{
    //turn off uart1 while we set it up
    UART1_CTL_R &= 0b110011111110;

    //clear interrupt flags
    UART1_ICR_R |= 0b1011111110010;

    // Enable interrupts for receiving bytes through UART1
    UART1_IM_R |= 0b00110000; //enable interrupt on receive - page 924

    //set priority of usart1 interrupt: Example to 1. group 1 bits 21-23
    NVIC_PRI1_R |= 0x00200000;

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    NVIC_EN0_R |= 0b01000000; //enable uart1 interrupts - page 104

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number

    //enable global interrupts
    IntMasterEnable();

    //re-enable enable RX, TX, and uart1
    UART1_CTL_R |= 0b001100000001;

}

void uart_interrupt_handler()
{
// STEP1: Check the Masked Interrup Status
    if(UART1_MIS_R & 0b10000) {         //check if a receive byte IRQ has occured

        uart_data = (char)(UART1_DR_R & 0xFF);

        //flag = 1;                 //PART 3
        UART1_ICR_R |= 0b10000;         //clear the receive byte interrupt
    }

    else if(UART1_MIS_R & 0b100000) {   //check if a transmit byte IRQ has occured

        //uart_data = 0;            //PART 3
        //flag = 0;                 //PART 3
        UART1_ICR_R |= 0b100000;        //clear the transmit byte interrupt
    }

//STEP2:  Copy the data 


//STEP3:  Clear the interrup   

}
