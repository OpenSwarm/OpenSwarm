// file sensext.h

#ifndef _SENSEXT
#define _SENSEXT

/* definitions */
//extension module i2c address
#define  I2C_ADDR_SENSEXT 0b10100010

// I2C Devantech Adress Definitions
#define  I2C_ADDR_SRF08 0xE0
#define  I2C_ADDR_SRF10 0xE0
#define  I2C_ADDR_CMPS03 0xC0
#define  I2C_ADDR_SRF235 0xE0

/* functions */

void e_init_sensext(void);   // to be called before starting using the Sharp
int e_sensext_process(int* sensext_param, unsigned int* sensext_value);
void e_stop_sensext_wait(void);
void e_start_sensext_wait(void);
int e_get_sensext_wait(void);

#endif
