/*
 * bxCAN.c
 *
 *  Created on: 16 Jul 2018
 *      Author: Alex Wong
 */

#include "ch.h"
#include "hal.h"
#include "bxCAN.h"

// userCommand_t commandInput;
canFeedback_t feedBack;

static const CANConfig cancfg = {
  CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
  CAN_BTR_SJW(0) | CAN_BTR_TS2(1) |
  CAN_BTR_TS1(8) | CAN_BTR_BRP(2)
};

#define CAN_FILTER_NUM 28U
static CANFilter canfilter[CAN_FILTER_NUM];

//userCommand_t* returnCommand(void) {
//
//	return &commandInput;
//
//}

// static void canProcessControl(const CANRxFrame* const rxmsg) {

// 	if (rxmsg->data8[0] <= BOOST) {

// 		commandInput.pathType = rxmsg->data8[0];
// 		commandInput.robotType = rxmsg->data8[1];
// 		commandInput.msgUpdated = TRUE;

// 	}

// }

static void canRxCallback(const CANRxFrame* const rxmsg)
{
  switch(rxmsg->SID)
  {
      case CAN_CONTROL_ID:
    	  // canProcessControl(rxmsg);
        break;
  }
}

static THD_WORKING_AREA(canRxWa, 256);
static THD_FUNCTION(canRx, p) {

  CANDriver* canp = (CANDriver*)p;
  event_listener_t el;
  CANRxFrame rxmsg;

  (void)p;
  chRegSetThreadName("can receiver");
  chEvtRegister(&canp->rxfull_event, &el, 0);
  while(!chThdShouldTerminateX())
  {
    if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(100)) == 0)
      continue;
    while (canReceive(canp, CAN_ANY_MAILBOX,
                      &rxmsg, TIME_IMMEDIATE) == MSG_OK)
    {
    	canRxCallback(&rxmsg);
    }
  }
  chEvtUnregister(&canp->rxfull_event, &el);
}


void can_processInit(void)
{

	palSetPadMode(GPIOA, 11, PAL_MODE_INPUT);
	palSetPadMode(GPIOA, 12, PAL_MODE_STM32_ALTERNATE_PUSHPULL);

  uint8_t i;
  for (i = 0; i < CAN_FILTER_NUM; i++)
  {
    canfilter[i].filter = i;
    canfilter[i].mode = 0; //CAN_FilterMode_IdMask
    canfilter[i].scale = 1; //CAN_FilterScale_32bit
    canfilter[i].assignment = 0;
    canfilter[i].register1 = 0;
    canfilter[i].register2 = 0;
  }

  //canSTM32SetFilters(14, CAN_FILTER_NUM, canfilter);

  canStart(&CAND1, &cancfg);

  chThdCreateStatic(canRxWa, sizeof(canRxWa), NORMALPRIO + 7,
  									canRx, (void *)&CAND1);

  chThdSleepMilliseconds(200);
}
