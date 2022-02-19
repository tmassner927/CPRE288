/*
 * adc.c
 *
 *  Created on: Oct 19, 2021
 *      Author: tmassner
 */

#include <stdio.h>
#include "adc.h"
#include "timer.h"

void adc_init(void) {
    SYSCTL_RCGCADC_R = 0b01;                //Module Init
    SYSCTL_RCGCGPIO_R |= 0b10;
    GPIO_PORTB_AFSEL_R |= 0b00010000;
    GPIO_PORTB_DIR_R &= 0b11101111;
    GPIO_PORTB_DEN_R &= 0b11101111;
    GPIO_PORTB_AMSEL_R |= 0b00010000;

    ADC0_ACTSS_R &= 0b1101;                 //Sample Sequencer configuration
    ADC0_EMUX_R &= 0x0F;
    ADC0_SSMUX1_R |= 0b1010;
    ADC0_SSCTL1_R |= 0b0110;
    ADC0_SAC_R &= 0x6;

    ADC0_ACTSS_R |= 0b0010;
}

int adc_read(void) {

    ADC0_PSSI_R |= 0x88000002;

    while((ADC0_RIS_R & 0b0010) == 0b0000) {
    }

    int val = (int)(ADC0_SSFIFO1_R & 0xFFF);

    ADC0_ISC_R |= 0b0010;
    return val;
}
