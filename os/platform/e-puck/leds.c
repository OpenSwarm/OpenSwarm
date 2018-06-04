#include "leds.h"

#include "../../definitions.h"

void clearLEDs(){
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
    BODY_LED = 0;
    FRONT_LED = 0;
}

void setLEDring(){
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
    BODY_LED = 0;
    FRONT_LED = 0;
}

void setLEDs(){
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
    BODY_LED = 1;
    FRONT_LED = 1;
}


void clearIRs(){  
    PULSE_IR0 = 0;
    PULSE_IR1 = 0;
    PULSE_IR2 = 0;
    PULSE_IR3 = 0;
}
void setIRs(){  
    PULSE_IR0 = 1;
    PULSE_IR1 = 1;
    PULSE_IR2 = 1;
    PULSE_IR3 = 1;
}