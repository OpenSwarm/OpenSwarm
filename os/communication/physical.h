#ifndef PHYSICAL_H
#define	PHYSICAL_H

#include "../definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
#define DEBUG_COM
    
typedef struct Sys_RawMsg_list_s{
    uint position;
    uint message[5];
    struct Sys_RawMsg_list_s *next;
}Sys_RawMessageList;

void Sys_Init_PhysicalLayer();
void Sys_Start_PhysicalLayer();
void Sys_Stop_PhysicalLayer();


uint Sys_ListLength(Sys_RawMessageList *list);
void Sys_AddOutMessage(Sys_RawMessageList *element);
Sys_RawMessageList *Sys_GetNextInMessage();

void Sys_SendTestPattern(void);

#endif	/* PHYSICAL_H */
