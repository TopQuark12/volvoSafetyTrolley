/*
 * chassisControl.c
 *
 *  Created on: 3 Aug 2018
 *      Author: Alex Wong
 */

#include "chassisControl.h"
#include "canBusProcess.h"
#include "rcRemote.h"
#include <string.h>
#include "tof.h"

static volatile Remote_canStruct* rcControl;
static joystick_t* stick;
static volatile ChassisEncoder_canStruct* chassisData;
static icucnt_t* tofData;

float velCommand[4];
float currentCommand[4];
pidProfile_t wheelPID[4];
uint8_t openLoopControl = 0;

float kP = 1000;
float kI = 0;
float kD = 0;
float maxCurrent = 10000.0;
float maxI = 100.0;


void driveKinematics(void) {

    float drive  = rcControl->yJoystick / MAXJOYSTICKVAL;														//-1 to 1
    float strafe = 0;				//-1 to 1
    float rotate = -(stick->xReading - 2200)  / MAXXJOYSTICKVAL * XGAIN;	//-1 to 1

    currentCommand[frontRight] = -(-1*rotate + strafe + drive) * 10000;   	// CAN ID: 0x201
    currentCommand[backRight] = -(rotate + strafe + drive) * 10000;       	// CAN ID: 0x202
    currentCommand[frontLeft] = (rotate - strafe + drive) * 10000;      	 	// CAN ID: 0x203
    currentCommand[backLeft] = (-1*rotate - strafe + drive) * 10000;     	// CAN ID: 0x204

}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static uint8_t abs_limit(float *a, float ABS_MAX)
{
  if (*a > ABS_MAX) {
	  *a = ABS_MAX;
	  return 1;
  }

  if (*a < -ABS_MAX){
	  *a = -ABS_MAX;
	  return 1;
  }

  return 0;

}

static uint8_t maxed(int16_t a, float ABS_MAX)
{
  if (a > ABS_MAX) {
	  return 1;
  }

  if (a < -ABS_MAX){
	 return 1;
  }

  return 0;

}

float lastForce = 0.0;
float sumForce = 0.0;

void driveCloseLoop(uint8_t move) {

  if (move) {
		float drive  = ((stick->correctedForceReading > 200.0) && (tofData[0] > 25)) ?
									 stick->correctedForceReading / MAXFORCEVAL * FORCEGAIN :
									 0;														//-1 to 1
//  	float drive  = stick->correctedForceReading / MAXFORCEVAL * FORCEGAIN;
//  	stick->correctedForceReading = stick->correctedForceReading / MAXFORCEVAL;
//  	float drive = stick->correctedForceReading > FERROR?
//  								(stick->correctedForceReading - FERROR) * FORCEP +
//									((stick->correctedForceReading - FERROR) - lastForce) * FORCED +
//									sumForce * FORCEI :
//									0;
  	lastForce = stick->correctedForceReading - FERROR;
  	sumForce += stick->correctedForceReading - FERROR;
  	sumForce *= FORCEDECAY;
		float strafe = (stick->xReading - 2050)  / MAXXJOYSTICKVAL * XGAIN;				//-1 to 1
		float rotate = 0;	//-1 to 1	//-1 to 1



		velCommand[frontRight] = -(-1*rotate + strafe + drive) * MAXSPEED;   	// CAN ID: 0x201
		velCommand[backRight] = -(rotate + strafe + drive) * MAXSPEED;       	// CAN ID: 0x202
		velCommand[frontLeft] = (rotate - strafe + drive) * MAXSPEED;      	 	// CAN ID: 0x203
		velCommand[backLeft] = (-1*rotate - strafe + drive) * MAXSPEED;     	// CAN ID: 0x204
  } else {
		velCommand[frontRight] = 0;   // CAN ID: 0x201
		velCommand[backRight] = 0;    // CAN ID: 0x202
		velCommand[frontLeft] = 0;    // CAN ID: 0x203
		velCommand[backLeft] = 0;			// CAN ID: 0x204
  }

  for (uint8_t i = 0 ; i < 4 ; i++) {

  	uint8_t maxed = 0;
  	wheelPID[i].error = velCommand[i] - RPM2MPS(chassisData[i].raw_speed);
  	wheelPID[i].P = kP * wheelPID[i].error;
  	wheelPID[i].D = kD * (wheelPID[i].error - wheelPID[i].lastError);

  	currentCommand[i] = wheelPID[i].P + wheelPID[i].D + (wheelPID[i].I * kI);
  	maxed = abs_limit(&currentCommand[i], maxCurrent);

  	wheelPID[i].I = maxed ? 0: wheelPID[i].error + wheelPID[i].I;
  	abs_limit(&wheelPID[i].I, maxI);

  	wheelPID[i].lastError = wheelPID[i].error;

  }

}

void driveZero(void) {

	currentCommand[frontRight] = 0;
	currentCommand[backRight] = 0;
	currentCommand[frontLeft] = 0;
	currentCommand[backLeft] = 0;

}

static THD_WORKING_AREA(chassisControlThd_wa, 1024);
static THD_FUNCTION(chassisControlThd, p) {

  (void)p;

  static systime_t now = 0;
  static systime_t next = 0;

  kP = 30000;
  kI = 1000;
  kD = 0;
  maxI = 100000;

  memset(&velCommand, 0, sizeof(velCommand));
  memset(&currentCommand, 0, sizeof(currentCommand));
  memset(&wheelPID, 0, sizeof(pidProfile_t) * 4);

  while(true) {

    now = chVTGetSystemTime();
    next = now + US2ST(1000);

    if (palReadPad(GPIOD, 14) == 0) {

    		driveCloseLoop(MOVE);

    } else {

    		driveCloseLoop(BREAK);

    }

    rcControl->updated = 0;

    can_motorSetCurrent(&CAND1, CAN_CHASSIS_CONTROL_ID, currentCommand[frontRight],
    		currentCommand[backRight], currentCommand[frontLeft], currentCommand[backLeft]);

    chThdSleepUntilWindowed(now, next);

  }

}

void chassisInit(void) {

	rcControl = getRemoteData();
	stick = getJoystickData();
	chassisData = can_getChassisMotor();
	tofData = getTofData();
	palSetPadMode(GPIOD, 14, PAL_MODE_INPUT_PULLDOWN);

  chThdCreateStatic(chassisControlThd_wa, sizeof(chassisControlThd_wa),
                      NORMALPRIO + 5, chassisControlThd, NULL);

}
