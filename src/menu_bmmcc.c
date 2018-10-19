#include "menu_bmmcc.h"
#include "lcd.h"
#include "cccb.h"
#include <stdint.h>

typedef struct {
    char* description;
    int8_t* dest;

} bmmcc_setting_t;

bmmcc_setting_t bmmcc_settings[] = {
    {
        .description = "Shutter angle       ",
        .dest = &bmmcc_request_sa
    },
    {
        .description = "White balance       ",
        .dest = &bmmcc_request_wb
    },
    {
        .description = "ISO                 ",
        .dest = &bmmcc_request_iso
    }
};

#define NUMBER_OF_BMMCC_PARAMETERS  (sizeof(bmmcc_settings)/sizeof(bmmcc_setting_t))
static uint8_t active_parameter=0;


void menu_bmmcc_init(void)
{
    menu_bmmcc_show();
}

void menu_bmmcc_show(void)
{
    lcd_gotoxy(0,1);
    lcd_puts(bmmcc_settings[active_parameter].description);
}

void menu_bmmcc_next()
{
    active_parameter++;
    if(active_parameter >= NUMBER_OF_BMMCC_PARAMETERS)
        active_parameter=0;

    menu_bmmcc_show();
}

void menu_bmmcc_down(void)
{
    *bmmcc_settings[active_parameter].dest=-1;
}

void menu_bmmcc_up(void)
{
    *bmmcc_settings[active_parameter].dest=1;

}
