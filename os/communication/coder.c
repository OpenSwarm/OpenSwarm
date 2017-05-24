/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdbool.h>
#include "coder.h"

/*
uint16 generator_p[] = {0b010011010111,
                        0b011010111100,
                        0b001101011110,
                        0b000110101111};

uint16 parity[] = { 0b0100110101111000,
                    0b0110101111000100,
                    0b0011010111100010,
                    0b0001101011110001};

uint16 syndromes[] = {0b0000000000000000,
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

uint16 parity[] = { 0b0000111101011001,
                    0b0001000111101011,
                    0b0010001111010110,
                    0b0100011110101100};

uint16 syndromes[] = {0b0000000000000000,
                    0b0100000000000000,
                    0b0010000000000000,
                    0b0000000000000100,
                    0b0001000000000000,
                    0b0000000000100000,
                    0b0000000000000010,
                    0b0000000010000000,
                    0b0000100000000000,
                    0b0000010000010000,
                    0b0000000000010000,
                    0b0000001000000000,
                    0b0000000000000001,
                    0b0000000000001000,
                    0b0000000001000000,
                    0b0000000100000000};
*/
        
uint16 generator_p[] = {0b011110101100,
                        0b001111010110,
                        0b000111101011,
                        0b011101011001};

uint16 parity[] = { 0b0100011110101100,
                    0b0010001111010110,
                    0b0001000111101011,
                    0b0000111101011001};

uint16 syndromes[] = {  0b0000000000000000,
                        0b0000100000000000,
                        0b0001000000000000,
                        0b0000000000000001,
                        0b0010000000000000,
                        0b0000000000010000,
                        0b0000000000000010,
                        0b0000000001000000,
                        0b0100000000000000,
                        0b0000010000000000,
                        0b0000000000100000,
                        0b0000000000001000,
                        0b0000000000000100,
                        0b0000001000000000,
                        0b0000000010000000,
                        0b0000000100000000};

uint8 getParity(uint16 word){
    word ^= (word >> 8);
    word ^= (word >> 4);
    word ^= (word >> 2);
    word ^= (word >> 1);
    
    return word & 1;
}

uint16 decodeBCH(uint16 segment, bool *error){
    uint16 data = 0;
    uint8 syn = 0;
    uint16 i;
    
    *error = false;
    
    for(i = 0; i < 4; i++){
        uint16 bit_seg = parity[i] & segment;
        
        syn |= getParity(bit_seg) << (3-i);
    }
    
    if(syn){ //!= 0
        *error = true;
    }
    
    data = segment ^ syndromes[syn];
    return (data & 0x7FF);
}

uint16 encodeBCH(uint16 data){
    uint16 segment = data & 0x07FF;
    uint16 i;
    
    for(i = 0; i < 4; i++){
        uint16 bit_seg = generator_p[i] & data;
        
        segment |= getParity(bit_seg) << (14-i);
    }
    
    return segment;
}