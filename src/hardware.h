/**
 * \file hardware.h
 * \author Torben Hellige
 * \brief This file provides an abstraction layer to the hardware. All hardware-related calls for LEDs and buttons are done here
 */

#ifndef __HARDWARE_H
#define __HARDWARE_H

#include <stdint.h>

/**
 * \enum softkey_t
 *
 *
 */ 
	typedef enum {
		NO_KEY=255,
		SK1=0,
		SK2=1,
		SK3=2,
		SK4=3,
		ROT=4,
		SK1_R = 5,
		SK2_R = 6,
		SK3_R = 7,
		SK4_R = 8,
		ROT_R = 9
	} softkey_t;

	typedef enum {
		CAM_NO_KEY = 255,
		CAM1 = 0,
		CAM2 = 1,
		CAM3 = 2,
		CAM4 = 3
	} camkey_t;

	typedef enum {
		STORE_NO_KEY = 255,
		STORE1 = 0,
		STORE2 = 1,
		STORE3 = 2,
		STORE4 = 3
	} storekey_t;

	typedef enum {
        FSEL_NO_KEY = 255,
        FSEL1 = 0,
        FSEL2 = 1,
        FSEL3 = 2,
        FSEL4 = 3
    } fselkey_t;

	void hardware_tick(void);
	void hardware_init(void);
	softkey_t get_softkeys(void);
	camkey_t get_camkeys(void);
	storekey_t get_storekeys(void);
    fselkey_t get_fselkeys(void);
	uint8_t get_cam1_key(void);
	void set_store_led(uint8_t led);
	void reset_store_led(uint8_t led);
	void set_cam_leds(uint8_t active);
    void set_fsel_leds(uint8_t function);
	void set_rotarys_leds(uint8_t number);
	uint8_t get_matrix_line(uint8_t line);

#endif
