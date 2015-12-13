#include "./motor_led/e_epuck_ports.h"

void wait(long num) {
	long i;
	for(i=0;i<num;i++);
}

int getselector() {
	return SELECTOR0 + 2*SELECTOR1 + 4*SELECTOR2 + 8*SELECTOR3;
}
