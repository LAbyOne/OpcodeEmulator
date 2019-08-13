//
//  f16c.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/2.
//  Copyright (c) 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef f16c_hpp
#define f16c_hpp

#include "opemu.hpp"
#include "half.hpp"
#include "fpins.hpp"

int f16c_instruction(x86_saved_state_t *state,
                     uint8_t vexreg,
                     uint8_t opcode,
                     uint8_t *modrm,
                     uint8_t high_reg,
                     uint8_t high_index,
                     uint8_t high_base,
                     uint16_t reg_size,
                     uint8_t leading_opcode,
                     uint8_t simd_prefix,
                     uint8_t *bytep,
                     uint8_t ins_size,
                     uint8_t modbyte,
                     int kernel_trap);

/**********************************************/
/**  F16C instructions implementation       **/
/**********************************************/
inline void vcvtph2ps128(XMM src, XMM *res) {
    int i;
    uint16_t f16 = 0;
    uint32_t f32 = 0;
    XMM tmp;
    
    for (i = 0; i < 4; ++i) {
        f16 = src.u16[i];
        f32 = HalfToFloat(f16);
        tmp.u32[i] = f32;
        res->fa32[i] = tmp.fa32[i];
    }
}
inline void vcvtph2ps256(XMM src, YMM *res) {
    int i;
    uint16_t f16 = 0;
    uint32_t f32 = 0;
    YMM tmp;
    
    for (i = 0; i < 8; ++i) {
        f16 = src.u16[i];
        f32 = HalfToFloat(f16);
        tmp.u32[i] = f32;
        res->fa32[i] = tmp.fa32[i];
    }
}

inline void vcvtps2ph128(XMM dst, XMM *res, uint8_t imm) {
    int i;
    uint32_t f32 = 0;
    uint16_t f16 = 0;
    int RC = 0;
    
    int MS1 = (imm >> 2) & 1;
    if (MS1 == 0) {
        RC = imm & 3;
    } else {
        //get mxcsr round control
        RC = getmxcsr();
    }
    
    for (i = 0; i < 4; ++i) {
        f32 = dst.u32[i];
        f16 = FloatToHalf(f32);
        res->u16[i] = f16;
    }
}
inline void vcvtps2ph256(YMM dst, YMM *res, uint8_t imm) {
    int i;
    uint32_t f32 = 0;
    uint16_t f16 = 0;
    int RC = 0;
    res->u128[1] = 0;
    
    int MS1 = (imm >> 2) & 1;
    if (MS1 == 0) {
        RC = imm & 3;
    } else {
        //get mxcsr round control
        RC = getmxcsr();
    }
    
    for (i = 0; i < 8; ++i) {
        f32 = dst.u32[i];
        f16 = FloatToHalf(f32);
        res->u16[i] = f16;
    }
}

#endif /* f16c_hpp */
