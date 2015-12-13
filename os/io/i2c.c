#include "i2c.h"
#include "i2c_data.h"
#include "../platform/e-puck/system_IO_i2c_HDI.h"
//#include <i2c.h>

#include <stdlib.h>
#include <stdbool.h>

#include "../definitions.h"
#include "../memory.h"
#include "../interrupts.h"

inline void Sys_I2C_Send_Start();
inline void Sys_I2C_Send_Restart(void);
inline void Sys_I2C_Send_Stop(void);
inline void Sys_I2C_Send_ACK(void);
inline void Sys_I2C_Send_NACK(void);
inline void Sys_I2C_Start_Reading(void);
inline char Sys_I2C_ReadByte(void);
inline void Sys_I2C_WriteByte(uint8 byte);

inline void Sys_Init_I2C(){
    Sys_Init_I2C_HDI();
}

inline void Sys_Start_I2C(){
    Sys_Start_I2C_HDI();
}

inline void Sys_Pause_I2C(){
    Sys_Pause_I2C_HDI();
}

inline void Sys_Contine_I2C(){
    Sys_Contine_I2C_HDI();
}

inline void Sys_Stop_I2C(){
    Sys_Stop_I2C_HDI();
}

/* MAster I2C interrupt
 * it gets called if:
 *      Start was sent/received
 *      Stop was sent/received
 *      Data Transfer completed (read | write)
 *      ACK was sent/recieved
 *      repeated Start was sent/recieved
 *      Carrier collusion :(
 
void  __attribute__((__interrupt__, auto_psv)) _MI2CInterrupt(void)
{
    IFS0bits.MI2CIF=0; // clear interrupt flag

    switch(sys_i2c_mode){//Am I sending or receiving -> diffrent state machines
        case I2C_IDLE_MODE:
            if(sys_i2c_msgs == 0){
               if(I2CSTATbits.P == 1){//STOP was sent
                    //Communication is finished
                    sys_i2c_state = I2C_IDLE;
                }else{
                    Sys_I2C_Send_Stop();
                }

               return;
            }

            sys_i2c_state = I2C_IS_STARTING;
            sys_i2c_mode = I2C_WRITING_ADDRESS_MODE;
            Sys_I2C_Send_Start();
            return;
        case I2C_WRITING_ADDRESS_MODE:
            if(I2CSTATbits.S == 1){//Start ir ReStart was sent
                sys_i2c_state = I2C_STARTED;

                if(sys_i2c_msgs == 0){//Should not happen
                        sys_i2c_state = I2C_IS_STOPPING;
                        Sys_I2C_Send_Stop();
                        return;
                }

                uint8 byte = sys_i2c_msgs->i2c_device_address<<1;
                if(sys_i2c_msgs->write){
                    byte &= 0b11111110;
                    sys_i2c_mode = I2C_WRITING_BYTES_MODE;
                }else{
                    byte |= 0b00000001;
                    sys_i2c_mode = I2C_READING_BYTES_MODE;
                }

                sys_i2c_state = I2C_IS_SENDING;
                Sys_I2C_WriteByte(byte);
                return;
            }else{//shouldn't occur
               if(I2CSTATbits.P == 1){//STOP was sent
                    //Communication is finished
                    sys_i2c_state = I2C_IDLE;
                }else{
                    sys_i2c_state = I2C_IS_STOPPING;
                    Sys_I2C_Send_Stop();
                }

               return;
            }
            break;
        case I2C_READING_BYTES_MODE:
            if(sys_i2c_state == I2C_IS_SENDING && I2CSTATbits.ACKSTAT == 1){
                sys_i2c_state = I2C_IS_READING;
                Sys_I2C_Start_Reading();
                return;
            }

            if(sys_i2c_state == I2C_IS_READING && I2CSTATbits.RBF == 1){
                sys_i2c_state = I2C_ACKNOWLEDGED;

                uint8 byte = Sys_I2C_ReadByte();

                sys_i2c_msgs->handler(byte);
                Sys_I2C_RemoveOldestMessage(&sys_i2c_msgs);

                Sys_I2C_Send_NACK();
            }

            if(sys_i2c_state == I2C_ACKNOWLEDGED){
                if(sys_i2c_msgs == 0){//if this was he last message
                    sys_i2c_state = I2C_IS_STOPPING;
                    Sys_I2C_Send_Stop();//stop conversation
                    return;
                }else{//if there is another
                    sys_i2c_mode = I2C_WRITING_ADDRESS_MODE;
                    sys_i2c_state = I2C_IS_STARTING;
                    Sys_I2C_Send_Restart();
                    return;
                }
            }
            
            break;
        case I2C_WRITING_BYTES_MODE:

            if(sys_i2c_state == I2C_IS_SENDING && I2CSTATbits.ACKSTAT == 0){//ACK was received from slave = previous byte was successfully sent
                                         //Hardware set or clear at end of slave Acknowledge.
                static uint16 position = 0;
                sys_i2c_state = I2C_SENT;

                if(sys_i2c_msgs == 0){//THAT shouldn't happen
                    sys_i2c_state = I2C_IS_STOPPING;
                    Sys_I2C_Send_Stop();
                    return;
                }

                if(sys_i2c_msgs->data == 0){//THAT shouldn't happen
                    sys_i2c_state = I2C_IS_STOPPING;
                    Sys_I2C_Send_Stop();
                    return;
                }

                if(sys_i2c_msgs->length == position){
                    Sys_I2C_RemoveOldestMessage(&sys_i2c_msgs);
                    if(sys_i2c_msgs == 0){//if this was he last message
                        sys_i2c_state = I2C_IS_STOPPING;
                        Sys_I2C_Send_Stop();//stop conversation
                        return;
                    }else{//if there is another
                        sys_i2c_mode = I2C_WRITING_ADDRESS_MODE;
                        sys_i2c_state = I2C_IS_STARTING;
                        Sys_I2C_Send_Restart();
                        return;
                    }
                }else{
                    sys_i2c_state = I2C_IS_SENDING;
                    Sys_I2C_WriteByte(sys_i2c_msgs->data[position++]);
                    return;
                }
            }

            break;
        default://shouldn't happen
            sys_i2c_state = I2C_IS_STOPPING;
            sys_i2c_mode = I2C_ERROR_MODE;
            Sys_I2C_Send_Stop();
            break;

    }

    if(sys_i2c_state == I2C_IS_STOPPING && I2CSTATbits.P == 1){//STOP
        sys_i2c_state = I2C_IDLE;
        sys_i2c_mode = I2C_IDLE_MODE;
        return;
    }


    if(I2CSTATbits.BCL == 1 || I2CSTATbits.I2COV == 1 || I2CSTATbits.IWCOL == 1){
        sys_i2c_state = I2C_IS_STOPPING;
        sys_i2c_mode = I2C_ERROR_MODE;
        Sys_I2C_Send_Stop();
        return;
    }


    sys_i2c_state = I2C_IS_STOPPING;
    Sys_I2C_Send_Stop();
    return;
}
*/

inline void Sys_I2C_Send_Start(){
    Sys_I2C_Send_Start_HDI();
}

inline void Sys_I2C_Send_Restart(){
    Sys_I2C_Send_Restart_HDI();
}

inline void Sys_I2C_Send_Stop(){
    Sys_I2C_Send_Stop_HDI();
}

inline void Sys_I2C_Send_ACK(){
    Sys_I2C_Send_ACK_HDI();
}

inline void Sys_I2C_Send_NACK(){
    Sys_I2C_Send_NACK_HDI();
}


inline void Sys_I2C_Start_Reading(){
    Sys_I2C_Start_Reading_HDI();
}

inline char Sys_I2C_ReadByte(){
    return Sys_I2C_ReadByte_HDI();
}

inline void Sys_I2C_WriteByte(uint8 byte){
    Sys_I2C_WriteByte_HDI(byte);
}

void Sys_I2C_SentBytes(uint8 address, uint8 *bytes, uint16 length){
    sys_i2c_msg *new = Sys_Malloc(sizeof(sys_i2c_msg));
    if(new == 0){
        return;
    }

    new->i2c_device_address = address;
    new->data = bytes;
    new->length = length;
    new->handler = 0;
    new->write = true;
    new->next = 0;

    Sys_I2C_AppendMessages(new);
}

void Sys_I2C_Read(uint8 address, uint8 *intern_address, uint16 length, pByteFunction bytehandler){
    sys_i2c_msg *new = Sys_Malloc(sizeof(sys_i2c_msg));
    if(new == 0){
        Sys_Free(intern_address);
        return;
    }

    new->i2c_device_address = address;
    new->data = intern_address;
    new->length = length;
    new->handler = 0;
    new->write = true;

    sys_i2c_msg *second = Sys_Malloc(sizeof(sys_i2c_msg));
    if(second == 0){
        Sys_Free(intern_address);
        Sys_Free(new);
        return;
    }

    new->next = second;
    second->i2c_device_address = address;
    second->data = 0;
    second->length = 0;
    second->handler = bytehandler;
    second->write = false;
    second->next = 0;

    Sys_I2C_AppendMessages(new);
}