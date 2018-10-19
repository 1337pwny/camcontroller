#include "menu_cam_config.h"
#include "globals.h"
#include "lcd.h"
#include "rotary.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>


uint8_t setup_active_cam=0;
uint8_t param_id=0;


typedef enum {
	type_uint16,
	type_uint8
} parameter_type;

typedef struct {
	char* name;
	parameter_type type;
	size_t offset;
	uint16_t min;
	uint16_t max;
} cam_setup_parameters_t;


#define NUMBER_OF_CAM_PARAMETERS  (sizeof(cam_setup_parameters)/sizeof(cam_setup_parameters_t))
const __flash cam_setup_parameters_t cam_setup_parameters[] = 
{
	{
		.name = "Camera active       ",
		.type = type_uint8,
		.offset = offsetof(cam_data_t, cam_active),
		.min = 0,
		.max = 1
	},
	{
		.name = "base_addr           ",
		.type = type_uint16,
		.offset = offsetof(cam_data_t, base_addr),
		.min = 1,
		.max = 512
	},
	{
		.name = "pan address         ",
		.type = type_uint16,
		.offset = offsetof(cam_data_t, pan_address),
		.min = 0,
		.max = 512
	},
	{
		.name = "tilt address       ",
		.type = type_uint16,
		.offset = offsetof(cam_data_t, tilt_address),
		.min = 0,
		.max = 512
	},
	{
		.name = "speed address       ",
		.type = type_uint16,
		.offset = offsetof(cam_data_t, speed_address),
		.min = 0,
		.max = 512
	},
	{
		.name = "switch address      ",
		.type = type_uint16,
		.offset = offsetof(cam_data_t, switch_address),
		.min = 0,
		.max = 512
	},
	{
		.name = "invert pan          ",
		.type = type_uint8,
		.offset = offsetof(cam_data_t, pan_invert),
		.min = 0,
		.max = 1
	},
	{
		.name = "invert tilt         ",
		.type = type_uint8,
		.offset = offsetof(cam_data_t, tilt_invert),
		.min = 0,
		.max = 1
	},
	{
		.name = "pan scaling         ",
		.type = type_uint16,
		.offset = offsetof(cam_data_t, pan_scaling),
		.min = 0,
		.max = 100
	},
	{
		.name = "tilt scaling        ",
		.type = type_uint16,
		.offset = offsetof(cam_data_t, tilt_scaling),
		.min = 0,
		.max = 100
	}
};


void setup_show_cam(void)
{
	lcd_gotoxy(0,1);

	char tmp[10];
	itoa(setup_active_cam+1,tmp,10);
	lcd_puts("Edit Cam ");
	lcd_gotoxy(9,1);
	lcd_puts(tmp);
}

void setup_cam_up(void)
{
	setup_active_cam++;

	if(setup_active_cam>= CAM_COUNT)
		setup_active_cam=0;

	setup_show_cam();
}

void setup_cam_down(void)
{
	setup_active_cam--;
	if(setup_active_cam>= CAM_COUNT)
		setup_active_cam= CAM_COUNT-1;

	setup_show_cam();
}


void param_next(void)
{
	param_id++;

	if(param_id>=NUMBER_OF_CAM_PARAMETERS)
		param_id=0;

	param_show();
}

void param_up(void)
{
	switch(cam_setup_parameters[param_id].type)
	{
		case type_uint16:  (*(uint16_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset))++; 
						 if(*(uint16_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset)  > cam_setup_parameters[param_id].max )
							*(uint16_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset)  = cam_setup_parameters[param_id].min;
		break;
		case type_uint8:  (*(uint8_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset))++; 
					    if(*(uint8_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset)  > cam_setup_parameters[param_id].max )
						   *(uint8_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset)  = cam_setup_parameters[param_id].min;
		break;
	}
	param_show();	
}

void param_down(void)
{
	switch(cam_setup_parameters[param_id].type)
	{
		case type_uint16:  (*(uint16_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset))--; 
						 if(*(uint16_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset)  > cam_setup_parameters[param_id].max )
							*(uint16_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset)  = cam_setup_parameters[param_id].max;
		break;
		case type_uint8:  (*(uint8_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset))--; 
					    if(*(uint8_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset)  > cam_setup_parameters[param_id].max )
						   *(uint8_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset)  = cam_setup_parameters[param_id].max;
		break;
	}
	param_show();	
}

void param_show(void)
{
	static rotary_config_t rotary_param_conf = 
		{
			.data_u8 = NULL,
			.type = rotary_uint8,
			.min = 0,
			.max = 255,
			.change = param_show,
			.multi = 1,
			.wrap = 1,
			.leds_on = 1
		};

	if(cam_setup_parameters[param_id].type == type_uint16)
	{
		rotary_param_conf.data_u16 = ((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset;
		rotary_param_conf.type = rotary_uint16;
	}	
	else if(cam_setup_parameters[param_id].type == type_uint8)
	{
		rotary_param_conf.data_u8 = ((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset;
		rotary_param_conf.type = rotary_uint8;
	}

	rotary_param_conf.min = cam_setup_parameters[param_id].min;
	rotary_param_conf.max = cam_setup_parameters[param_id].max;

	rotary_setconfig(&rotary_param_conf);


	lcd_gotoxy(14,0);
	lcd_puts("CAM ");
	char tmp[10];
	itoa(setup_active_cam+1,tmp,10);
	lcd_gotoxy(18,0);
	lcd_puts(tmp);

	lcd_gotoxy(0,1);
	lcd_puts(cam_setup_parameters[param_id].name);

	switch(cam_setup_parameters[param_id].type)
	{
		case type_uint16: itoa( *(uint16_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset), tmp, 10); break;
		case type_uint8: itoa( *(uint8_t*)(((void*)&cams[setup_active_cam]) + cam_setup_parameters[param_id].offset), tmp, 10); break;
	}


	lcd_gotoxy(0,2);
	lcd_puts("                    ");
	lcd_gotoxy(0,2);
	lcd_puts(tmp);

}

void param_resetId(void)
{
	param_id=0;
}
