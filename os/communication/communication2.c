#include "communication.h"

#include "../events/events.h"
#include "../platform/e-puck/adc.h"

uint sensorReadings[8] ={0};
uint minReadings = 0;

uint sensorThreshold[8]={0};

typedef struct message_body{
    uint position;
    uint message[5];
}Sys_RawMessage;


typedef enum rx_state{
    waiting,
    receiving
} Sys_RX_state;

Sys_RX_state rxState = waiting;


typedef struct comm_device{
    uint8 address;
    uint8 msg_id;
}Sys_CommDevice;

static Sys_CommDevice sys_com_device;

void ComSensor0(uint);
void ComSensor1(uint);
void ComSensor2(uint);
void ComSensor3(uint);
void ComSensor4(uint);
void ComSensor5(uint);
void ComSensor6(uint);
void ComSensor7(uint);
void ComReceived(void);

void InitReceiving(uint,uint);
void AddBit(uint,uint);

void Sys_Init_Communication(){
    Sys_Init_ADC();//make sure ADCs are initialised
    
    PULSE_IR0 = 0;
    PULSE_IR1 = 0;
    PULSE_IR2 = 0;
    PULSE_IR3 = 0;  
    
    Sys_Register_Event(SYS_EVENT_COM_RX_MSG);
    Sys_Register_Event(SYS_EVENT_COM_TX_MSG);
    
    Sys_Subscribe_ADCChannelProcessor(Prx0, ComSensor0);
    Sys_Subscribe_ADCChannelProcessor(Prx1, ComSensor1);
    Sys_Subscribe_ADCChannelProcessor(Prx2, ComSensor2);
    Sys_Subscribe_ADCChannelProcessor(Prx3, ComSensor3);
    Sys_Subscribe_ADCChannelProcessor(Prx4, ComSensor4);
    Sys_Subscribe_ADCChannelProcessor(Prx5, ComSensor5);
    Sys_Subscribe_ADCChannelProcessor(Prx6, ComSensor6);
    Sys_Subscribe_ADCChannelProcessor(Prx7, ComSensor7); 
    
    Sys_Subscribe_ADCFinish(ComReceived, ProxDone); 
    
    rxState=waiting;
}

void Sys_Start_Communication();
void Sys_Stop_Communication();
    

void ComSensor0(uint s){
    sensorReadings[0] = s;
}

void ComSensor1(uint s){
    sensorReadings[1] = s;
}

void ComSensor2(uint s){
    sensorReadings[2] = s;
}

void ComSensor3(uint s){
    sensorReadings[3] = s;
}

void ComSensor4(uint s){
    sensorReadings[4] = s;
}

void ComSensor5(uint s){
    sensorReadings[5] = s;
}

void ComSensor6(uint s){
    sensorReadings[6] = s;
}

void ComSensor7(uint s){
    sensorReadings[7] = s;
}

void ComReceived(void){
    uint min = 0xFFFF;
    uint i_min = 0;
    int i = 0;
    
    for(i = 0; i < 8; i++){
        if(sensorReadings[i] < min){
            min = sensorReadings[i];
            i_min = i;
        }
    }
    
    switch(rxState){
        case waiting:
        case receiving:
        default:
            AddBit(min,i_min);
            break;
    }
    
    if(min <= sensorThreshold[i_min]){//if the signal exceeds threshold
        ;
    }
}

void InitReceiving(uint value, uint pos){
    
}

Sys_RawMessage msg_RX_Buffer;

Sys_RawMessage msg_full;
bool finish_reading;

void AddBit(uint value, uint pos){
    uint block = msg_RX_Buffer.position / 15;
    
    msg_RX_Buffer.message[block] <<= 1;
    if(value >= sensorThreshold[pos]){
        msg_RX_Buffer.message[block] |= 0x0001;
    }
    
    msg_RX_Buffer.position++;
    
    if(msg_RX_Buffer.position > 63){
        msg_full = msg_RX_Buffer;
        msg_RX_Buffer.position = 0;
        finish_reading = true;
    }
}

bool isNewMessageReady(){
    return finish_reading;
}

uint generator_p[] = {  0b010011010111,
                        0b011010111100,
                        0b001101011110,
                        0b000110101111};

uint parity[] = {   0b0100110101111000,
                    0b0110101111000100,
                    0b0011010111100010,
                    0b0001101011110001};

uint syndromes[] = {0b0000000000000000,
                    0b0000000000000001,
                    0b0000000000000010,
                    0b0001000000000000,
                    0b0000000000000100,
                    0b0000001000000000,
                    0b0010000000000000,
                    0b0000000010000000,
                    0b0000000000001000,
                    0b0000000000010000,
                    0b0000010000000000,
                    0b0000000000100000,
                    0b0100000000000000,
                    0b0000100000000000,
                    0b0000000100000000,
                    0b0000000001000000};

char getParity(uint word){
    word ^= (word >> 8);
    word ^= (word >> 4);
    word ^= (word >> 2);
    word ^= (word >> 1);
    
    return word & 1;
}

uint decodeBCH(uint segment, bool *error){
    uint data = 0;
    uint8 syn = 0;
    int i;
    
    *error = false;
    
    for(i = 0; i < 4; i++){
        uint8 bit_seg = parity[i] ^ segment;
        syn |= getParity(bit_seg) << (3-i);
    }
    
    if(syn){ //!= 0
        *error = true;
    }
    
    data ^= syndromes[syn];
    return (data >> 4);
}

uint encodeBCH(uint data){
    uint segment = 0;
    segment = data << 4;
    int i;
    
    for(i = 0; i < 4; i++){
        uint8 bit_seg = generator_p[i] ^ data;
        segment |= getParity(bit_seg) << (3-i);
    }
    
    return segment;
}

Sys_Message *getNewMessage(){
    static Sys_Message out;
    int error_num = 0;
    uint buffer = 0;
    bool error;
    
    uint data = decodeBCH(msg_full.message[0], &error);//first 11 bits
    if(error){
        error_num++;
    }
    
    
    if((data & 0x400) == 0){// != 0 //is the start bit '1' there -> if not increase error
        error_num++;
    }
    
    // ID 7-bit [10-4] != 0
    buffer = (data >> 3) & 0x7F;
    if(buffer == 0){// != 0
        error_num++;
    }
    
    out.id = (uint8) buffer;
    
    // Source address 0:[3-1]+1:[11-9] != 0
    buffer = (data & 0x07) << 3; //0:[3-1]
    data = decodeBCH(msg_full.message[1], &error);
    if(error){
        error_num++;
    }
    
    buffer |= data >> 8;//1:[11-9]
    if(buffer == 0){// != 0
        error_num++;
    }
    
    out.address_source = (uint8) buffer;
    
    // destination address 1:[8-3]
    buffer = (0xFC & data) >> 2;
    out.address_destination = (uint8) buffer;
    
    // Type 1:[2-1]+2:[11]
    buffer = (0x03 & data) << 1;//1:[2-1]
    
    data = decodeBCH(msg_full.message[2], &error);
    if(error){
        error_num++;
    }
    
    buffer = data >> 10;
    out.type |= (uint8) buffer;//2:[11]
    
    // Data 2:[10-1]+3:[11-1]+4:[11-1]
    uint32 longData = ( (uint32) (data & 0x03FF) ) << 22;//2:[10-1]
    
    data = decodeBCH(msg_full.message[3], &error);
    if(error){
        error_num++;
    }
    
    longData |= ((uint32) data) << 11;//3:[11-1]
    
    data = decodeBCH(msg_full.message[4], &error);
    if(error){
        error_num++;
    }
       
    longData |= (uint32) data;  //4:[11-1]
    out.data = longData;
    out.error = error_num;
    
    return &out;
}

/*
void Sys_Send_Message(Sys_Message *data){
    Sys_RawMessageList *out;
    
    out = Sys_Malloc(sizeof(Sys_RawMessageList));
    
    
    uint16 buffer = 0x0400;
    buffer |= ((uint16) data->id) << 3;
    buffer |= ((uint16) data->address_source) >> 3;
    out.message[0] = encodeBCH(buffer);
    
    buffer  = ((uint16) (data->address_source & 0x07)) << 8;
    buffer |= ((uint16) data->address_destination) << 2;
    buffer |= ((uint16) data->type) >> 1;
    out.message[1] = encodeBCH(buffer);
    
    buffer  = ((uint16) (data->type & 0x01)) << 10;
    buffer |= (uint16) (data->data >> 22);
    out.message[2] = encodeBCH(buffer);
    
    buffer = (uint16) ((data->data >> 11) & 0x07FF);
    out.message[3] = encodeBCH(buffer);
    
    buffer = (uint16) (data->data & 0x07FF);        
    out.message[4] = encodeBCH(buffer);
    
}


void Sys_Send_Word(uint32 data, uint8 address){
    Sys_RawMessageList *out;
    
    out = Sys_Malloc(sizeof(Sys_RawMessageList));
    
    
    uint16 buffer = 0x0400;
    buffer |= ((uint16) data->id) << 3;
    buffer |= ((uint16) data->address_source) >> 3;
    out.message[0] = encodeBCH(buffer);
    
    buffer  = ((uint16) (data->address_source & 0x07)) << 8;
    buffer |= ((uint16) data->address_destination) << 2;
    buffer |= ((uint16) data->type) >> 1;
    out.message[1] = encodeBCH(buffer);
    
    buffer  = ((uint16) (data->type & 0x01)) << 10;
    buffer |= (uint16) (data->data >> 22);
    out.message[2] = encodeBCH(buffer);
    
    buffer = (uint16) ((data->data >> 11) & 0x07FF);
    out.message[3] = encodeBCH(buffer);
    
    buffer = (uint16) (data->data & 0x07FF);        
    out.message[4] = encodeBCH(buffer);
    
}
*/

Sys_RawMessageList *Sys_convert_WordToPackage(uint8 address, uint32 dvalue, Sys_MsgType type);

Sys_RawMessageList *Sys_OutMsg_List = 0;
Sys_RawMessageList **Sys_OutMsg_List_End = &Sys_OutMsg_List;

void Sys_Send_Data(uint8 address, void *data, uint length){
    Sys_RawMessageList *list = 0;
    Sys_RawMessageList **list_end = &list;
    uint32 dvalue = 0;
    
    while(length >= 4){
        uint32* dpointer = (uint32 *)data;
        
        dvalue = *dpointer;
        data = &dpointer[1];//next element
        length -= 4;
        
        Sys_RawMessageList *element = Sys_convert_WordToPackage(address, dvalue, FINAL);
        *list_end = element
        list_end = &(element->next);        
    }
    
    if(length > 0){
        Sys_MemCpy(data,dvalue,length);
    }
    
    Sys_RawMessageList *element = Sys_convert_WordToPackage(address, dvalue, FINAL);
    *list_end = element
    list_end = &(element->next);  
    //queue all packages to be sent
    
    Sys_Start_AtomicSection()
        *Sys_OutMsg_List_End = list;
        Sys_OutMsg_List_End = list_end;
    Sys_End_AtomicSection()
}

Sys_RawMessageList *Sys_convert_WordToPackage(uint8 address, uint32 dvalue, Sys_MsgType type){
    Sys_RawMessageList *out = 0;
    static uint8 out_id = 1;
    
    out = Sys_Malloc(sizeof(Sys_RawMessageList));
    out->next = 0;
    
    uint16 buffer = 0x0400;
    buffer |= ((uint16) (sys_com_device.msg_id+1) << 3;
    sys_com_device.msg_id = (sys_com_device.msg_id+1) % 0x7E;
    
    buffer |= ((uint16) sys_com_device.address) >> 3;
    out.message[0] = encodeBCH(buffer);
    
    buffer  = ((uint16) (sys_com_device.address & 0x07)) << 8;
    buffer |= ((uint16) address) << 2;
    buffer |= ((uint16) (type & 0x07)) >> 1;
    out.message[1] = encodeBCH(buffer);
    
    buffer  = ((uint16) (type & 0x01)) << 10;
    buffer |= (uint16) (dvalue >> 22);
    out.message[2] = encodeBCH(buffer);
    
    buffer = (uint16) ((dvalue >> 11) & 0x07FF);
    out.message[3] = encodeBCH(buffer);
    
    buffer = (uint16) (dvalue & 0x07FF);        
    out.message[4] = encodeBCH(buffer);
    
    return out;
}


