//
//  aes.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/22.
//  Copyright © 2019 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef aes_hpp
#define aes_hpp

#include "opemu.hpp"
#include "aesins.hpp"

int aes_instruction(x86_saved_state_t *state,
                     uint8_t opcode,
                     uint8_t *modrm,
                     uint8_t high_reg,
                     uint8_t high_index,
                     uint8_t high_base,
                     uint16_t reg_size,
                     uint8_t operand_size,
                     uint8_t leading_opcode,
                     uint8_t simd_prefix,
                     uint8_t *bytep,
                     uint8_t ins_size,
                     uint8_t modbyte,
                     int kernel_trap);

int vaes_instruction(x86_saved_state_t *state,
                     uint8_t vexreg,
                     uint8_t opcode,
                     uint8_t *modrm,
                     uint8_t high_reg,
                     uint8_t high_index,
                     uint8_t high_base,
                     uint16_t reg_size,
                     uint8_t operand_size,
                     uint8_t leading_opcode,
                     uint8_t simd_prefix,
                     uint8_t *bytep,
                     uint8_t ins_size,
                     uint8_t modbyte,
                     int kernel_trap);

__uint128_t cl_mul(__uint128_t a, __uint128_t b);

/**********************************************/
/**  AES instructions implementation         **/
/**********************************************/
inline void aesimc(XMM src, XMM *res) {
    XMM block;
    block.u128 = src.u128;
    InvMixColumns(&block);

    res->u128 = block.u128;
}

//round key (key) = mod.r/m (src)
//state (data) = mod.reg (dst) / vex.v (vsrc)
inline void aesenc(XMM key, XMM data, XMM *res) {
    XMM STATE, RoundKey;
    STATE = data;
    RoundKey = key;
    
    ShiftRows(&STATE);
    SubBytes(&STATE);
    MixColumns(&STATE);
    
    res->u128 = STATE.u128 ^ RoundKey.u128;
}

//round key (key) = mod.r/m (src)
//state (data) = mod.reg (dst) / vex.v (vsrc)
inline void aesenclast(XMM key, XMM data, XMM *res) {
    XMM STATE, RoundKey;
    STATE = data;
    RoundKey = key;
    
    ShiftRows(&STATE);
    SubBytes(&STATE);
    
    res->u128 = STATE.u128 ^ RoundKey.u128;
}

//round key (key) = mod.r/m (src)
//state (data) = mod.reg (dst) / vex.v (vsrc)
inline void aesdec(XMM key, XMM data, XMM *res) {
    XMM STATE, RoundKey;
    STATE = data;
    RoundKey = key;
    
    InvShiftRows(&STATE);
    InvSubBytes(&STATE);
    InvMixColumns(&STATE);
    
    res->u128 = STATE.u128 ^ RoundKey.u128;
}

//round key (key) = mod.r/m (src)
//state (data) = mod.reg (dst) / vex.v (vsrc)
inline void aesdeclast(XMM key, XMM data, XMM *res) {
    XMM STATE, RoundKey;
    STATE = data;
    RoundKey = key;
    
    InvShiftRows(&STATE);
    InvSubBytes(&STATE);
    
    res->u128 = STATE.u128 ^ RoundKey.u128;
}

inline void aeskeygenassist(XMM src, XMM *res, uint8_t imm) {
    uint32_t X0=0, X1=0, X2=0, X3=0, R2=0, R4=0, T2=0, T4=0;
    uint32_t RCON = imm;

    X0 = src.u32[0];
    X1 = src.u32[1];
    X2 = src.u32[2];
    X3 = src.u32[3];

    X1 = SubWord(X1);
    
    T2 = RotWord(X1);
    R2 = T2 ^ RCON;
    
    X3 = SubWord(X3);
    
    T4 = RotWord(X3);
    R4 = T4 ^ RCON;
    
    res->u32[0] = X1;
    res->u32[1] = R2;
    res->u32[2] = X3;
    res->u32[3] = R4;
}

/**********************************************/
/**  pclmulqdq instructions implementation   **/
/**********************************************/
//SRC1 = mod.reg (dst) / vex.v (vsrc)
//SRC2 = mod.r/m (src)
inline void pclmulqdq_128(XMM src2, XMM src1, XMM *res, uint8_t imm) {
    int imm0 = imm & 1;
    int imm4 = (imm >> 4) & 1;
    __uint128_t SRC1, SRC2, DEST;
    
    SRC1 = src1.u64[imm0];
    SRC2 = src2.u64[imm4];
    DEST = cl_mul(SRC1, SRC2);
    res->u128 = DEST;
}
inline void pclmulqdq_256(YMM src2, YMM src1, YMM *res, uint8_t imm) {
    int imm0 = imm & 1;
    int imm4 = (imm >> 4) & 1;
    __uint128_t SRC1, SRC2, DEST;
    
    //[127:0] bit
    SRC1 = src1.u64[imm0];
    SRC2 = src2.u64[imm4];
    DEST = cl_mul(SRC1, SRC2);
    res->u128[0] = DEST;
    
    //[255:128] bit
    SRC1 = src1.u64[imm0+2];
    SRC2 = src2.u64[imm4+2];
    DEST = cl_mul(SRC1, SRC2);
    res->u128[1] = DEST;
}

#endif /* aes_hpp */
