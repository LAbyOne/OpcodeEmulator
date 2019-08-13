//
//  f16c.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/2.
//  Copyright (c) 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "f16c.hpp"

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
                     int kernel_trap)
{
    uint8_t imm;
    //uint8_t modreg = (*modrm >> 3) & 0x7;
    uint8_t mod = *modrm >> 6; // ModRM.mod
    uint8_t num_dst = (*modrm >> 3) & 0x7;
    uint8_t num_src = *modrm & 0x7;
    
    
    if (high_reg) num_dst += 8;
    if (high_base) num_src += 8;
    
    uint64_t rmaddrs = 0;
    
    if (reg_size == 128) {
        XMM xmmsrc, xmmvsrc, xmmdst, xmmres;
        
        uint16_t rm_size = reg_size;
        
        get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
        
        int consumed = get_consumed(modrm);
        imm = *((uint8_t*)&bytep[consumed]);
        ins_size += consumed;
        
        switch(opcode) {
            case 0x13: //vcvtph2ps
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vcvtph2ps128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
                
            case 0x1D: //vcvtps2ph
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vcvtps2ph128(xmmdst, &xmmres, imm);
                        if (mod == 3) {
                            _load_xmm(num_src, &xmmres);
                        } else {
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                        ins_size++;
                    }
                }
                break;
                
            default: return 0;
        }
    } else if (reg_size == 256) {
        YMM ymmsrc, ymmvsrc, ymmdst, ymmres;
        XMM xmmsrc, xmmres;

        int consumed = get_consumed(modrm);
        imm = *((uint8_t*)&bytep[consumed]);
        ins_size += consumed;

        switch(opcode) {
            case 0x13: //vcvtph2ps
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        uint16_t rm_size = reg_size / 2;
                        get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                        
                        vcvtph2ps256(xmmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;

            case 0x1D: //vcvtps2ph
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        uint16_t rm_size = reg_size;
                        get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                        
                        vcvtps2ph256(ymmdst, &ymmres, imm);
                        if (mod == 3) {
                            xmmres.u128 = ymmres.u128[0];
                            _load_xmm(num_src, &xmmres);
                        } else {
                            _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                        }
                        ins_size++;
                    }
                }
                break;
                
            default: return 0;
        }
    } else {
        return 0;
    }

    return ins_size;
    
}

