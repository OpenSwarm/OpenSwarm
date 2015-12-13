#include "i2c_data.h"
//#include <i2c.h>


#include "../definitions.h"
#include "../memory.h"

sys_i2c_messages *sys_i2c_msgs = 0;

void Sys_I2C_RemoveOldestMessage(sys_i2c_messages **list){

    sys_i2c_msg *element = *list;

    *list = (*list)->next;
    element->next = 0;

    Sys_I2C_FreeMessages(element);
}

void Sys_I2C_FreeMessages(sys_i2c_messages *list){

    sys_i2c_msg *element = 0;
    while(list != 0){
        element = list;
        list = list->next;

        if(element->data != 0){
            Sys_Free(element->data);
        }

        Sys_Free(element);
    }
}


void Sys_I2C_AppendMessages(sys_i2c_msg *item){

    if(sys_i2c_msgs == 0){
        sys_i2c_msgs = item;
        return;
    }

    sys_i2c_messages *element = sys_i2c_msgs;
    while(element->next != 0){
        element = element->next;
    }

    element->next = item;
}