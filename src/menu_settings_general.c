#include "menu_settings_general.h"
#include "menu_system.h"
#include "lcd.h"
uint8_t current_menu=0;

typedef struct {
	menu_identifiers menu;
	char* description;

} setting_menu_t;

setting_menu_t setting_menues[] = {
	{
		.menu = MENU_SETUP,
		.description = "Camera setup        "
	},
	{
		.menu = MENU_CTRL,
		.description = "Camera controlling  "
	},
	{
		.menu = MENU_ALL_POWER,
		.description = "General camera power"
	},
	{
		.menu = MENU_LOCKED,
		.description = "Lock cam controller "
	},
	{
		.menu = MENU_LOCK_SETUP,
		.description = "Enter lock code     "
	},
	{
		.menu = MENU_HARDWARE,
		.description = "Show hardware info  "
	},
	{
		.menu = MENU_RESET,
		.description = "Reset settings      "
    },
    {
        .menu = MENU_BMMCC,
        .description = "BMMCC settings      "
    }
};


rotary_config_t rotary_menu_settings_general = {
	.data_u8 = &current_menu,
	.type = rotary_uint8,
	.min = 0,
	.max = (sizeof(setting_menues)/sizeof(setting_menu_t))-1,
	.change = setup_menu_show,
	.multi = 1,
	.wrap = 1,
	.leds_on = 0
};

void setup_menu_next(void)
{
	current_menu++;

	if(current_menu >= (sizeof(setting_menues)/sizeof(setting_menu_t)))
	{
		current_menu=0;
	}

	setup_menu_show();
}

void setup_menu_prev(void)
{
	current_menu--;

	if(current_menu >=(sizeof(setting_menues)/sizeof(setting_menu_t)))
	{
		current_menu=(sizeof(setting_menues)/sizeof(setting_menu_t))-1;
	}

	setup_menu_show();
}


void setup_menu_enter(void)
{
	set_menu(setting_menues[current_menu].menu);
}

void setup_menu_show(void)
{
	lcd_gotoxy(0,1);
	lcd_puts(setting_menues[current_menu].description);
}

void setup_reset(void)
{
	current_menu=0;
	setup_menu_show();
}
