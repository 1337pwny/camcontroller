#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>

#include <string.h>
#include "lcd.h"
#include "dmx.h"
#include "adc.h"
#include "hardware.h"
#include "rotary.h"
#include "menu_system.h"
#include "menu_main.h"
#include "menu_lock.h"
#include "menu_cam_ctrl.h"
#include "globals.h"
#include "cccb.h"




void store_clear(void);
void save_data(void);

void process_inputs(void);
void update_leds(void);
void load_default(void);

void lock_system(void);
void unlock_system(void);




//###########################
//  Global variables
//###########################



const cam_data_t cams_default[CAM_COUNT] PROGMEM = 
{
	{
		.cam_active = 1,
		.base_addr = 1,
		.pan_address = 0,
		.tilt_address = 2,
		.speed_address = 4,
		.switch_address = 7,
		.pan_invert = 0,
		.tilt_invert=0,
		.pan_scaling = 30,
		.tilt_scaling = 20,
		.pan = 0, 
		.tilt = 0,
		.speed = 0,
		.store_pan = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
		.store_tilt= { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
		.button_state = 0,
		.power_state = 1
	},	
	{
		.cam_active = 1,
		.base_addr = 17,
		.pan_address = 0,
		.tilt_address = 2,
		.speed_address = 4,
		.switch_address = 7,
		.pan_invert = 0,
		.tilt_invert=0,
		.pan_scaling = 30,
		.tilt_scaling = 20,
		.pan = 0, 
		.tilt = 0,
		.speed = 0,
		.store_pan = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
		.store_tilt= { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
		.button_state = 0,
		.power_state = 1 
	},
	{
		.cam_active = 1,
		.base_addr = 33,
		.pan_address = 0,
		.tilt_address = 2,
		.speed_address = 4,
		.switch_address = 7,
		.pan_invert = 0,
		.tilt_invert=0,
		.pan_scaling = 30,
		.tilt_scaling = 20,
		.pan = 0, 
		.tilt = 0,
		.speed = 0,
		.store_pan = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
		.store_tilt= { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
		.button_state = 0,
		.power_state = 1 
	},
	{
        .cam_active = 1,
		.base_addr = 0xFFFF,
		.pan_address = 0xFFFF,
		.tilt_address = 0xFFFF,
		.speed_address = 0xFFFF,
		.switch_address = 0xFFFF,
		.pan_invert = 0xFF,
		.tilt_invert=0xFF,
		.pan_scaling = 0xFFFF,
		.tilt_scaling = 0xFFFF,
		.pan = 0xFF, 
		.tilt = 0xFF,
		.speed = 0xFF,
		.store_pan = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
		.store_tilt= { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
		.button_state = 0,
        .power_state = 1,
        .focus = 500,
        .iris=0
	} 
};

cam_data_t backup_cams[CAM_COUNT] EEMEM; //store CAM Data in eeprom
uint8_t menu_lock_active EEMEM;
//Hauptprogramm
int main (void) 
{  
	//initialise stuff
	hardware_init();
	eeprom_read_block (cams, backup_cams, sizeof(cams));
	lock_load();

	lcd_init(LCD_DISP_ON);

	uint8_t lock_active=eeprom_read_byte(&menu_lock_active);
	
	//if the system was locked during last shutdown and camkey 1 is not pressed lock the system
	if(lock_active==1 && !get_cam1_key())
	{
		set_menu(MENU_LOCKED);
	}
	else
	{
		if(lock_active==255 || get_cam1_key())
			unlock_system();
		set_menu(MENU_SPLASH);
	}

	dmx_init();
    cccb_init();
	ADC_Init();
	rotary_init();
//
	send_switch();

	set_cam_leds(active_cam);

	for(uint8_t i=0; i < CAM_COUNT; i++)
	{
		if(cams[i].cam_active)
		{
			write_dmx(cams[i].base_addr-1 + cams[i].pan_address, cams[i].pan);
			write_dmx(cams[i].base_addr-1 + cams[i].tilt_address, cams[i].tilt);
			write_dmx(cams[i].base_addr-1 + cams[i].speed_address, cams[i].speed);
			send_switch_state(i);
		}
	}


	uint16_t blink_counter=0;
	uint8_t loop=0;

	while(1)
	{
		blink_counter++;
		loop++;

		if(loop==10)
		{  
			blink_counter++;
			if(active_menu != MENU_LOCKED)
			{
				//Process analog inputs, cam changes and store requests
				process_inputs();
				rotary_process();
			}

			//Process the menu system  / softkeys
			process_menu();
		

			//The STORE-LEDs should flash if MENU_STORE or MENU_CLEAR is selected
			if((active_menu == MENU_STORE || active_menu == MENU_CLEAR) && blink_counter % 16 == 0)
			{
				for(int i=0; i < STORE_COUNT; i++)
					reset_store_led(i);
			}

			if((active_menu == MENU_STORE || active_menu == MENU_CLEAR )&& blink_counter % 32 == 0)
			{
				for(int i=0; i < STORE_COUNT; i++)
					set_store_led(i);
			}

			
			loop=0;
              cccb_run();
		}
		


		hardware_tick();
		rotary_tick();

		//awkward delay here.. should be replaced by a timer. But it works, DMX is interrupt-driven
		_delay_ms(0.5);
	}
}

void update_leds(void)
{
	//update the store leds if the leds are not flashing
	if( ! (active_menu == MENU_STORE || active_menu == MENU_CLEAR))
	{
		for(int i=0; i < STORE_COUNT; i++)
		{
			//0xFFFF marks an empty store
			if(cams[active_cam].store_pan[i]!= 0xFFFF && cams[active_cam].store_tilt[i]!= 0xFFFF)
			{
				set_store_led(i);
			}
			else
			{
				reset_store_led(i);
			}
		}
	}
}


void process_inputs(void)
{
	//Fetch keypress from hardware
	camkey_t keys = get_camkeys();



    //Change the activ  e cam
	switch(keys)
	{
		case CAM1: active_cam = cams[0].cam_active ? 0 : active_cam; break;
		case CAM2: active_cam = cams[1].cam_active ? 1 : active_cam; break; 
		case CAM3: active_cam = cams[2].cam_active ? 2 : active_cam; break;
		case CAM4: active_cam = cams[3].cam_active ? 3 : active_cam; break;
		default: break;
	}

	//update stuff if the cam changed
	if(keys != CAM_NO_KEY)
	{
		//camera leds
		set_cam_leds(active_cam);

		update_leds();


        active_rotary_funtion=0;
        set_fsel_leds(0 );
		

		//main screen
		if(active_menu == MENU_MAIN)
			main_init();

	}

	static uint8_t analog=0;
	
	uint8_t old_tilt = cams[active_cam].tilt;
	uint8_t old_pan = cams[active_cam].pan;
	
	analog++;
	if(analog%8==0)
	{
		//fetch joystick deflection and calculate new pan
		int16_t diff = axis_offset(0, cams[active_cam].pan_scaling);
		if(cams[active_cam].pan_invert)
		{
			diff=-diff;
		}
	
		if( (int16_t)cams[active_cam].pan+diff > 255)
			cams[active_cam].pan=255;
		else if( (int16_t)cams[active_cam].pan+diff < 0)
			cams[active_cam].pan=0;
		else
			cams[active_cam].pan+=diff	;

		//fetch joystick deflection and calculate new tilt
		diff = -axis_offset(1, cams[active_cam].tilt_scaling);
		if(cams[active_cam].tilt_invert)
		{
			diff=-diff;
		}


		if( (int16_t)cams[active_cam].tilt+diff > 255)
			cams[active_cam].tilt=255;
		else if( (int16_t)cams[active_cam].tilt+diff < 0)
			cams[active_cam].tilt=0;
		else
			cams[active_cam].tilt+=diff	;
	}

	//fetch store key selection from hardware
	storekey_t store = get_storekeys();

    if(store != STORE_NO_KEY)
	{
		uint8_t store_id=0;

		//get store id from button
		switch(store)
		{
			case STORE1: store_id = 0; break;
			case STORE2: store_id = 1; break;
			case STORE3: store_id = 2; break;
			case STORE4: store_id = 3; break;
			default: break;
		}

		//if the current menu is store, save the data, else read or clear
		if(active_menu == MENU_STORE)
		{
			cams[active_cam].store_pan[store_id] = cams[active_cam].pan;
			cams[active_cam].store_tilt[store_id] = cams[active_cam].tilt;
			save_data(); //write data to EEPROM
			set_menu(MENU_MAIN); //jump back to main menu
		}
		else if(active_menu == MENU_CLEAR) //if the current menu is clear, clear the selected slot
		{
			cams[active_cam].store_pan[store_id]=0xFFFF;
			cams[active_cam].store_tilt[store_id]=0xFFFF;
			save_data(); //write data to EEPROM
			set_menu(MENU_MAIN); //jump back to main menu
		}
		else //load stored values
		{
			//can not load empty store
			if(cams[active_cam].store_pan[store_id]!= 0xFFFF &&  cams[active_cam].store_tilt[store_id] != 0xFFFF)
			{
				cams[active_cam].pan = cams[active_cam].store_pan[store_id]; 
				cams[active_cam].tilt = cams[active_cam].store_tilt[store_id];;
			}
		}

	}

	//if tilt or pan changed
	
	//write values to DMX
	write_dmx(cams[active_cam].base_addr-1 + cams[active_cam].pan_address, cams[active_cam].pan);
	write_dmx(cams[active_cam].base_addr-1 + cams[active_cam].tilt_address, cams[active_cam].tilt);
	write_dmx(cams[active_cam].base_addr-1 + cams[active_cam].speed_address, cams[active_cam].speed);

	//update main menu
	if(old_tilt != cams[active_cam].tilt || old_pan != cams[active_cam].pan)
	{
		if(active_menu == MENU_MAIN)
			main_show();
	}

    fselkey_t fsel = get_fselkeys();

    if(fsel != FSEL_NO_KEY)
	{
        if(active_cam!=3)
        {
            set_fsel_leds(0);
            active_rotary_funtion=0;
        }
        else
        {
            switch(fsel)
            {
            case FSEL1: active_rotary_funtion=0; break;
            case FSEL2: active_rotary_funtion=1; break;
            case FSEL3: active_rotary_funtion=2; break;
            case FSEL4: active_rotary_funtion=3; break;
            }

            set_fsel_leds(fsel);
        }

        if(active_menu==MENU_MAIN)
            main_init();

	}
}

void store_clear(void)
{
	for(uint8_t i=0; i < STORE_COUNT; i++)
	{
		cams[active_cam].store_pan[i] = 0xFFFF;
		cams[active_cam].store_tilt[i] = 0xFFFF;
	}
}

void save_data(void)
{
	eeprom_write_block (cams, backup_cams, sizeof(cams));
	lock_save();
}

void load_default(void)
{

	memcpy_P(cams,cams_default, sizeof(cams)); 
	save_data();
}

void lock_system(void)
{
	eeprom_write_byte(&menu_lock_active, 1);
}

void unlock_system(void)
{
	eeprom_write_byte(&menu_lock_active, 0);
}

void send_switch_state(uint8_t cam)
{
	uint8_t dmx;

	if(!cams[cam].button_state && !cams[cam].power_state)
		dmx=5;
	else if(!cams[cam].button_state && cams[cam].power_state)
		dmx=64;
	else if(cams[cam].button_state && !cams[cam].power_state)
		dmx=128;
	else if(cams[cam].button_state && cams[cam].power_state)
		dmx=255;


	write_dmx(cams[cam].base_addr-1 + cams[cam].switch_address, dmx);
}

