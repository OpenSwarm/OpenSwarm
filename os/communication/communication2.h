/* 
 * File:   communication.h
 * Author: strenkwalder
 *
 * Created on 02 May 2017, 22:48
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#include "../definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef struct Sys_RawMsg_list_s{
    uint position;
    uint message[5];
    struct message_body *next;
}Sys_RawMessageList;
    
typedef enum Sys_MsgType_e{
    FINAL = 0,
    CONTINUOUS,
    REPLY
} Sys_MsgType;

typedef struct message_s{
    uint8           id;
    uint8           address_source;
    uint8           address_destination;
    Sys_MsgType     type;
    uint32          data;
    uint8           error;
} Sys_Message;

void Sys_Init_Communication();
void Sys_Start_Communication();
void Sys_Stop_Communication();
    
void Sys_Send_Message(Sys_Message *data);
void Sys_Send_Data(uint8 address, void *data, uint length)
Sys_Message *getNewMessage();




#ifdef	__cplusplus
}
#endif

#endif	/* COMMUNICATION_H */

