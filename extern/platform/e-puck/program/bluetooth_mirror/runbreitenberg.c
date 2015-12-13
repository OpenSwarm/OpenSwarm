#include "p30f6014A.h"
#include "stdio.h"
#include "string.h"

#include <codec/e_sound.h>
#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <motor_led/advance_one_timer/e_led.h>
#include <uart/e_uart_char.h>
#include <a_d/advance_ad_scan/e_ad_conv.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <math.h>

void wait(long num) {
	long i;
	for(i=0;i<num;i++);
}


int sensorzero[8];
int weightleft[8] = {-10,-10,-5,0,0,5,10,10};
int weightright[8] = {10,10,5,0,0,-5,-10,-10};

void sensor_calibrate() {
	int i, j;
	char buffer[80];
	long sensor[8];

	for (i=0; i<8; i++) {
		sensor[i]=0;
	}
	
	for (j=0; j<32; j++) {
		for (i=0; i<8; i++) {
			sensor[i]+=e_get_prox(i);
		}
		wait(10000);
	}

	for (i=0; i<8; i++) {
		sensorzero[i]=(sensor[i]>>5);
		sprintf(buffer, "%d, ", sensorzero[i]);
		e_send_uart1_char(buffer, strlen(buffer));
	}


	wait(100000);
}

void run_breitenberg() {
	int i;
	int sensor;
	char buffer[80];
	int leftwheel, rightwheel;

	// Init sensors
	e_start_agendas_processing();
	e_init_port();
	e_init_motors();
	e_init_ad_scan(ALL_ADC);

	// Calibrate sensors
	e_set_led(8, 1);
	sensor_calibrate();
	e_set_led(8, 0);

	// 
	while (1) {
		leftwheel=200;
		rightwheel=200;
		for (i=0; i<8; i++) {
			sensor=e_get_prox(i)-sensorzero[i];
			leftwheel+=weightleft[i]*(sensor>>4);
			rightwheel+=weightright[i]*(sensor>>4);
		}

		
		
		if (leftwheel>800) {leftwheel=800;}
		if (rightwheel>800) {rightwheel=800;}
		if (leftwheel<-800) {leftwheel=-800;}
		if (rightwheel<-800) {rightwheel=-800;}
		e_set_speed_left(leftwheel);
		e_set_speed_right(rightwheel);
		sprintf(buffer, "D,%d,%d\r", leftwheel, rightwheel);
		e_send_uart1_char(buffer, strlen(buffer));
		while(e_uart1_sending());
		wait(100000);
	}
}

