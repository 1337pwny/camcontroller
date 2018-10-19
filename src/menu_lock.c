#include "menu_lock.h"
#include "menu_system.h"
#include "lcd.h"
#include <stdint.h>
#include <avr/eeprom.h>


uint8_t code[] = {1,1,1,1};
uint8_t code_eeprom[4] EEMEM;

extern void lock_system(void);    //Functions from main
extern void unlock_system(void);  //Functions from main
extern void save_data(void);
static void lock_pressed(uint8_t n);

void lock_load(void)
{
	eeprom_read_block (code, code_eeprom, sizeof(code));
}

void lock_save(void)
{
	eeprom_write_block (code, code_eeprom, sizeof(code));
}

void lock_1_pressed(void)
{
	lock_pressed(1);
}

void lock_2_pressed(void)
{
	lock_pressed(2);
}

void lock_3_pressed(void)
{
	lock_pressed(3);
}

void lock_4_pressed(void)
{
	lock_pressed(4);
}

static void lock_pressed(uint8_t n)
{
	static uint8_t index=0;
	static uint8_t numbers[4];

	lcd_gotoxy(8+index,2);

	if(active_menu == MENU_LOCK_SETUP)
		lcd_putc('0'+n);
	else
		lcd_putc('*');

	numbers[index]=n;
	index++;
	
	if(index==4)
	{
		index=0;
	
		if(active_menu == MENU_LOCK_SETUP)
		{
			for(uint8_t i=0; i < 4; i++)
			{
				code[i]=numbers[i];
				set_menu(MENU_GENERAL_SETUP);
				save_data();
			}
		}
		else
		{
			uint8_t error=0;
			for(uint8_t i=0; i < 4; i++)
			{
				if(code[i]!=numbers[i])
					error=1;

			}

			if(error==0)
			{
				unlock_system();
				set_menu(MENU_MAIN);
			}
			else
			{
				lcd_gotoxy(8,2);
				lcd_puts("    ");
			}
		}
	}
}
