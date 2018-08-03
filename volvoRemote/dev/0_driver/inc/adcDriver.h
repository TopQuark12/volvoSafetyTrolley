/*
 * ADC.h
 *
 *  Created on: 12 Jul 2018
 *      Author: Alex's Desktop
 */

#ifndef ADCDRIVER_H_
#define ADCDRIVER_H_

#include "ch.h"
#include "hal.h"

#define ADCNUMCH            2
#define ADCDEPTH            10

#define ADC2MV              8.862304688f

#define XPOTMIDPOINT				2070
#define YPOTMIDPOINT				2025

typedef struct voltages{

  int16_t xPot;
  int16_t yPot;

} voltages;

void adcDriverInit(void);
void adcDriverUpdate(voltages* data);

#endif /* 0_DRIVER_INC_ADC_H_ */
