/*
 * bxCAN.h
 *
 *  Created on: 16 Jul 2018
 *      Author: Alex Wong
 */

#ifndef BXCAN_H_
#define BXCAN_H_

#define CAN_FEEDBACK_ID				0x050
#define CAN_CONTROL_ID				0x055

typedef enum {

	notUpdated = 0,
	updated = 1

} msgUpdatedEnum;

typedef struct canFeedback_t{

	int16_t xJoystick;
	int16_t yJoystick;
	uint8_t button;

}__attribute__((packed)) canFeedback_t;

//userCommand_t* returnCommand(void);
void can_processInit(void);

#endif /* 0_DRIVER_INC_BXCAN_H_ */
