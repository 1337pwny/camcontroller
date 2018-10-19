/**
 * \file globals.h
 * \author Torben Hellige
 * \brief This file provides the declarations for some global variables containing importen informations about the system state
 *
 *
 */


#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <stdint.h>

#define CAM_COUNT 4 	//!< Number of cameras in the system
#define STORE_COUNT 4 	//!< Number of stores for each cam

/**
 * \struct cam_data_t 
 * This struct contains all relevant data for one camera. It stores the DMX addresses, dmx values, setup parameter and positions
 */
typedef struct  {
	uint8_t  cam_active; //!< Is the cam active? (0/1)
	uint16_t base_addr;	//!< DMX base address (0-512)
	uint16_t pan_address;	//!< Offset of pan address from base
	uint16_t tilt_address;	//!< Offset of tilt address from base
	uint16_t speed_address; //!< Offset of speed address from base
	uint16_t switch_address;//!< Offset of switch address (most likely an color channel) from base
	uint8_t pan_invert;	//!< Invert pan joystick axis (0/1)
	uint8_t tilt_invert;	//!< Invert tilt joystick axis? (0/1)
	uint16_t pan_scaling;   //!< joystick scaling for pan axis
	uint16_t tilt_scaling;  //!< Joystick scaling for tilt axis
	uint8_t pan;		//!< Pan value (send over DMX)
	uint8_t tilt;		//!< Tilt value (send over DMX)
	uint8_t speed;		//!< Speed value (send over DMX)
	uint8_t button_state;	//!< GoPro button pressed? (0/1)
	uint8_t power_state;	//!< GoPro power on? (0/1)

	uint16_t store_pan[STORE_COUNT];  //!< Position stores for pan
	uint16_t store_tilt[STORE_COUNT]; //!< Position stores for tilt
    uint16_t focus;
    uint8_t iris;
    int8_t zoom;
} cam_data_t;

extern cam_data_t cams[CAM_COUNT];	//!< global array to store the configuration data for each cam
extern uint8_t active_cam; 		//!< global variable that contains the number of the currently manipulated camera
extern uint8_t active_rotary_funtion;
#endif
