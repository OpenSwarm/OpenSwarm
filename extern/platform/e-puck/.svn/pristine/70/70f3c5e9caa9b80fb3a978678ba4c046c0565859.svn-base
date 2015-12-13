#include "p30f6014A.h"
#include "stdio.h"
#include "string.h"

#include "../../../library/motor_led/e_init_port.h"
#include "../../../library/a_d/e_accelerometer.h"
#include "../../../library/motor_led/e_led.h"
#include "../../../library/uart/e_uart_char.h"
#include "../../../library/a_d/e_ad_conv.h"
#include "../../../library/a_d/e_prox.h"
#include "utility.h"

void run_acclive() {
	int i;
	int accx, accy, accz;
	int saccx, saccy, saccz;
	char buffer[80];

	e_init_acc();

	while (1) {
		saccx=0;
		saccy=0;
		saccz=0;
		for(i=0;i<8;i++) {
			e_get_acc(&accx, &accy, &accz);
			saccx+=accx;
			saccy+=accy;
			saccz+=accz;
			wait(500);
		}
	
		sprintf(buffer, "%d, %d, %d\r\n", saccx, saccy, saccz);
		e_send_uart1_char(buffer, strlen(buffer));
		wait(10000);
	}
}
