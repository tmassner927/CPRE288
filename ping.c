/*
 * ping.c
 *
 *  Created on: Oct 28, 2021
 *      Author: tmassner
 */
#include "ping.h"
#include "driverlib/interrupt.h"

volatile enum {LOW, HIGH, DONE} state;
volatile int start_time;
volatile int end_time;

void ping_inits(void) {

    //INITIALIZATION
    SYSCTL_RCGCTIMER_R |= 0b1000;
    SYSCTL_RCGCGPIO_R |= 0b10;
    timer_waitMicros(10);

    GPIO_PORTB_DEN_R |= 0b1000;
    GPIO_PORTB_DIR_R |= 0b1000;

    TIMER3_CTL_R |= 0xD00;
    TIMER3_CFG_R |= 0x4;
    TIMER3_TBMR_R |= 0b111;
    TIMER3_TBMR_R &= ~0b10000;
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_IMR_R |= 0x400;
    TIMER3_ICR_R |= 0x400;

    NVIC_EN1_R = 0x10;
    IntRegister(INT_TIMER3B, TIMER3B_HANDLER);
    IntMasterEnable();
//    timer_waitMillis(5);
//
//    GPIO_PORTB_AFSEL_R |= 0b1000;
//
//    GPIO_PORTB_PCTL_R = 0x700;
//
//
//
//
//
//    //TIMER INITIALIZATION
//          //Turns off TIMER B
//
//
//    TIMER3_CTL_R |= 0xC00;       //Chooses type of event: chose both sides
//
//
//    TIMER3_ICR_R |= 0x400;
//
//    NVIC_EN1_R |= 0b10000;

//    TIMER3_CTL_R |= 0x100;
//    IntMasterEnable();

}

void TIMER3B_HANDLER() {

    if(state == LOW) {
        start_time = TIMER3_TBR_R;
        state = HIGH;
    }

    else if(state == HIGH) {
        end_time = TIMER3_TBR_R;
        state = DONE;
    }

    //Clear status flag
    TIMER3_ICR_R |= 0x400;
}

void send_pulse(void) {

    //Set PB3 in GPIO mode
//    GPIO_PORTB_AFSEL_R &= ~0b1000;

    GPIO_PORTB_DEN_R |= 0b1000;

    //Set PB3 as an output
    GPIO_PORTB_DIR_R |= 0b1000;

    //Set PB3 to 0
//    GPIO_PORTB_DATA_R &= ~0b1000;

    //Mask Timer Interrupt
//    TIMER3_IMR_R &= 0x0FF;

    //Turn off timer
//    TIMER3_CTL_R &= ~0x100;

    //Set PB3 to 1
    GPIO_PORTB_DATA_R |= 0b1000;

    //Wait at-least 5 microseconds
    timer_waitMicros(7);

    //Set PB3 to 0
    GPIO_PORTB_DATA_R &= ~0b1000;

    //Set PB3 as an input
    GPIO_PORTB_DIR_R &= ~0b1000;

    //Set PB3 to use alternate hardware
    GPIO_PORTB_AFSEL_R |= 0b1000;

    GPIO_PORTB_PCTL_R &= ~0xF000;
    GPIO_PORTB_PCTL_R |= 0x7000;


    //Unmask Timer Interrupt
//    TIMER3_IMR_R |= 0x400;

    //Clear Timer interrupt
//    TIMER3_ICR_R |= 0x400;

}

int ping_read(void) {

    //Sends a trigger pulse to start the PING sensor
    send_pulse();

    state = LOW;

    //Catch rising edge
    while(state != DONE) {
    }

    GPIO_PORTB_AFSEL_R &= ~0b1000;

    if(start_time < end_time) {
        start_time += 16777215;
    }

    GPIO_PORTB_PCTL_R &= ~0xF000;
    GPIO_PORTB_AFSEL_R &= ~0b1000;

    return (start_time - end_time) * 100 * 340 / 16000000 / 2;
}
