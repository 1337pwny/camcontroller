#include "menu_main.h"
#include "globals.h"
#include "rotary.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>

static uint8_t zoom_tmp;

void main_init(void)
{
	static rotary_config_t rotary_speed_config = 
		{
			.data_u8 = &cams[0].speed,
			.type = rotary_uint8,
			.min = 0,
			.max = 255,
			.change = main_show,
			.multi = 5,
			.wrap = 0,
			.leds_on = 1
		};

    static rotary_config_t rotary_focus_config =
        {
            .data_u16 = &cams[0].focus,
            .type = rotary_uint16,
            .min = 190,
            .max = 1800,
            .change = main_show,
            .multi = 5,
            .wrap = 0,
            .leds_on = 1
        };

    static rotary_config_t rotary_iris_config =
        {
            .data_u8 = &cams[0].iris,
            .type = rotary_uint8,
            .min = 0,
            .max = 22,
            .change = main_show,
            .multi = 1,
            .wrap = 0,
            .leds_on = 1
        };

    static rotary_config_t rotary_zoom_config =
        {
            .data_u8 = &zoom_tmp,
            .type = rotary_uint8,
            .min = 0,
            .max = 255,
            .change = NULL,
            .multi = 1,
            .wrap = 0,
            .leds_on = 0
        };





    switch(active_rotary_funtion)
    {
    case 0: rotary_speed_config.data_u8 = &cams[active_cam].speed;
            rotary_setconfig(&rotary_speed_config); break;
    case 1: rotary_focus_config.data_u16 = &cams[active_cam].focus;
            rotary_setconfig(&rotary_focus_config); break;
    case 2: rotary_setconfig(&rotary_zoom_config); break;
    case 3: rotary_iris_config.data_u8 = &cams[active_cam].iris;
            rotary_setconfig(&rotary_iris_config); break;
    }





	main_show();
}

void main_show(void)
{
	lcd_gotoxy(0,0);
	lcd_puts("Active: CAM ");
	char tmp[21];
	itoa(active_cam+1,tmp,10);
	lcd_gotoxy(12,0);
	lcd_puts(tmp);

	lcd_gotoxy(0,1);
	sprintf(tmp,"Pan: %3d Tilt: %3d", cams[active_cam].pan, cams[active_cam].tilt);
	lcd_puts(tmp);

    if(active_rotary_funtion==0)
    {
        lcd_gotoxy(0,2);
        sprintf(tmp,"Speed: %3d ", cams[active_cam].speed);
        lcd_puts(tmp);
    }
    else if(active_rotary_funtion==1)
    {
        lcd_gotoxy(0,2);
        sprintf(tmp,"Focus: %4d", cams[active_cam].focus);
        lcd_puts(tmp);
    }
    else if(active_rotary_funtion==2)
    {
        lcd_gotoxy(0,2);
        sprintf(tmp,"Zoom       ");
        lcd_puts(tmp);
    }
    else if(active_rotary_funtion==3)
    {
        lcd_gotoxy(0,2);
        sprintf(tmp,"Iris:       ");
        lcd_puts(tmp);
        lcd_gotoxy(5,2);
        switch(cams[active_cam].iris)
        {
        case 0: lcd_puts("f3.5"); break;
        case 1: lcd_puts("f3.7"); break;
        case 2: lcd_puts("f4"); break;
        case 3: lcd_puts("f4.5"); break;
        case 4: lcd_puts("f4.8"); break;
        case 5: lcd_puts("f5.2"); break;
        case 6: lcd_puts("f5.6"); break;
        case 7: lcd_puts("f6.2"); break;
        case 8: lcd_puts("f6.7"); break;
        case 9: lcd_puts("f7.3"); break;
        case 10: lcd_puts("f8"); break;
        case 11: lcd_puts("f8.7"); break;
        case 12: lcd_puts("f9.5"); break;
        case 13: lcd_puts("f10"); break;
        case 14: lcd_puts("f11"); break;
        case 15: lcd_puts("f12"); break;
        case 16: lcd_puts("f14"); break;
        case 17: lcd_puts("f15"); break;
        case 18: lcd_puts("f16"); break;
        case 19: lcd_puts("f17"); break;
        case 20: lcd_puts("f19"); break;
        case 21: lcd_puts("f21"); break;
        case 22: lcd_puts("f22"); break;
        default: sprintf(tmp,"f%d",cams[active_cam].iris); lcd_puts(tmp); break;

        }

    }


}	

void main_run(void)
{
    static uint8_t i;

    if(i==20)
     {
    char tmp[20];
    if(active_rotary_funtion==2)
    {

        cams[active_cam].zoom = (int8_t)((int8_t)zoom_tmp-(int8_t)127)*8;

        lcd_gotoxy(5,2);
        sprintf(tmp,"%d     ",cams[active_cam].zoom);
        lcd_puts(tmp);
    }

    zoom_tmp=127;
    i=0;
    }

    i++;
}
