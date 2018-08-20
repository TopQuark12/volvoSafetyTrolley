/*
 * rcRemote.c
 *
 *  Created on: 19 Aug 2018
 *      Author: Alex Wong
 */

#include "rcRemote.h"
#include "canBusProcess.h"
#include <string.h>

Remote_canStruct remoteData;
static uint8_t sdrxbuf[JUDGE_BUFFER_SIZE];
static uint8_t datalength = 0;

joystick_t joystick;

joystick_t* getJoystickData(void) {

	return &joystick;

}

Remote_canStruct* getRemoteData(void) {

	return &remoteData;

}

static const SerialConfig SERIAL_JUDGE_CONFIG = {
  115200,               //Baud Rate
  USART_CR1_UE,         //CR1 Register
  USART_CR2_LINEN,      //CR2 Register
  0                     //CR3 Register
};

void serialDecode(void) {

	if (sdrxbuf[0] == 170 && sdrxbuf[9] == 170){
		memcpy(&joystick, sdrxbuf + 1, sizeof(uint8_t) * 8);
		joystick.correctedForceReading = (8315500.0 - joystick.forceReading) / 100.0;
		remoteData.updated = 1;
	}

}

static THD_WORKING_AREA(JudgeThread_wa, 1024);
static THD_FUNCTION(JudgeThread, arg) {

  (void)arg;

  static const eventflags_t serial_wkup_flags =                     //Partially from SD driver
    CHN_INPUT_AVAILABLE | CHN_DISCONNECTED | SD_NOISE_ERROR |       //Partially inherited from IO queue driver
    SD_PARITY_ERROR | SD_FRAMING_ERROR | SD_OVERRUN_ERROR |
    SD_BREAK_DETECTED;

  event_listener_t serial_listener;
  static eventflags_t pending_flags;
  static eventflags_t current_flag;
  chEvtRegisterMaskWithFlags(chnGetEventSource(SERIAL_JUDGE), &serial_listener,
                             SERIAL_EVT_MASK, serial_wkup_flags);   //setup event listening

  while (!chThdShouldTerminateX()) {


    chEvtWaitAny(1);                                                //wait for selected serial events
    chSysLock();
    pending_flags = chEvtGetAndClearFlagsI(&serial_listener);       //get event flag
    chSysUnlock();

    do {

      current_flag = LEAST_SET_BIT(pending_flags);                  //isolates single flag to work on
      pending_flags &= ~current_flag;                               //removes isolated flag

      switch(current_flag) {

      case CHN_INPUT_AVAILABLE:                                     //Serial data available
        chThdSleep(MS2ST(JUDGEACQTIME));                            //Acquire data packet, release CPU
        if((!pending_flags)) {
          datalength = sdAsynchronousRead(SERIAL_JUDGE, &sdrxbuf,
                                         (size_t)JUDGE_BUFFER_SIZE);  //Non-blocking data read
          serialDecode();
        }

        FLUSH_I_QUEUE(SERIAL_JUDGE);
        break;

      case CHN_DISCONNECTED:
        FLUSH_I_QUEUE(SERIAL_JUDGE);
        break;

      case SD_NOISE_ERROR:
        FLUSH_I_QUEUE(SERIAL_JUDGE);
        break;

      case SD_PARITY_ERROR:
        FLUSH_I_QUEUE(SERIAL_JUDGE);
        break;

      case SD_FRAMING_ERROR:
        FLUSH_I_QUEUE(SERIAL_JUDGE);
        break;

      case SD_OVERRUN_ERROR:
        FLUSH_I_QUEUE(SERIAL_JUDGE);
        break;

      case SD_BREAK_DETECTED:
        FLUSH_I_QUEUE(SERIAL_JUDGE);
        break;

      default:
        break;

      }

    } while (pending_flags);

    FLUSH_I_QUEUE(SERIAL_JUDGE);
    memset((void*)sdrxbuf, 0, JUDGE_BUFFER_SIZE);               //Flush RX buffer
  }

}

void judgeinit(void) {

	memset(&remoteData, 0, sizeof(remoteData));
	memset(&joystick, 0, sizeof(joystick));
  palSetPadMode(GPIOG, 14, PAL_MODE_ALTERNATE(8));              //UART6 TX
  palSetPadMode(GPIOG, 9, PAL_MODE_ALTERNATE(8));               //UART6 RX
  sdStart(SERIAL_JUDGE, &SERIAL_JUDGE_CONFIG);                  //Start Serial Driver
  chThdCreateStatic(JudgeThread_wa, sizeof(JudgeThread_wa),     //Start Judge RX thread
                    NORMALPRIO + 5, JudgeThread, NULL);

}
