/*
 * ADC.c
 *
 *  Created on: 12 Jul 2018
 *      Author: Alex's Desktop
 */

#include "ch.h"
#include "hal.h"
#include "adcDriver.h"

static adcsample_t sample [ADCDEPTH] [ADCNUMCH];
static adcsample_t avg    [ADCNUMCH];

/*
 * Joystick handle position sensing
 */
static const ADCConversionGroup adcCfg = {

  TRUE,	                 										//Continuous conversion
  ADCNUMCH,              										//Number of ADC channels
  NULL,                  										//end of conversion callback
	NULL,						      										//ADC error callback
	0,						           									//CR1
  ADC_CR2_TSVREFE,       										//CR2
  0,                                        //Sample time setting 1
  ADC_SMPR2_SMP_AN1(ADC_SAMPLE_55P5) |
  ADC_SMPR2_SMP_AN2(ADC_SAMPLE_55P5),       //Sample time setting 2
  ADC_SQR1_NUM_CH(ADCNUMCH),                //Sequence setting 1
  0,                                        //Sequence setting 2
  ADC_SQR3_SQ2_N(ADC_CHANNEL_IN2)   |
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN1)           //Sequence setting 3

};

void adcDriverInit(void) {

  palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_ANALOG);
  palSetPadMode(GPIOA, 2, PAL_MODE_INPUT_ANALOG);

  adcStart(&ADCD1, NULL);

  adcStartConversion(&ADCD1, &adcCfg, (adcsample_t*) &sample, ADCDEPTH);

}

void * memset (void *block, int c, size_t size);

void adcDriverUpdate(voltages* data) {

  memset(avg, 0, sizeof(avg));

  for (uint8_t channelno = 0; channelno < ADCNUMCH; channelno++) {

    for (uint8_t sampleno = 0; sampleno < ADCDEPTH; sampleno++) {
      avg[channelno] += sample[sampleno] [channelno];
    }

    avg[channelno] = (uint16_t) avg[channelno] / ADCDEPTH;

  }

  data->xPot = (int16_t) (avg[0] - XPOTMIDPOINT);
  data->yPot = (int16_t) (avg[1] - YPOTMIDPOINT);

}
