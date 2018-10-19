#ifndef __CCCB_H
#define __CCCB_H
#include <stdint.h>

void cccb_init(void);
void cccb_run(void);

extern int8_t bmmcc_request_sa;
extern int8_t bmmcc_request_wb;
extern int8_t bmmcc_request_iso;


#endif
