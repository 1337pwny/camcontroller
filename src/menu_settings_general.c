#include "menu_settings_general.h"
#include "menu_system.h"

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
		.menu = MENU_LOCKED,
		.description = "Lock cam controller "
	},
	{
		.menu = MENU_LOCK_SETUP,
		.description = "Enter lock code     "
	},
	{
		.menu = MENU_RESET,
		.description = "Reset settings      "
	}
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