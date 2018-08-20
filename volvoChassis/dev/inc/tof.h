/*
 * tof.h
 *
 *  Created on: 4 Aug 2018
 *      Author: Alex Wong
 */

#ifndef INC_TOF_H_
#define INC_TOF_H_

#define TOFSENSORSNUM			1

icucnt_t* getTofData(void);
void tofInit(void);

#endif /* INC_TOF_H_ */
