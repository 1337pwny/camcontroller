#include "menu_system.h"
#include "menu_main.h"
#include "menu_lock.h"
#include "menu_settings_general.h"
#include "menu_cam_config.h"
#include "menu_cam_ctrl.h"
#include "menu_all_power.h"
#include "menu_hardware.h"
#include "menu_bmmcc.h"
#include "lcd.h"
#include "hardware.h"


extern void store_clear(void);
extern void save_data(void);
extern void load_default(void);
extern void update_leds(void);
extern void lock_system(void); 

static const char MENU_SPLASH_L1[] PROGMEM			= "StreamingFurs       ";
static const char MENU_SPLASH_L2[] PROGMEM			= "CaracalCamController";
static const char MENU_SPLASH_L3[] PROGMEM			= "By Karrn            ";
static const char MENU_SPLASH_L4[] PROGMEM			= "                2018";

static const char MENU_MAIN_L1[] PROGMEM			= "                    ";
static const char MENU_MAIN_L2[] PROGMEM			= "                    ";
static const char MENU_MAIN_L3[] PROGMEM			= "                    ";
static const char MENU_MAIN_L4[] PROGMEM			= "STORE           MENU";

static const char MENU_SETUP_L1[] PROGMEM			= "Select cam          ";
static const char MENU_SETUP_L2[] PROGMEM			= "                    ";
static const char MENU_SETUP_L3[] PROGMEM			= "                    ";
static const char MENU_SETUP_L4[] PROGMEM			= "PREV NEXT ENTER BACK";

static const char MENU_EDIT_CAM_L1[] PROGMEM		= "Enter parameter     ";
static const char MENU_EDIT_CAM_L2[] PROGMEM		= "                    ";
static const char MENU_EDIT_CAM_L3[] PROGMEM		= "                    ";
static const char MENU_EDIT_CAM_L4[] PROGMEM		= "DOWN UP   NEXT  BACK";

static const char MENU_STORE_L1[] PROGMEM			= "Store               ";
static const char MENU_STORE_L2[] PROGMEM			= "Choose store to save";
static const char MENU_STORE_L3[] PROGMEM			= "                    ";
static const char MENU_STORE_L4[] PROGMEM			= "     CLEAR     ABORT";

static const char MENU_CLEAR_L1[] PROGMEM			= "Clear store         ";
static const char MENU_CLEAR_L2[] PROGMEM			= "Choose store        "; 
static const char MENU_CLEAR_L3[] PROGMEM			= "                    ";
static const char MENU_CLEAR_L4[] PROGMEM			= "ALL            ABORT";

static const char MENU_GENERAL_SETUP_L1[] PROGMEM	= "Choose menu option  ";
static const char MENU_GENERAL_SETUP_L2[] PROGMEM	= "                    ";
static const char MENU_GENERAL_SETUP_L3[] PROGMEM	= "                    ";
static const char MENU_GENERAL_SETUP_L4[] PROGMEM	= "PREV NEXT ENTER BACK";

static const char MENU_RESET_L1[] PROGMEM			= "Setup               ";
static const char MENU_RESET_L2[] PROGMEM			= "Load default config ";
static const char MENU_RESET_L3[] PROGMEM			= "and erase all data? ";
static const char MENU_RESET_L4[] PROGMEM			= "YES             BACK";

static const char MENU_CTRL_L1[] PROGMEM			= "Setup               ";
static const char MENU_CTRL_L2[] PROGMEM			= "                    ";
static const char MENU_CTRL_L3[] PROGMEM			= "                    ";
static const char MENU_CTRL_L4[] PROGMEM			= "PREV NEXT CTRL  BACK";

static const char MENU_CTRL_EDIT_L1[] PROGMEM		= "Setup               ";
static const char MENU_CTRL_EDIT_L2[] PROGMEM		= "                    ";
static const char MENU_CTRL_EDIT_L3[] PROGMEM		= "  Power             ";
static const char MENU_CTRL_EDIT_L4[] PROGMEM		= " ON  OFF   BTN  BACK";

static const char MENU_LOCKED_L1[] PROGMEM			= "    DragonVideo     ";
static const char MENU_LOCKED_L2[] PROGMEM			= "  **** LOCKED ****  ";
static const char MENU_LOCKED_L3[] PROGMEM			= "                    ";
static const char MENU_LOCKED_L4[] PROGMEM			= " 1    2     3     4 ";

static const char MENU_LOCK_SETUP_L1[] PROGMEM		= "Setup               ";
static const char MENU_LOCK_SETUP_L2[] PROGMEM		= "Enter new code:     ";
static const char MENU_LOCK_SETUP_L3[] PROGMEM		= "                    ";
static const char MENU_LOCK_SETUP_L4[] PROGMEM		= " 1    2     3     4 ";

static const char MENU_ALL_POWER_L1[] PROGMEM 		= "Setup               ";
static const char MENU_ALL_POWER_L2[] PROGMEM 		= "                    ";
static const char MENU_ALL_POWER_L3[] PROGMEM 		= "Switch all cams     ";
static const char MENU_ALL_POWER_L4[] PROGMEM 		= "ON   OFF        BACK";

static const char MENU_HARDWARE_L1[] PROGMEM 		= "                    ";
static const char MENU_HARDWARE_L2[] PROGMEM 		= "                    ";
static const char MENU_HARDWARE_L3[] PROGMEM 		= "                    ";
static const char MENU_HARDWARE_L4[] PROGMEM 		= "                BACK";

static const char MENU_LED_BRIGHTNESS_L1[] PROGMEM  = "Setup               ";
static const char MENU_LED_BRIGHTNESS_L2[] PROGMEM  = "                    ";
static const char MENU_LED_BRIGHTNESS_L3[] PROGMEM  = "                    ";
static const char MENU_LED_BRIGHTNESS_L4[] PROGMEM  = "UP  DOWN        BACK";


static const char MENU_BMMCC_L1[] PROGMEM  = "BMMCC setup         ";
static const char MENU_BMMCC_L2[] PROGMEM  = "                    ";
static const char MENU_BMMCC_L3[] PROGMEM  = "                    ";
static const char MENU_BMMCC_L4[] PROGMEM  = "DOWN UP   NEXT  BACK";

//Define the menu structure
__flash const menu_t menues[] =
{ 	
	{ //MENU_SPLASH
		.lines = { MENU_SPLASH_L1,MENU_SPLASH_L2,MENU_SPLASH_L3,MENU_SPLASH_L4},
		.next =  { MENU_MAIN,MENU_MAIN,MENU_MAIN,MENU_MAIN, MENU_MAIN},
		.cb =    { NULL,NULL,NULL,NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init =  NULL,
		.cyclic = NULL,
		.rotary= NULL
    },
	{ //MENU_MAIN
		.lines = { MENU_MAIN_L1,MENU_MAIN_L2,MENU_MAIN_L3,MENU_MAIN_L4},
		.next  = { MENU_STORE, MENU_INVALID,MENU_INVALID,MENU_GENERAL_SETUP,MENU_INVALID},
		.cb    = { NULL,NULL,NULL,NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = main_init,
        .cyclic = main_run,
		.rotary= NULL //the main menu uses the rotary encoder, but it's initialised in the main_init
	},
	{ //MENU_SETUP
		.lines = { MENU_SETUP_L1,MENU_SETUP_L2,MENU_SETUP_L3,MENU_SETUP_L4},
		.next  = { MENU_INVALID,MENU_INVALID,MENU_EDIT_CAM,MENU_GENERAL_SETUP,MENU_GENERAL_SETUP},
		.cb    = { setup_cam_down,setup_cam_up,param_resetId,NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init =  setup_show_cam,
		.cyclic = NULL,
		.rotary= NULL 
	},
	{ //MENU_EDIT_CAM
		.lines = { MENU_EDIT_CAM_L1,MENU_EDIT_CAM_L2,MENU_EDIT_CAM_L3,MENU_EDIT_CAM_L4},
		.next  = { MENU_INVALID,MENU_INVALID,MENU_INVALID,MENU_SETUP,MENU_SETUP},
		.cb    = { param_down,param_up, param_next, NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = param_show,
		.cyclic = NULL,
		.rotary= NULL //the cam edit menu uses the rotary encoder, but it's initialised in parm_show
	},
	{ //MENU_STORE
		.lines = { MENU_STORE_L1,MENU_STORE_L2,MENU_STORE_L3,MENU_STORE_L4},
		.next  = { MENU_INVALID,MENU_CLEAR,MENU_INVALID,MENU_MAIN,MENU_INVALID},
		.cb    = { NULL,NULL, NULL, NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = NULL,
		.cyclic = NULL,
		.rotary= NULL
	},
	{ //MENU_CLEAR
		.lines = { MENU_CLEAR_L1,MENU_CLEAR_L2,MENU_CLEAR_L3,MENU_CLEAR_L4},
		.next  = { MENU_MAIN,MENU_INVALID,MENU_INVALID,MENU_MAIN,MENU_INVALID},
		.cb    = { store_clear,NULL, NULL, NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = NULL,
		.cyclic = NULL,
		.rotary= NULL
	}, 
	{ //MENU_GENERAL_SETUP
		.lines = { MENU_GENERAL_SETUP_L1,MENU_GENERAL_SETUP_L2,MENU_GENERAL_SETUP_L3,MENU_GENERAL_SETUP_L4},
		.next  = { MENU_INVALID,MENU_INVALID,MENU_INVALID,MENU_MAIN,MENU_INVALID},
		.cb    = { setup_menu_prev,setup_menu_next, setup_menu_enter, save_data,setup_menu_enter},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = setup_reset,
		.cyclic = NULL,
		.rotary= &rotary_menu_settings_general
	}, 
	{ //MENU_RESET
		.lines = { MENU_RESET_L1,MENU_RESET_L2,MENU_RESET_L3,MENU_RESET_L4},
		.next  = { MENU_MAIN,MENU_INVALID,MENU_INVALID,MENU_MAIN,MENU_INVALID},
		.cb    = { load_default,NULL, NULL, NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = NULL,
		.cyclic = NULL,
		.rotary= NULL
	},  
	{ //MENU_CTRL
		.lines = { MENU_CTRL_L1,MENU_CTRL_L2,MENU_CTRL_L3,MENU_CTRL_L4},
		.next  = { MENU_INVALID,MENU_INVALID,MENU_CTRL_EDIT,MENU_GENERAL_SETUP,MENU_CTRL_EDIT},
		.cb    = { ctrl_cam_down,ctrl_cam_up, NULL, NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = ctrl_cam_show,
		.cyclic = NULL,
		.rotary= &rotary_menu_ctrl
	},  
	{ //MENU_CTRL_EDIT
		.lines = { MENU_CTRL_EDIT_L1,MENU_CTRL_EDIT_L2,MENU_CTRL_EDIT_L3,MENU_CTRL_EDIT_L4},
		.next  = { MENU_INVALID,MENU_INVALID,MENU_CTRL_EDIT,MENU_CTRL,MENU_INVALID},
		.cb    = { cam_power_on,cam_power_off, cam_button_press, NULL,NULL},
		.cb_r=   { NULL,NULL,cam_button_release,NULL,NULL},
		.init  = cam_power_show,
		.cyclic = NULL,
		.rotary= NULL
	} ,  
	{ //MENU_LOCKED
		.lines = { MENU_LOCKED_L1,MENU_LOCKED_L2,MENU_LOCKED_L3,MENU_LOCKED_L4},
		.next  = { MENU_INVALID,MENU_INVALID,MENU_INVALID,MENU_INVALID,MENU_INVALID},
		.cb    = { lock_1_pressed,lock_2_pressed, lock_3_pressed, lock_4_pressed,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = lock_system,
		.cyclic = NULL,
		.rotary= NULL
	} ,  
	{ //MENU_LOCK_SETUP
		.lines = { MENU_LOCK_SETUP_L1,MENU_LOCK_SETUP_L2,MENU_LOCK_SETUP_L3,MENU_LOCK_SETUP_L4},
		.next  = { MENU_INVALID,MENU_INVALID,MENU_INVALID,MENU_INVALID,MENU_INVALID},
		.cb    = {lock_1_pressed,lock_2_pressed, lock_3_pressed, lock_4_pressed,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = NULL,
		.cyclic = NULL,
		.rotary= NULL
	} ,  
	{ //MENU_ALL_POWER
		.lines = { MENU_ALL_POWER_L1,MENU_ALL_POWER_L2,MENU_ALL_POWER_L3,MENU_ALL_POWER_L4},
		.next  = { MENU_INVALID,MENU_INVALID,MENU_INVALID,MENU_GENERAL_SETUP,MENU_INVALID},
		.cb    = {switch_cams_on,switch_cams_off, NULL, NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = NULL,
		.cyclic = NULL,
		.rotary= NULL
	} ,
	{ //MENU_HARDWARE
		.lines = { MENU_HARDWARE_L1,MENU_HARDWARE_L2,MENU_HARDWARE_L3,MENU_HARDWARE_L4},
		.next  = { MENU_INVALID,MENU_INVALID,MENU_INVALID,MENU_GENERAL_SETUP,MENU_INVALID},
		.cb    = { NULL,NULL,NULL,NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = NULL,
		.cyclic = show_hardware_info,
		.rotary= NULL
	} ,
	{ //MENU_LED_BRIGHTNESS
		.lines = { MENU_LED_BRIGHTNESS_L1,MENU_LED_BRIGHTNESS_L2,MENU_LED_BRIGHTNESS_L3,MENU_LED_BRIGHTNESS_L4},
		.next  = { MENU_INVALID,MENU_INVALID,MENU_INVALID,MENU_GENERAL_SETUP,MENU_INVALID},
		.cb    = { NULL,NULL,NULL,NULL,NULL},
		.cb_r=   { NULL,NULL,NULL,NULL,NULL},
		.init  = NULL,
		.cyclic = NULL,
		.rotary= NULL
	}  ,
    { //MENU_BMMCC
        .lines = { MENU_BMMCC_L1,MENU_BMMCC_L2,MENU_BMMCC_L3,MENU_BMMCC_L4},
        .next  = { MENU_INVALID,MENU_INVALID,MENU_INVALID,MENU_GENERAL_SETUP,MENU_INVALID},
        .cb    = { menu_bmmcc_down,menu_bmmcc_up,menu_bmmcc_next,NULL,NULL},
        .cb_r=   { NULL,NULL,NULL,NULL,NULL},
        .init  = menu_bmmcc_init,
        .cyclic = NULL,
        .rotary= NULL
    }
};

menu_identifiers active_menu;

void process_menu(void)
{
	softkey_t button = get_softkeys();

	if(button != NO_KEY )
	{
		menu_identifiers old;
		switch(button)
		{
			case SK1: 
			case SK2: 
			case SK3:
			case SK4: 
			case ROT:
				old = active_menu;

				if(menues[active_menu].cb[button] != NULL)
						menues[active_menu].cb[button]();	

				if(active_menu == old && menues[active_menu].next[button] != MENU_INVALID)
					set_menu(menues[active_menu].next[button]);
			break;
			default:
				if(menues[active_menu].cb_r[button-5] != NULL)
						menues[active_menu].cb_r[button-5]();	

			break;
					
		}
	}	

	if(menues[active_menu].cyclic)
		menues[active_menu].cyclic();
}

void set_menu(menu_identifiers menu)
{
	active_menu = menu;
	lcd_clrscr();
	lcd_puts_p(menues[menu].lines[0]);
	lcd_gotoxy(0,1);
	lcd_puts_p(menues[menu].lines[1]);
	lcd_gotoxy(0,2);
	lcd_puts_p(menues[menu].lines[2]);
	lcd_gotoxy(0,3);
	lcd_puts_p(menues[menu].lines[3]);

	//NULL is also valid here, so no NULL check (disable rotary)
	rotary_setconfig(menues[menu].rotary);

	if(menues[menu].init)
		menues[menu].init();

	update_leds();

}





