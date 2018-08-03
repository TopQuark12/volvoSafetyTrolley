/*
 * canFeedback.c
 *
 *  Created on: 23 Jul 2018
 *      Author: Alex Wong
 */

#include "voltMonitor.h"
#include "ch.h"
#include "hal.h"
#include "bxCAN.h"
#include <string.h>
#include "adcDriver.h"

static canFeedback_t feedbackData;
static voltages* joystickData;

static THD_WORKING_AREA(canFeedbackThd_wa, 1024);
static THD_FUNCTION(canFeedbackThd, p) {

  (void)p;

  static systime_t now = 0;
  static systime_t next = 0;

  CANTxFrame txmsg;
  txmsg.IDE = CAN_IDE_STD;
  txmsg.SID = CAN_FEEDBACK_ID;
  txmsg.RTR = CAN_RTR_DATA;
  txmsg.DLC = 0x08;

  while(true) {

    now = chVTGetSystemTime();
    next = now + US2ST(1000);

    chSysLock();

    feedbackData.button = 1 - palReadPad(GPIOA, 0);
    feedbackData.xJoystick = -joystickData->xPot;
    feedbackData.yJoystick = joystickData->yPot;

    memcpy(txmsg.data8, &feedbackData, sizeof(feedbackData));

    chSysUnlock();

    canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(100));

    chThdSleepUntilWindowed(now, next);

  }

}

void canFeedbackInit(void) {

	joystickData = voltMonData();

  chThdCreateStatic(canFeedbackThd_wa, sizeof(canFeedbackThd_wa),
                    NORMALPRIO + 10, canFeedbackThd, NULL);

}
