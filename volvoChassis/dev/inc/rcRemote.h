/*
 * rcRemote.h
 *
 *  Created on: 19 Aug 2018
 *      Author: Alex Wong
 */

#ifndef INC_RCREMOTE_H_
#define INC_RCREMOTE_H_

#include "canBusProcess.h"

#define SERIAL_JUDGE                &SD6

#define JUDGE_BUFFER_SIZE           SERIAL_BUFFERS_SIZE

#define FLUSH_I_QUEUE(sdp)      \
    chSysLock();                \
    chIQResetI(&(sdp)->iqueue); \
    chSysUnlock();                          //Flush serial in queue

#define LEAST_SET_BIT(x)        x&(-x)      //Clear all but least set bit

#define JUDGEACQTIME            2           //Milliseconds

#define SERIAL_EVT_MASK         1

typedef struct joystick_t{

	int32_t forceReading;
	int16_t xReading;
	int16_t yReading;
	float correctedForceReading;

}__attribute__((packed)) joystick_t;

joystick_t* getJoystickData(void);
Remote_canStruct* getRemoteData(void);
void judgeinit(void);

#endif /* INC_RCREMOTE_H_ */
