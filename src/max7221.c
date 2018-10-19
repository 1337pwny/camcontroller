#include "max7221.h"
#include <util/delay.h>
#include <avr/io.h>

#define MAX_PORT PORTK
#define MAX_PIN PINK
#define MAX_DDR DDRK
#define MAX_DIN 5
#define MAX_LOAD 6
#define MAX_CLK 7

#define MAX7221_NOOP 0x0
#define MAX7221_DECODE_MODE 0x9
#define MAX7221_INTENSITY 0xA
#define MAX7221_SCAN_LIMIT 0xB
#define MAX7221_SHUTDOWN 0xC
#define MAX7221_TEST 0xF

void max7221_write_reg(uint8_t reg, uint8_t data);
void max7221_update_digit(uint8_t digit);

static uint8_t digits[8];

void max7221_init(void)
{
	MAX_DDR |= (1<<MAX_DIN)|(1<<MAX_LOAD)|(1<<MAX_CLK);

	MAX_PORT |= (1<<MAX_LOAD); 
	MAX_PORT &=~(1<<MAX_CLK);

	_delay_ms(10);

	max7221_write_reg(MAX7221_DECODE_MODE, 0x00); //no decoding!
	_delay_ms(1);
	max7221_write_reg(MAX7221_SCAN_LIMIT, 0x7); //all digits
	_delay_ms(5);
	//Explicitly shutting down and restarting, also disabling test mode. This might do the trick
	max7221_write_reg(MAX7221_TEST, 0x0);
	_delay_ms(5);
	max7221_write_reg(MAX7221_SHUTDOWN, 0x0);
	_delay_ms(5);
	max7221_write_reg(MAX7221_SHUTDOWN, 0x1);
	_delay_ms(5);
	max7221_write_reg(MAX7221_INTENSITY, 255);

	for(uint8_t i=0; i < 8; i++)
	{
		digits[i]=0x00;
		max7221_update_digit(i);
	}

}


void max7221_set_intensity(uint8_t intensity)
{
	max7221_write_reg(MAX7221_INTENSITY, intensity);
}

void max7221_set_led(uint8_t anode, uint8_t cathode)
{
	digits[cathode] |= (1<<anode);
	max7221_update_digit(cathode);
}

void max7221_reset_led(uint8_t anode, uint8_t cathode)
{
	digits[cathode] &=~(1<<anode);
	max7221_update_digit(cathode);
}

void max7221_set_ledbuffer(uint8_t anode, uint8_t cathode)
{
	digits[cathode] |= (1<<anode);
}

void max7221_reset_ledbuffer(uint8_t anode, uint8_t cathode)
{
	digits[cathode] &=~(1<<anode);
}

void max7221_send_buffer(void)
{
	for(uint8_t i=0; i < 8; i++)
		max7221_update_digit(i);
}

void max7221_set_digit(uint8_t cathode, uint8_t data)
{
	digits[cathode]=data;
	max7221_update_digit(cathode);
}

void max7221_update_digit(uint8_t digit)
{
	max7221_write_reg(digit+1, digits[digit]);
}

void max7221_write_reg(uint8_t reg, uint8_t data)
{
	MAX_PORT &= ~(1<<MAX_LOAD); // CS low

	uint16_t word=0;

	word = reg<<8|data;
	
	uint16_t mask = 0x8000;
	for(uint8_t i=0; i < 16; i++)
	{
		if(word&mask)
			MAX_PORT |= (1<<MAX_DIN);
		else
			MAX_PORT &=~(1<<MAX_DIN);

		MAX_PORT |= (1<<MAX_CLK);
		asm("NOP");
		asm("NOP");
		asm("NOP");
		MAX_PORT &=~(1<<MAX_CLK);
		
		
		
		mask=mask>>1;
	}
	MAX_PORT |= (1<<MAX_LOAD); //CS high
}
