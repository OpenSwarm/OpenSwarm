#include "p30f6014A.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "./motor_led/e_epuck_ports.h"
#include "./motor_led/e_init_port.h"
#include "./motor_led/e_led.h"
#include "./a_d/e_prox.h"
#include "./motor_led/e_motors.h"
#include "ComModule.h"
#include "utility.h"
#include "./uart/e_uart_char.h"

#include "runcollaboration.h"

#define FROM_ROBOT_PKT_ID 	0 		// ID of packet sent from epuck
#define FROM_MOTE_PKT_ID	1 		// ID of packet sent from mote to epuck
							  		// ID of packet between two motes have other IDs

#define LEFT_FOLLOW			0		// behaviors IDs	
#define RIGHT_FOLLOW		1 

#define NB_SENSORS          8		// number of sensors
#define BIAS_SPEED      	200		// robot bias speed
#define SENSOR_THRESHOLD	300		// discount sensor noise below threshold
#define MAXSPEED 			800		// maximum robot speed

#define COM_RANGE			3  		// Radio power from 1 to 31; arena range = ~3
#define OPCHANGE_PROB		25		// Probability of changing opinion * 100
#define COM_TIME			1000	// Time between decision and packet send (in miliseconds)

int opinion;			// actual behavior : LEFT_FOLLOW for left wall following, RIGHT_FOLLOW for right
int leftPacketCounter;	// # packets received of type "turning left"
int rightPacketCounter;	// # packets received of type "turning right"

int HaveToSendOpinion;	// 1 if opinion changed 0 other, this flag is used to tell the main loop to send data with the radio

//int Interconn[16] = {6,4,6,3,5,-5,-15,-20,   -18,-15,-5,5,3,4,4,6};
//int Interconn[16] = {5,1,2,3,5,4,4,4,-12,    -13,-5,-1,3,-5,-15,-23};

int collab_sensorzero[8];
int collab_weightleft[8] = {-10,-10,-5,0,0,5,10,10};
int collab_weightright[8] = {10,10,5,0,0,-5,-10,-10};

int getSelectorValue();
int sendLabPacket(int dataA, int dataB);

void collab_sensor_calibrate() {
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
		collab_sensorzero[i]=(sensor[i]>>5);
		sprintf(buffer, "%d, ", collab_sensorzero[i]);
		e_send_uart1_char(buffer, strlen(buffer));
	}

	sprintf(buffer, " calibration done\r\n");
	e_send_uart1_char(buffer, strlen(buffer));
	wait(100000);
}


// timer 1 interrupt
// this code is executed every COM_TIME ms
void __attribute__((interrupt, auto_psv, shadow))
_T1Interrupt(void) {
	int nmsg;			// Number of message received
	int OppositeOp;		// the opposit opinion
	int OppositeNb;		// the number of packet of opposit opinion

	IFS0bits.T1IF = 0;	// clear interrupt flag

	e_set_led(0,2); 		// front LED blinks when decision is done
	e_set_body_led(0);

	switch (getSelectorValue()) {
	case 13: // the robot is forced to go right
		opinion = RIGHT_FOLLOW;
		break;
	case 11: // the robot is forced to go left
		opinion = LEFT_FOLLOW;
		break;
	case 12: // normal operation
  		nmsg = leftPacketCounter + rightPacketCounter;

  		if (opinion == LEFT_FOLLOW) {
			OppositeOp = RIGHT_FOLLOW;
			OppositeNb = rightPacketCounter;
		} else {
			OppositeOp = LEFT_FOLLOW;	
			OppositeNb = leftPacketCounter;
		}

  		// Check if majority of opinions are opposing 
  		if (OppositeNb > nmsg/2) {
    		// Random chance of changing 
    		if (rand() < OPCHANGE_PROB*(RAND_MAX/100.0)) {
				opinion = OppositeOp;
			}
  		}
		break;
	default: // wrong position
		e_set_body_led(1);
		break;	
	}

	HaveToSendOpinion = 1;
	leftPacketCounter = 0;
	rightPacketCounter = 0;
}


/* init the Timer 1 */
void InitTMR1(void) {
  T1CON = 0;                   
  T1CONbits.TCKPS = 3;          	// prescsaler = 256
  TMR1 = 0;                     	// clear timer 1
  PR1 = (COM_TIME*MILLISEC)/256.0;	// interrupt after COM_TIME ms
  IFS0bits.T1IF = 0;            	// clear interrupt flag
  IEC0bits.T1IE = 1;            	// set interrupt enable bit
  T1CONbits.TON = 1;            	// start Timer1
}


/* return the selector value 	*/
/* from 0 to 15 				*/
int getSelectorValue() {
	return SELECTOR0 + 2*SELECTOR1 + 4*SELECTOR2 + 8*SELECTOR3;
}

/* send two int_16 to default group 	*/
/* return 1 if OK					 	*/
int sendLabPacket(int dataA, int dataB) {
	unsigned char packet[6];	
	packet[0] = FROM_ROBOT_PKT_ID;
	packet[1] = 0;
	packet[2] = (unsigned char)((dataA & 0xFF00)>>8);
	packet[3] = (unsigned char)(dataA & 0xFF);
	packet[4] = (unsigned char)((dataB & 0xFF00)>>8);
	packet[5] = (unsigned char)(dataB & 0xFF);
	if (SendPacket(COM_MODULE_DEFAULT_GROUP,0xff,packet,6)) {
		return 1;
	}
	return 0;
}

/* return 1 if packet received */
int LabPacketReady(int* dataA, int* dataB) {
	unsigned int size;
	unsigned char packet[COM_MODULE_MAXSIZE];
	if (IsPacketReady(packet, &size) )
	{
		if ((packet[0] == FROM_ROBOT_PKT_ID) || (packet[0] == FROM_MOTE_PKT_ID))
		{
			*dataA = (int)(packet[2])<<8 | (int)(packet[3]);
			*dataB = (int)(packet[4])<<8 | (int)(packet[5]);
			return 1;
		}
	}
	return 0;
}


/* 
	read sensor prox and return values in a int array
   	return values from 0x0000 to 0x00FF (from 0 to 255)
*/
void GetSensorValues(int *sensorTable) {
	unsigned int i;
	for (i=0; i < NB_SENSORS; i++) {
		sensorTable[i] = e_get_prox(i) - collab_sensorzero[i];
	}		
}

/*
	set robot speed
	speeds: from -MAXSPEED to MAXSPEED
*/
void setSpeed(int LeftSpeed, int RightSpeed) {
	if (LeftSpeed < -MAXSPEED) {LeftSpeed = -MAXSPEED;}
	if (LeftSpeed >  MAXSPEED) {LeftSpeed =  MAXSPEED;}
	if (RightSpeed < -MAXSPEED) {RightSpeed = -MAXSPEED;}
	if (RightSpeed >  MAXSPEED) {RightSpeed =  MAXSPEED;}
	e_set_speed_left(LeftSpeed);
	e_set_speed_right(RightSpeed); 
}

int run_collaboration(void) {
	int leftwheel, rightwheel;		// motor speed left and right
	int distances[NB_SENSORS];		// array keeping the distance sensor readings
	int i;							// FOR-loop counters
	int opinionMessage;				// the Opinion receive with the radio
	int gostraight;
	int loopcount;

	leftPacketCounter = 0;
	rightPacketCounter = 0;
	HaveToSendOpinion = 0;

	e_init_port();
	e_init_motors();
	e_init_prox();
	InitComModule(COM_MODULE_DEFAULT_GROUP,10,COM_MODULE_HW_ATTENUATOR_25DB, COM_RANGE );

	collab_sensor_calibrate();
	
	// random choice of initial condition (use the LSB of sensor 0 to choose initial direction)
	if (e_get_prox(0) & 0x01) {
		opinion = LEFT_FOLLOW;
	} else {
		opinion = RIGHT_FOLLOW;
	}

	InitTMR1(); // enable inerrupt every COM_TIME ms

	loopcount=0;
	while (1) {
		GetSensorValues(distances); // read sensor values

		gostraight=0;
		if (opinion == RIGHT_FOLLOW) {
			e_set_led(6,0);
			e_set_led(2,1);
			for (i=0; i<8; i++) {
				if (distances[i]>50) {break;}
			}
			if (i==8) {
				gostraight=1;
			} else {
				collab_weightleft[0]=-10;
				collab_weightleft[7]=-10;
				collab_weightright[0]=10;
				collab_weightright[7]=10;
				if (distances[2]>300) {
					distances[1]-=200;
					distances[2]-=600;
					distances[3]-=100;
				}
			}
		} else {
			e_set_led(6,1);
			e_set_led(2,0);
			for (i=0; i<8; i++) {
				if (distances[i]>50) {break;}
			}
			if (i==8) {
				gostraight=1;
			} else {
				collab_weightleft[0]=10;
				collab_weightleft[7]=10;
				collab_weightright[0]=-10;
				collab_weightright[7]=-10;
				if (distances[5]>300) {
					distances[4]-=100;
					distances[5]-=600;
					distances[6]-=200;
				}
			}
		}

		leftwheel=BIAS_SPEED;
		rightwheel=BIAS_SPEED;
		if (gostraight==0) {
			for (i=0; i<8; i++) {
				leftwheel+=collab_weightleft[i]*(distances[i]>>4);
				rightwheel+=collab_weightright[i]*(distances[i]>>4);
			}
		}

		// set robot speed
		setSpeed(leftwheel, rightwheel);

		if (loopcount<1000) {
			loopcount++;
			LabPacketReady(&opinionMessage,&i);
		} else {
			// check if a packet is ready in the com module
			if (LabPacketReady(&opinionMessage,&i)) { // i not used but must have 2 params
				switch (opinionMessage) {
				case LEFT_FOLLOW:
					leftPacketCounter++; // it was a LEFT packet so increment left counter
					break;
				case RIGHT_FOLLOW:
					rightPacketCounter++; // it was a RIGHT packet so increment right counter
					break;
				default:
					break;
				}
			}

			// check it it's time to send our own packet
			if (HaveToSendOpinion) {
				sendLabPacket(opinion,0); // if yes, send it to with our opinion
				HaveToSendOpinion = 0; // packet sent, clear the flag
			}
		}

		// make sure the main loop is not done too fast
		wait(15000);
	}

	return 0;	
}
