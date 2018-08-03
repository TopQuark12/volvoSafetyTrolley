/*
 * main.c
 *
 *  Created on: 5 Jul 2018
 *      Author: Alex's Desktop
 */

#include <2_routine/inc/canFeedback.h>
#include <2_routine/inc/voltMonitor.h>
#include "ch.h"
#include "hal.h"

#include "adcDriver.h"
#include "bxCAN.h"

int main(void) {

  //ChibiOS init
  halInit();
  chSysInit();

  //Driver init
  palSetPadMode(GPIOA, 0, PAL_MODE_INPUT);		//init pushbutton on joystick
  can_processInit();

  //Routine init
  voltInit();
  canFeedbackInit();

  while (true) {

    chThdSleepMilliseconds(100);

  }
}
