/*
 * voltMonitor.c
 *
 *  Created on: 3 Aug 2018
 *      Author: Alex Wong
 */

#include <2_routine/inc/voltMonitor.h>
#include "ch.h"
#include "hal.h"
#include "adcDriver.h"

static voltages voltData;

static THD_WORKING_AREA(voltMonThd_wa, 1024);
static THD_FUNCTION(voltMonThd, p) {

  (void)p;

  static systime_t now = 0;
  static systime_t next = 0;

  while(true) {

    now = chVTGetSystemTime();
    next = now + US2ST(1000);
    adcDriverUpdate(&voltData);
    chThdSleepUntilWindowed(now, next);

  }

}

voltages* voltMonData(void) {

	return &voltData;

}

void voltInit(void) {

  adcDriverInit();

  chThdCreateStatic(voltMonThd_wa, sizeof(voltMonThd_wa),
                      NORMALPRIO + 5, voltMonThd, NULL);

  chThdSleep(MS2ST(500));		//make sure adc buffer actually contains sth before sending it back thru can

}


