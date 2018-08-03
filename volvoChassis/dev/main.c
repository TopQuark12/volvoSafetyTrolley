/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include "main.h"

icucnt_t last_width;
icucnt_t last_period;

static void icuwidthcb(ICUDriver *icup) {

  //palSetPad(GPIOD, GPIOD_LED4);
  last_width = icuGetWidthX(icup);
}

static void icuperiodcb(ICUDriver *icup) {

  //palClearPad(GPIOD, GPIOD_LED4);
  last_period = icuGetPeriodX(icup);
}

static void icuoverflowcb(ICUDriver *icup) {

  //palClearPad(GPIOD, GPIOD_LED4);
  last_period = icuGetPeriodX(icup);

}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_HIGH,
  10000,                                    /* 10kHz ICU clock frequency.   */
  icuwidthcb,
  icuperiodcb,
  icuoverflowcb,
  ICU_CHANNEL_1,
  0
};

int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  can_processInit();
  chassisInit();

  last_period = 0;
  last_width = 0;
  icuStart(&ICUD8, &icucfg);
	palSetPadMode(GPIOI, 5, PAL_MODE_ALTERNATE(3));
	icuStartCapture(&ICUD8);
	icuEnableNotifications(&ICUD8);

  while (true)
  {

    chThdSleepMilliseconds(500);

  }

  return 0;

}
