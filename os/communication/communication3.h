#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#include "../definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef struct Sys_RawMsg_list_s{
    uint position;
    uint message[5];
    struct Sys_RawMsg_list_s *next;
}Sys_RawMessageList;

typedef struct message_s{
    uint8           id;
    uint8           address_source;
    uint8           address_destination;
    uint8           type;
    uint32          data;
    uint8           error;
} Sys_Message;


void Sys_Init_Communication();
void Sys_Start_Communication();
void Sys_Stop_Communication();
    
bool Sys_IsReceivingMessage();

void Sys_Send_Data(uint8 address, void *data, uint length);

Sys_Message *getNewMessage();


#endif	/* COMMUNICATION_H */
