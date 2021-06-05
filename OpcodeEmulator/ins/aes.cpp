//
//  aes.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/22.
//  Copyright © 2019 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "aes.hpp"

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
                    int kernel_trap)
{
    uint8_t imm;
    //uint8_t mod = *modrm >> 6; // ModRM.mod
    uint8_t num_dst = (*modrm >> 3) & 0x7;
    uint8_t num_src = *modrm & 0x7;
    
    if (high_reg) num_dst += 8;
    if (high_base) num_src += 8;
    
    uint64_t rmaddrs = 0;
    
    XMM xmmsrc, xmmdst, xmmres;
    uint16_t rm_size = reg_size;
    
    get_rexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmdst, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
    
    int consumed = get_consumed(modrm);
    imm = *((uint8_t*)&bytep[consumed]);
    ins_size += consumed;

    switch(opcode) {
        case 0xDB: //aesimc
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) {//0F38
                    aesimc(xmmsrc, &xmmres);
                    _load_xmm(num_dst, &xmmres);
                }
            }
            break;
            
        case 0xDC: //aesenc
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) {//0F38
                    //round key (key) = mod.r/m (src)
                    //state (data) = mod.reg (dst) / vex.v (vsrc)
                    aesenc(xmmsrc, xmmdst, &xmmres);
                    _load_xmm(num_dst, &xmmres);
                }
            }
            break;
            
        case 0xDD: //aesenclast
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) {//0F38
                    //round key (key) = mod.r/m (src)
                    //state (data) = mod.reg (dst) / vex.v (vsrc)
                    aesenclast(xmmsrc, xmmdst, &xmmres);
                    _load_xmm(num_dst, &xmmres);
                }
            }
            break;

        case 0xDE: //aesdec
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) {//0F38
                    //round key (key) = mod.r/m (src)
                    //state (data) = mod.reg (dst) / vex.v (vsrc)
                    aesdec(xmmsrc, xmmdst, &xmmres);
                    _load_xmm(num_dst, &xmmres);
                }
            }
            break;
            
        case 0xDF: //aesdeclast / aeskeygenassist
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) {//0F38
                    //round key (key) = mod.r/m (src)
                    //state (data) = mod.reg (dst) / vex.v (vsrc)
                    aesdeclast(xmmsrc, xmmdst, &xmmres);
                    _load_xmm(num_dst, &xmmres);
                }
                if (leading_opcode == 3) {//0F3A
                    aeskeygenassist(xmmsrc, &xmmres, imm);
                    _load_xmm(num_dst, &xmmres);
                    ins_size++;
                }

            }
            break;
            
        case 0x44: //pclmulqdq
            if (simd_prefix == 1) { //66
                if (leading_opcode == 3) {//0F3A
                    //SRC1 = mod.reg (dst) / vex.v (vsrc)
                    //SRC2 = mod.r/m (src)
                    pclmulqdq_128(xmmsrc, xmmdst, &xmmres, imm);
                    _load_xmm(num_dst, &xmmres);
                    ins_size++;
                }
            }
            break;

        default: return 0;
    }

    return ins_size;
}

// ==================================================================================== //
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
                       int kernel_trap)
{
    uint8_t imm;
    //uint8_t mod = *modrm >> 6; // ModRM.mod
    //uint8_t modreg = (*modrm >> 3) & 0x7;
    uint8_t num_dst = (*modrm >> 3) & 0x7;
    uint8_t num_src = *modrm & 0x7;
    
    if (high_reg) num_dst += 8;
    if (high_base) num_src += 8;
    
    uint64_t rmaddrs = 0;
    
    XMM xmmsrc, xmmvsrc, xmmdst, xmmres;
    YMM ymmsrc, ymmvsrc, ymmdst, ymmres;
    
    uint16_t rm_size = reg_size;
    
    get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
    
    int consumed = get_consumed(modrm);
    imm = *((uint8_t*)&bytep[consumed]);
    ins_size += consumed;
    
    switch(opcode) {
        case 0xDB: //vaesimc
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) {//0F38
                    aesimc(xmmsrc, &xmmres);
                    _load_xmm(num_dst, &xmmres);
                }
            }
            break;
            
        case 0xDC: //vaesenc
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) {//0F38
                    //round key (key) = mod.r/m (src)
                    //state (data) = mod.reg (dst) / vex.v (vsrc)
                    aesenc(xmmsrc, xmmvsrc, &xmmres);
                    _load_xmm(num_dst, &xmmres);
                }
            }
            break;
            
        case 0xDD: //vaesenclast
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) {//0F38
                    //round key (key) = mod.r/m (src)
                    //state (data) = mod.reg (dst) / vex.v (vsrc)
                    aesenclast(xmmsrc, xmmvsrc, &xmmres);
                    _load_xmm(num_dst, &xmmres);
                }
            }
            break;
            
        case 0xDE: //vaesdec
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) {//0F38
                    //round key (key) = mod.r/m (src)
                    //state (data) = mod.reg (dst) / vex.v (vsrc)
                    aesdec(xmmsrc, xmmvsrc, &xmmres);
                    _load_xmm(num_dst, &xmmres);
                }
            }
            break;
            
        case 0xDF: //vaesdeclast / vaeskeygenassist
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) {//0F38
                    //round key (key) = mod.r/m (src)
                    //state (data) = mod.reg (dst) / vex.v (vsrc)
                    aesdeclast(xmmsrc, xmmvsrc, &xmmres);
                    _load_xmm(num_dst, &xmmres);
                }
                if (leading_opcode == 3) {//0F3A
                    aeskeygenassist(xmmsrc, &xmmres, imm);
                    _load_xmm(num_dst, &xmmres);
                    ins_size++;
                }
                
            }
            break;
            
        case 0x44: //vpclmulqdq
            if (simd_prefix == 1) { //66
                if (leading_opcode == 3) {//0F3A
                    if (reg_size == 128) {
                        //SRC1 = mod.reg (dst) / vex.v (vsrc)
                        //SRC2 = mod.r/m (src)
                        pclmulqdq_128(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    } else {
                        uint16_t rm_size = reg_size;
                        get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);

                        //SRC1 = mod.reg (dst) / vex.v (vsrc)
                        //SRC2 = mod.r/m (src)
                        pclmulqdq_256(ymmsrc, ymmvsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
            }
            break;

        default: return 0;
    }

    return ins_size;
}

// ==================================================================================== //
__uint128_t cl_mul(__uint128_t a, __uint128_t b) {
    __uint128_t r = 0;
    while (b != 0)
    {
        if ((b & 1) != 0)
            r ^= a;
        a <<= 1;
        b >>= 1;
    }
    return r;
}
