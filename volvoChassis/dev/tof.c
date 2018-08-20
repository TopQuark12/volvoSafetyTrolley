/*
 * tof.c
 *
 *  Created on: 4 Aug 2018
 *      Author: Alex Wong
 */

#include "ch.h"
#include "hal.h"
#include "tof.h"

icucnt_t last_width[TOFSENSORSNUM];
icucnt_t last_period[TOFSENSORSNUM];

icucnt_t* getTofData(void) {

	return last_width;

}

static void icuwidthcb(ICUDriver *icup) {

	if(icup == &ICUD4) {
  	last_width[0] = icuGetWidthX(icup) / 100;  //gives cm
	} else if (icup == &ICUD5) {
		last_width[1] = icuGetWidthX(icup) / 100;
	} else if (icup == &ICUD8) {
		last_width[2] = icuGetWidthX(icup) / 100;
	}

}

static void icuperiodcb(ICUDriver *icup) {

	if(icup == &ICUD4) {
  	last_period[0] = icuGetPeriodX(icup);
	} else if (icup == &ICUD5) {
  	last_period[1] = icuGetPeriodX(icup);
	} else if (icup == &ICUD8) {
  	last_period[2] = icuGetPeriodX(icup);
	}

}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_HIGH,
  1000000,                                    /* 1MHz ICU clock frequency.   */
  icuwidthcb,
  icuperiodcb,
  NULL,
  ICU_CHANNEL_1,
  0
};

void tofInit(void) {

  memset(&last_period, 0, sizeof(icucnt_t) * TOFSENSORSNUM);
  memset(&last_width, 0, sizeof(icucnt_t) * TOFSENSORSNUM);
  icuStart(&ICUD4, &icucfg);
  palSetPadMode(GPIOD, 12, PAL_MODE_ALTERNATE(2));
	icuStartCapture(&ICUD4);
	icuEnableNotifications(&ICUD4);

}
