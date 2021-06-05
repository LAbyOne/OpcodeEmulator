//
//  aesins.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/22.
//  Copyright © 2019 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef aesins_hpp
#define aesins_hpp

#include "opemu.hpp"

unsigned char GF28_Multi(unsigned char a, unsigned char b);
void MixColumns(XMM *block);
void InvMixColumns(XMM *block);

void SubBytes(XMM *block);
void InvSubBytes(XMM *block);

void ShiftRows(XMM *block);
void InvShiftRows(XMM *block);

uint32_t SubWord(uint32_t X);
uint32_t RotWord(uint32_t X);

#endif /* aesins_hpp */
