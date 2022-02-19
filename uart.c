/*
*
*   uart.c
*
*
*
*
*
*   @author
*   @date
*/

#include "uart.h"
#include "timer.h"

extern volatile char uart_data;
void uart_init(void){
	SYSCTL_RCGCUART_R |= 0b00000010;
	SYSCTL_RCGCGPIO_R |= 0b000010;
	GPIO_PORTB_AFSEL_R |= 0b00000011;
	GPIO_PORTB_PCTL_R &= 0xffffff00;
	GPIO_PORTB_PCTL_R |= 0x00000011;
	GPIO_PORTB_DEN_R |= 0b00000011;
	GPIO_PORTB_DIR_R &= 0b11111101;
	GPIO_PORTB_DIR_R |= 0b00000001;

	//compute baud values [UART clock= 16 MHz]
	double fbrd;
	int    ibrd;

    fbrd = 16000000 / (16 * 115200); // page 903
    ibrd = (int)fbrd;
    fbrd = (int)((fbrd - ibrd) * 64 + .5);

    UART1_CTL_R &= 0b11111110;      // disable UART1 (page 918)
    UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = fbrd;   // write fractional portion of BRD to FBRD
    UART1_LCRH_R = 0b01100000;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R   = 0b0000;          // use system clock as clock source (page 939)
    UART1_CTL_R |= 0b00000001;        // enable UART1


}

void uart_sendChar(char data){
	UART1_DR_R = data;
}

char uart_receive(void){
	//return UART1_DR_R;

    uart_data = UART1_DR_R;
    return uart_data;
}

void uart_sendStr(const char *data){
	//TODO for reference see lcd_puts from lcd.c file
}
