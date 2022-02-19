/*
 * adc.h
 *
 *  Created on: Oct 19, 2021
 *      Author: tmassner
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>

//Initializes Analog to Digital Converter
void adc_init(void);


//Takes a sample of the data
int adc_read(void);



#endif /* ADC_H_ */
