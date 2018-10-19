#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>
#include "rotary.h"
#include "hardware.h"

#define ROTARY_DDR DDRE
#define ROTARY_PORT PORTE
#define ROTARY_PIN PINE

#define ROTARY_A 6
#define ROTARY_B 5


#define PHASE_A     (ROTARY_PIN & 1<<ROTARY_A)
#define PHASE_B     (ROTARY_PIN & 1<<ROTARY_B)

static int8_t encode_read( void );
static void calc_number_of_leds(int32_t tmp);


static int8_t enc_delta;          // -128 ... 127
static int8_t last;
static rotary_config_t* current_config = NULL;

void rotary_init(void)
{
	ROTARY_DDR &= ~( (1<<ROTARY_A) | (1<<ROTARY_B));

	int8_t new;

	new = 0;
	if( PHASE_A ) new = 3;
	if( PHASE_B ) new ^= 1;       // convert gray to binary
	last = new;                   // power on state
	enc_delta = 0;
}

void rotary_tick(void)
{
	int8_t new, diff;

	new = 0;
	if( PHASE_A ) new = 3;
	if( PHASE_B ) new ^= 1;          // convert gray to binary
	diff = last - new;               // difference last - new
	if( diff & 1 ) 
	{                 // bit 0 = value (1)
		last = new;                    // store new as next last
		enc_delta += (diff & 2) - 1;   // bit 1 = direction (+/-)
	}
}

void rotary_setconfig(rotary_config_t* config)
{
	current_config = config;

	if(!current_config || !current_config->leds_on)
	{
		set_rotarys_leds(0);
	}
	else
	{
		int32_t tmp = current_config->type==rotary_uint8 ? *current_config->data_u8 : *current_config->data_u16;
		calc_number_of_leds(tmp);
	}
}

void rotary_process(void)
{
	int8_t diff = encode_read();
	int32_t tmp;

	if(diff!=0 && current_config!=NULL)
	{
		diff = diff * current_config->multi;

		switch(current_config->type)
		{
			case rotary_uint8: tmp = *(current_config->data_u8)+(int16_t)diff; 

								if( diff>0  && tmp > current_config->max )
								{
									tmp = current_config->wrap==1? current_config->min : current_config->max;
								}
								else if( diff < 0 &&  tmp < (int16_t)current_config->min )
								{
									tmp = current_config->wrap==1? current_config->max : current_config->min;
								}

								*(current_config->data_u8)=tmp;

			break;
			case rotary_uint16: tmp = *(current_config->data_u16)+(int32_t)diff; 

								if( diff>0  && tmp > current_config->max )
								{
									tmp = current_config->wrap==1? current_config->min : current_config->max;
								}
								else if( diff < 0 &&  tmp < (int32_t)current_config->min )
								{
									tmp = current_config->wrap==1? current_config->max : current_config->min;
								}

								*(current_config->data_u16)=tmp;

			break;
		}		

		if(current_config->change)
			current_config->change();

		if(current_config->leds_on)
		{
			calc_number_of_leds(tmp);
		}
		else
		{
			set_rotarys_leds(0);
		}	
	}
}

static void calc_number_of_leds(int32_t tmp)
{
	uint16_t divisor = (current_config->max-current_config->min);

	if(divisor==0)
		divisor=1;
	uint8_t number_of_leds = (32.0 / divisor) * (float)tmp;

	if(number_of_leds>32)
		number_of_leds=32;
	set_rotarys_leds(number_of_leds);
}

static int8_t encode_read( void )         // read single step encoders
{
  int8_t val;
  val = enc_delta;
  enc_delta = val & 3;

  return val >> 2;
}



