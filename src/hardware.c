#include "hardware.h"
#include "max7221.h"
#include <avr/io.h>
#include <util/delay.h>

#define BUTTON_ROW_1 7 //change set_row if changed!!!
#define BUTTON_ROW_2 6
#define BUTTON_ROW_3 5
#define BUTTON_ROW_4 4


#define BUTTON_ROW_PORT PORTD
#define BUTTON_ROW_DDR DDRD
#define BUTTON_ROW_PIN PIND
#define BUTTON_COL_PORT PORTC
#define BUTTON_COL_DDR DDRC
#define BUTTON_COL_PIN PINC

#define CAMKEY_POS 1
#define CAMKEY_1 !(button_states[0]&(1<<CAMKEY_POS))
#define CAMKEY_2 !(button_states[1]&(1<<CAMKEY_POS))
#define CAMKEY_3 !(button_states[2]&(1<<CAMKEY_POS))
#define CAMKEY_4 !(button_states[3]&(1<<CAMKEY_POS))

#define STOREKEY_POS 6
#define STOREKEY_1 !(button_states[0]&(1<<STOREKEY_POS))
#define STOREKEY_2 !(button_states[1]&(1<<STOREKEY_POS))
#define STOREKEY_3 !(button_states[2]&(1<<STOREKEY_POS))
#define STOREKEY_4 !(button_states[3]&(1<<STOREKEY_POS))

#define SOFTKEY_POS 0
#define SOFTKEY_1 !(button_states[0]&(1<<SOFTKEY_POS))
#define SOFTKEY_2 !(button_states[1]&(1<<SOFTKEY_POS))
#define SOFTKEY_3 !(button_states[2]&(1<<SOFTKEY_POS))
#define SOFTKEY_4 !(button_states[3]&(1<<SOFTKEY_POS))

#define FSELKEY_POS 7
#define FSELKEY_1 !(button_states[0]&(1<<FSELKEY_POS))
#define FSELKEY_2 !(button_states[1]&(1<<FSELKEY_POS))
#define FSELKEY_3 !(button_states[2]&(1<<FSELKEY_POS))
#define FSELKEY_4 !(button_states[3]&(1<<FSELKEY_POS))

#define ROT_PUSH_PORT PORTE
#define ROT_PUSH_PIN PINE
#define ROT_PUSH_DDR DDRE
#define ROT_PUSH 4

#define LED_CAMSEL_1 3,5  //anode,cathode
#define LED_CAMSEL_2 7,5
#define LED_CAMSEL_3 2,5
#define LED_CAMSEL_4 4,5

#define LED_STORE_1 3,6
#define LED_STORE_2 7,6
#define LED_STORE_3 2,6
#define LED_STORE_4 4,6

#define LED_FSEL_1 3,7
#define LED_FSEL_2 7,7
#define LED_FSEL_3 2,7
#define LED_FSEL_4 4,7

static uint8_t button_states[4];

void hardware_init(void)
{
	BUTTON_COL_DDR = 0x00; //All input
	BUTTON_COL_PORT= 0xFF; //All pullups
	BUTTON_ROW_DDR &= ~((1<<BUTTON_ROW_1)|(1<<BUTTON_ROW_2)|(1<<BUTTON_ROW_3)|(1<<BUTTON_ROW_4));
	ROT_PUSH_DDR &= ~(1<<ROT_PUSH);

	hardware_tick();
	hardware_tick();
	hardware_tick();
	hardware_tick();

	max7221_init();


}

static void set_row(uint8_t row)
{
	switch(row)
	{
		case 0: BUTTON_ROW_DDR =  (BUTTON_ROW_DDR & 0x0F)  | (1<<BUTTON_ROW_1); break;
		case 1: BUTTON_ROW_DDR =  (BUTTON_ROW_DDR & 0x0F)  | (1<<BUTTON_ROW_2); break;
		case 2: BUTTON_ROW_DDR =  (BUTTON_ROW_DDR & 0x0F)  | (1<<BUTTON_ROW_3); break;
		case 3: BUTTON_ROW_DDR =  (BUTTON_ROW_DDR & 0x0F)  | (1<<BUTTON_ROW_4); break;
	}
}

void hardware_tick(void)
{
	static uint8_t row=0;
	set_row(row);

	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");

	button_states[row]= BUTTON_COL_PIN;

	row++;
	if(row>3)
		row=0;

	//lcd_gotoxy(0,row);
	//char tmp[10];
	//sprintf(tmp,"%02X", button_states[row]);
	//lcd_puts(tmp);
	
}

camkey_t get_camkeys(void)
{	
	static uint8_t old_k1=0;
//static uint8_t old_k2=0;
//	static uint8_t old_k3=0;
//	static uint8_t old_k4=0;


	if(CAMKEY_1 )
	{
		if(old_k1==0)
		{
			old_k1=1;
			return CAM1;
		}
	}
	else
	{
		old_k1=0;
	}

	if(CAMKEY_2)
		return CAM2;
	if(CAMKEY_3)
		return CAM3;
	if(CAMKEY_4)
		return CAM4;
	return CAM_NO_KEY;
}

uint8_t get_cam1_key(void)
{
	return CAMKEY_1;
}

softkey_t get_softkeys(void)
{	
	static uint8_t old_k1=0;
	static uint8_t old_k2=0;
	static uint8_t old_k3=0;
	static uint8_t old_k4=0;
	static uint8_t old_rot=0;


	if( SOFTKEY_1)
	{
		if(old_k1==0)
		{
			old_k1=1;
			return SK1;
		}
	}
	else
	{
		if(old_k1==1)
		{
			old_k1=0;	
			return SK1_R;
		}
	}

	if(SOFTKEY_2)
	{
		if(old_k2==0)
		{
			old_k2=1;
			return SK2;
		}
	}
	else
	{
		if(old_k2==1)
		{
			old_k2=0;
			return SK2_R;
		}
	}


	if(SOFTKEY_3)
	{
		if(old_k3==0)
		{
			old_k3=1;
			return SK3;
		}
	}
	else
	{
		if(old_k3==1)
		{
			old_k3=0;
			return SK3_R;
		}
	}


	if(SOFTKEY_4)
	{
		if(old_k4==0)
		{
			old_k4=1;
			return SK4;
		}
	}
	else
	{
		if(old_k4==1)
		{
			old_k4=0;
			return SK4_R;
		}
	}

	if(!(ROT_PUSH_PIN&(1<<ROT_PUSH)))
	{
		if(old_rot==0)
		{
			old_rot=1;
			return ROT;
		}
	}
	else
	{
		if(old_rot==1)
		{
			old_rot=0;
			return ROT_R;
		}
	}

	return NO_KEY;
}

storekey_t get_storekeys(void)
{	

	if( STOREKEY_1)
		return STORE1;
	if( STOREKEY_2)
		return STORE2;
	if( STOREKEY_3)
		return STORE3;
	if( STOREKEY_4)
		return STORE4;
	return STORE_NO_KEY;
}

fselkey_t get_fselkeys(void)
{
    if( FSELKEY_1)
        return FSEL1;
    if( FSELKEY_2)
        return FSEL2;
    if( FSELKEY_3)
        return FSEL3;
    if( FSELKEY_4)
        return FSEL4;
    return FSEL_NO_KEY;
}

void set_store_led(uint8_t led)
{
	switch(led)
	{
		case 0: max7221_set_led(LED_STORE_1); break;
		case 1: max7221_set_led(LED_STORE_2); break;
		case 2: max7221_set_led(LED_STORE_3); break;
		case 3: max7221_set_led(LED_STORE_4); break;
	}
}

void reset_store_led(uint8_t led)
{
	switch(led)
	{
		case 0: max7221_reset_led(LED_STORE_1); break;
		case 1: max7221_reset_led(LED_STORE_2); break;
		case 2: max7221_reset_led(LED_STORE_3); break;
		case 3: max7221_reset_led(LED_STORE_4); break;
	}
}

void set_cam_leds(uint8_t active)
{
	switch(active)
	{
		case 0: max7221_set_led(LED_CAMSEL_1);
				max7221_reset_led(LED_CAMSEL_2);
				max7221_reset_led(LED_CAMSEL_3);
				max7221_reset_led(LED_CAMSEL_4);
		break;
		case 1: max7221_reset_led(LED_CAMSEL_1);
				max7221_set_led(LED_CAMSEL_2);
				max7221_reset_led(LED_CAMSEL_3);
				max7221_reset_led(LED_CAMSEL_4);
		break;
		case 2: max7221_reset_led(LED_CAMSEL_1);
				max7221_reset_led(LED_CAMSEL_2);
				max7221_set_led(LED_CAMSEL_3);
				max7221_reset_led(LED_CAMSEL_4);
		break;
		case 3: max7221_reset_led(LED_CAMSEL_1);
				max7221_reset_led(LED_CAMSEL_2);
				max7221_reset_led(LED_CAMSEL_3);
				max7221_set_led(LED_CAMSEL_4);
		break;
	}
}

void set_rotarys_leds(uint8_t number)
{
    static const uint8_t cathodes[] = { 4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3};
    static const uint8_t anodes[]   = { 4,4,4,4,7,7,7,7,3,3,3,3,2,2,2,2,0,0,0,0,5,5,5,5,1,1,1,1,6,6,6,6};

	uint8_t i=0;
	for(i=0; i < number; i++)
	{
		max7221_set_ledbuffer(anodes[i],cathodes[i]);
	}	
	for(;i<32;i++)
	{
		max7221_reset_ledbuffer(anodes[i],cathodes[i]);
	}

	max7221_send_buffer();

}

uint8_t get_matrix_line(uint8_t line)
{
	return button_states[line];
}



void set_fsel_leds(uint8_t function)
{
    switch(function)
    {
        case 0: max7221_set_led(LED_FSEL_1);
                max7221_reset_led(LED_FSEL_2);
                max7221_reset_led(LED_FSEL_3);
                max7221_reset_led(LED_FSEL_4);
        break;
        case 1: max7221_reset_led(LED_FSEL_1);
                max7221_set_led(LED_FSEL_2);
                max7221_reset_led(LED_FSEL_3);
                max7221_reset_led(LED_FSEL_4);
        break;
        case 2: max7221_reset_led(LED_FSEL_1);
                max7221_reset_led(LED_FSEL_2);
                max7221_set_led(LED_FSEL_3);
                max7221_reset_led(LED_FSEL_4);
        break;
        case 3: max7221_reset_led(LED_FSEL_1);
                max7221_reset_led(LED_FSEL_2);
                max7221_reset_led(LED_FSEL_3);
                max7221_set_led(LED_FSEL_4);
        break;
    }
}
