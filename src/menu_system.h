#ifndef __MENU_STRUCTURE_H
#define __MENU_STRUCTURE_H

	#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "rotary.h"

	typedef enum {
		MENU_SPLASH =0,
		MENU_MAIN = 1,
		MENU_SETUP = 2,
		MENU_EDIT_CAM = 3,
		MENU_STORE = 4,
		MENU_CLEAR = 5,
		MENU_GENERAL_SETUP = 6,
		MENU_RESET = 7,
		MENU_CTRL = 8,
		MENU_CTRL_EDIT = 9,
		MENU_LOCKED = 10,
		MENU_LOCK_SETUP = 11,
		MENU_ALL_POWER = 12,
		MENU_HARDWARE = 13,
		MENU_LED_BRIGHTNESS = 14,
        MENU_BMMCC = 15,
		MENU_INVALID = 255
	} menu_identifiers;


	typedef void(*menu_button)(void);
	typedef void(*init_function)(void);
	typedef void(*cyclic_function)(void);

	// This struct defines a menu page
	// Each page contains the 4 LCD lines, the ID of the reachable menues, callbacks for each buttons and an init-function called when opening the menu
	//
	typedef struct
	{
		const char* lines[4];				//Display content
		menu_identifiers next[5];	//Menu to reach if a softkey is pressed
		menu_button cb[5];			//Callback for buttonpresses
		menu_button cb_r[5];
		init_function init;			//Init function
		cyclic_function cyclic;
		rotary_config_t* rotary;
	} menu_t;

	extern __flash const menu_t menues[];
	extern menu_identifiers active_menu;

	void set_menu(menu_identifiers menu);
	void process_menu(void);
#endif
