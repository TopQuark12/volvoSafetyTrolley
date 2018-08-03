#pragma once
#include "mcuconf.h"

#define CHPRINTF_USE_FLOAT TRUE


#define GPIOE_LED_R                 11U
#define GPIOF_LED_G                 14U

#define LEDG_ON()       (palClearPad(GPIOF, GPIOF_LED_G))
#define LEDG_OFF()      (palSetPad(GPIOF, GPIOF_LED_G))
#define LEDG_TOGGLE()   (palTogglePad(GPIOF, GPIOF_LED_G))
#define LEDR_ON()       (palClearPad(GPIOE, GPIOE_LED_R))
#define LEDR_OFF()      (palSetPad(GPIOE, GPIOE_LED_R))
#define LEDR_TOGGLE()   (palTogglePad(GPIOE, GPIOE_LED_R))
#define LEDY_OFF()      (palClearPad(GPIOA, GPIOA_LED_Y))
#define LEDY_ON()       (palSetPad(GPIOA, GPIOA_LED_Y))
#define LEDY_TOGGLE()   (palTogglePad(GPIOA, GPIOA_LED_Y))
#define LEDB_OFF()      (palClearPad(GPIOA, GPIOA_LED_B))
#define LEDB_ON()       (palSetPad(GPIOA, GPIOA_LED_B))
#define LEDB_TOGGLE()   (palTogglePad(GPIOA, GPIOA_LED_B))
#define LASER_ON()      (palSetPad(GPIOG, GPIOG_PIN13_LASER))
#define LASER_OFF()     (palClearPad(GPIOG, GPIOG_PIN13_LASER))

// Sets of command to turn the new green led on and off in the RM2018 board
#define LEDG1_OFF()       (palSetPad(GPIOG, GPIOG_PIN1_LED))
#define LEDG2_OFF()       (palSetPad(GPIOG, GPIOG_PIN2_LED))
#define LEDG3_OFF()       (palSetPad(GPIOG, GPIOG_PIN3_LED))
#define LEDG4_OFF()       (palSetPad(GPIOG, GPIOG_PIN4_LED))
#define LEDG5_OFF()       (palSetPad(GPIOG, GPIOG_PIN5_LED))
#define LEDG6_OFF()       (palSetPad(GPIOG, GPIOG_PIN6_LED))
#define LEDG7_OFF()       (palSetPad(GPIOG, GPIOG_PIN7_LED))
#define LEDG8_OFF()       (palSetPad(GPIOG, GPIOG_PIN8_LED))
#define LEDG1_ON()        (palClearPad(GPIOG, GPIOG_PIN1_LED))
#define LEDG2_ON()        (palClearPad(GPIOG, GPIOG_PIN2_LED))
#define LEDG3_ON()        (palClearPad(GPIOG, GPIOG_PIN3_LED))
#define LEDG4_ON()        (palClearPad(GPIOG, GPIOG_PIN4_LED))
#define LEDG5_ON()        (palClearPad(GPIOG, GPIOG_PIN5_LED))
#define LEDG6_ON()        (palClearPad(GPIOG, GPIOG_PIN6_LED))
#define LEDG7_ON()        (palClearPad(GPIOG, GPIOG_PIN7_LED))
#define LEDG8_ON()        (palClearPad(GPIOG, GPIOG_PIN8_LED))
#define LEDG1_TOGGLE()    (palTogglePad(GPIOG, GPIOG_PIN1_LED))
#define LEDG2_TOGGLE()    (palTogglePad(GPIOG, GPIOG_PIN2_LED))
#define LEDG3_TOGGLE()    (palTogglePad(GPIOG, GPIOG_PIN3_LED))
#define LEDG4_TOGGLE()    (palTogglePad(GPIOG, GPIOG_PIN4_LED))
#define LEDG5_TOGGLE()    (palTogglePad(GPIOG, GPIOG_PIN5_LED))
#define LEDG6_TOGGLE()    (palTogglePad(GPIOG, GPIOG_PIN6_LED))
#define LEDG7_TOGGLE()    (palTogglePad(GPIOG, GPIOG_PIN7_LED))
#define LEDG8_TOGGLE()    (palTogglePad(GPIOG, GPIOG_PIN8_LED))

// Sets of command to turn the 24V output on and off in the RM2018 board
#define POWER1_ON()     (palSetPad(GPIOH, GPIOH_PIN2_POWER1))
#define POWER2_ON()     (palSetPad(GPIOH, GPIOH_PIN3_POWER2))
#define POWER3_ON()     (palSetPad(GPIOH, GPIOH_PIN4_POWER3))
#define POWER4_ON()     (palSetPad(GPIOH, GPIOH_PIN5_POWER4))
#define POWER1_OFF()    (palClearPad(GPIOH, GPIOH_PIN2_POWER1))
#define POWER2_OFF()    (palClearPad(GPIOH, GPIOH_PIN3_POWER2))
#define POWER3_OFF()    (palClearPad(GPIOH, GPIOH_PIN4_POWER3))
#define POWER4_OFF()    (palClearPad(GPIOH, GPIOH_PIN5_POWER4))