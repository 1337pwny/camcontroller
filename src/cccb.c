#include "cccb.h"
#include "uart.h"
#include "globals.h"

#define RS485_DIR_DDR DDRJ
#define RS485_DIR 3
#define RS485_DOR_PORT PORTJ


int8_t bmmcc_request_sa;
int8_t bmmcc_request_wb;
int8_t bmmcc_request_iso;


void cccb_init()
{
    RS485_DIR_DDR |= (1<<RS485_DIR);
    RS485_DOR_PORT |= (1<<RS485_DIR);

    uart3_init(UART_BAUD_SELECT(9600,F_CPU) );
}

void cccb_run()
{


    uart3_putc(0xAA);
    uart3_putc(cams[3].focus>>8);
    uart3_putc(cams[3].focus);
    uart3_putc(cams[3].iris);
    uart3_putc(bmmcc_request_wb);
    uart3_putc(bmmcc_request_sa);
    uart3_putc(bmmcc_request_iso);



    uart3_putc( *((uint8_t*)&cams[3].zoom)); //zoom

    bmmcc_request_sa=0;
    bmmcc_request_wb=0;
    bmmcc_request_iso=0;
}
