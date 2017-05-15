/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   coder.h
 * Author: strenkwalder
 *
 * Created on 09 May 2017, 16:38
 */

#ifndef CODER_H
#define CODER_H

#include "../definitions.h"
    
uint8 getParity(uint16 word);
uint16 encodeBCH(uint16 data);
uint16 decodeBCH(uint16 segment, bool *error);


#endif /* CODER_H */

