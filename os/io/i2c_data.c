/**
 * @file i2c_data.c
 * @author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 10 August 2015
 *
 * LICENSE: adapted FreeBSD License
 * Copyright (c) 2015, Stefan M. Trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code (as code or binary) is, in any form, used for an commercial product or service (in any form), this product or service must provide a clear notice/message to any user/customer that OpenSwarm was used in this product.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * @section DESCRIPTION
 *
 * This file includes the IO timer and functions to execute IO functions periodically.
 */
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
