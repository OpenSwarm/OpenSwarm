/*!
 * \file
 * \ingroup i2c
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 10 August 2015
 * 
 * \brief It defines functions to manage the I2C queue.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "i2c_data.h"
//#include <i2c.h>

#include "../../definitions.h"
#include "../../memory.h"

sys_i2c_messages *sys_i2c_msgs = 0;  /*!< Pointer to the linked list of messages */

/**
 *
 * This function removes the oldest message (first element) of the linked list
 *
 * @param[in,out] list pointer to the linked list
 */
void Sys_I2C_RemoveOldestMessage(sys_i2c_messages **list){

    sys_i2c_msg *element = *list;

    *list = (*list)->next;
    element->next = 0;

    Sys_I2C_FreeMessages(element);
}

/**
 *
 * This function frees all messages of the linked list.
 *
 * @param[in] list pointer to a list of elements that should be removed
 */
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

/**
 *
 * This function appends  on the bottom of the linked list.
 *
 * @param[in,out] item  pointer to a element that should be added
 */
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
