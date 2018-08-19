/*
 * chassisControl.h
 *
 *  Created on: 3 Aug 2018
 *      Author: Alex Wong
 */

#ifndef INC_CHASSISCONTROL_H_
#define INC_CHASSISCONTROL_H_

typedef enum {

	frontRight = 0,
	backRight,
	frontLeft,
	backLeft

} motorPos_e;

typedef enum {

	MOVE = 1,
	BREAK = 0

}movement_e;

typedef struct {

	float out;
	float error;
	float lastError;
	float P;
	float I;
	float D;

}pidProfile_t;

#define MAXJOYSTICKVAL			530.0

#define GEAR_RATIO					36.0
#define RPM2RPS							60.0
#define WHEEL_CIRCUM				0.08		//m
#define PI									3.14159265
#define RPM2MPS(x)					(((x / GEAR_RATIO) / RPM2RPS * WHEEL_CIRCUM) * PI)

#define MAXSPEED						0.5		//abs max 1.5

void chassisInit(void);

#endif /* INC_CHASSISCONTROL_H_ */
