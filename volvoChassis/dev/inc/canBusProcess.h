
#ifndef _CAN_BUS_PROCESS_H_
#define _CAN_BUS_PROCESS_H_

#include "stdint.h"
#include "stdbool.h"
#include "hal.h"

#define CHASSIS_MOTOR_NUM 4U

/* CAN Bus 1 or 2 */
#define CAN_CHASSIS_CONTROL_ID											0x200

#define CAN_CHASSIS_FR_FEEDBACK_MSG_ID              0x201
#define CAN_CHASSIS_FL_FEEDBACK_MSG_ID              0x203
#define CAN_CHASSIS_BL_FEEDBACK_MSG_ID              0x204
#define CAN_CHASSIS_BR_FEEDBACK_MSG_ID              0x202

#define CAN_REMOTE_RECEIVER_ID                			0x050
#define CAN_REMOTE_SEND_MODE_ID               			0x055

#define CAN_ENCODER_RANGE           8192            // 0x2000
#define CAN_ENCODER_RADIAN_RATIO    7.669904e-4f    // 2*M_PI / 0x2000

typedef enum
{
    FRONT_RIGHT = 0,
    FRONT_LEFT = 2,
    BACK_LEFT = 3,
    BACK_RIGHT = 1
}chassis_num_t;

typedef struct {
    uint16_t raw_angle;
    int16_t  raw_speed;
    int16_t act_current;
    uint8_t temperature;

    uint16_t last_raw_angle;
    uint16_t offset_raw_angle;
    uint32_t msg_count;
    int32_t round_count;
    int32_t total_ecd;
    float radian_angle; // Continuous

    bool updated;
} ChassisEncoder_canStruct;

typedef struct {

	int16_t xJoystick;
	int16_t yJoystick;
	uint8_t button;
	uint8_t updated;

}__attribute__((packed)) Remote_canStruct;

volatile ChassisEncoder_canStruct* can_getChassisMotor(void);
volatile Remote_canStruct* can_get_remoteData(void);

void can_processInit(void);
void can_motorSetCurrent(CANDriver *const CANx,
  const uint16_t EID,
  const int16_t cm1_iq,
  const int16_t cm2_iq,
  const int16_t cm3_iq,
  const int16_t cm4_iq);


#endif
