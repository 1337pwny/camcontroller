#ifndef __MENU_CAM_CTRL_H
#define __MENU_CAM_CTRL_H

#include "rotary.h"

void ctrl_cam_up(void);
void ctrl_cam_down(void);
void ctrl_cam_show(void);

void cam_power_on(void);
void cam_power_off(void);
void cam_button_press(void);
void cam_button_release(void);
void cam_power_show(void);

void send_switch(void);

extern rotary_config_t rotary_menu_ctrl;
#endif
