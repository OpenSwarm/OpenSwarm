/*
 * File:   system.h
 * Author: Stefan
 *
 * Created on 07 July 2014, 14:15
 */

#include "definitions.h"

void Sys_Init_Kernel();   //initialises the hardware
void Sys_Start_Kernel(void);    //starts all needed functions

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

