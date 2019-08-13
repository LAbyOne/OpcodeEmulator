//
//  vsse42.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/8.
//  Copyright © 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "vsse42.hpp"

int vsse42_instruction(x86_saved_state_t *state,
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
    
    if (reg_size == 128) {
        XMM xmmsrc, xmmvsrc, xmmdst, xmmres;
        uint16_t rm_size = reg_size;
        
        get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
        
        int consumed = get_consumed(modrm);
        imm = *((uint8_t*)&bytep[consumed]);
        ins_size += consumed;
        
        switch(opcode) {
            case 0x37:
                //VPCMPGTQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpcmpgtq_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;

            case 0x60:
                //VPCMPESTRM
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        pcmpestrm(xmmsrc, xmmdst, imm, state);
                        ins_size++;
                    }
                }
                break;
            case 0x61:
                //VPCMPESTRI
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        pcmpestri(xmmsrc, xmmdst, imm, state);
                        ins_size++;
                    }
                }
                break;
            case 0x62:
                //VPCMPISTRM
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        pcmpistrm(xmmsrc, xmmdst, imm, state);
                        ins_size++;
                    }
                }
                break;
            case 0x63:
                //VPCMPISTRI
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        pcmpistri(xmmsrc, xmmdst, imm, state);
                        ins_size++;
                    }
                }
                break;

            default: return 0;
        }
        
    } else if (reg_size == 256) {
        YMM ymmsrc, ymmvsrc, ymmdst, ymmres;
        uint16_t rm_size = reg_size;
        
        get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
        
        int consumed = get_consumed(modrm);
        imm = *((uint8_t*)&bytep[consumed]);
        ins_size += consumed;
        
        switch(opcode) {
            case 0x37:
                //VPCMPGTQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpcmpgtq_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
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

