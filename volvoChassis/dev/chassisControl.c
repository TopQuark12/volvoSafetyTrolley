/*
 * chassisControl.c
 *
 *  Created on: 3 Aug 2018
 *      Author: Alex Wong
 */

#include "chassisControl.h"
#include "canBusProcess.h"

static volatile Remote_canStruct* rcControl;
static volatile ChassisEncoder_canStruct* chassisData;

int16_t front_right;
int16_t back_right;
int16_t front_left;
int16_t back_left;

void driveKinematics(void) {

    int16_t drive  = rcControl->yJoystick;														//strafe
    int16_t strafe = rcControl->xJoystick * rcControl->button;				//rotate
    int16_t rotate = rcControl->xJoystick * (1 - rcControl->button);	//drive

//    int16_t strafe  = rcControl->yJoystick;														//strafe
//    int16_t rotate = rcControl->xJoystick * rcControl->button;				//rotate
//    int16_t drive = rcControl->xJoystick * (1 - rcControl->button);		//drive

    front_right = -(-1*rotate + strafe + drive);   // CAN ID: 0x201
    back_right = -(rotate + strafe + drive);       // CAN ID: 0x202
    front_left = (rotate - strafe + drive);       // CAN ID: 0x203
    back_left = (-1*rotate - strafe + drive);     // CAN ID: 0x204

}

void driveZero(void) {

  front_right = 0;
  back_right = 0;
  front_left = 0;
  back_left = 0;

}

static THD_WORKING_AREA(chassisControlThd_wa, 1024);
static THD_FUNCTION(chassisControlThd, p) {

  (void)p;

  static systime_t now = 0;
  static systime_t next = 0;

  while(true) {

    now = chVTGetSystemTime();
    next = now + US2ST(1000);

    if (rcControl->updated) {
    	driveKinematics();
    } else {
    	driveZero();
    }

    rcControl->updated = 0;

    can_motorSetCurrent(&CAND1, CAN_CHASSIS_CONTROL_ID, front_right,
    										back_right, front_left, back_left);

    chThdSleepUntilWindowed(now, next);

  }

}

void chassisInit(void) {

	rcControl = can_get_remoteData();
	chassisData = can_getChassisMotor();

  chThdCreateStatic(chassisControlThd_wa, sizeof(chassisControlThd_wa),
                      NORMALPRIO + 5, chassisControlThd, NULL);

}
