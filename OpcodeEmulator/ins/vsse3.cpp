//
//  vsse3.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/8.
//  Copyright © 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "vsse3.hpp"

int vsse3_instruction(x86_saved_state_t *state,
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
            case 0x12:
                //VMOVSLDUP
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vmovsldup_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VMOVDDUP
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vmovddup_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x16:
                //VMOVSHDUP
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vmovshdup_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xF0:
                //VLDDQU
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vlddqu_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x7C:
                //VHADDPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vhaddpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VHADDPS
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vhaddps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x7D:
                //VHSUBPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vhsubpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VHSUBPS
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vhsubps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xD0:
                //VADDSUBPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vaddsubpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VADDSUBPS
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vaddsubps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
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
            case 0x12:
                //VMOVSLDUP
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vmovsldup_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                //VMOVDDUP
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vmovddup_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x16:
                //VMOVSHDUP
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vmovshdup_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xF0:
                //VLDDQU
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vlddqu_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x7C:
                //VHADDPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vhaddpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                //VHADDPS
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vhaddps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x7D:
                //VHSUBPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vhsubpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                //VHSUBPS
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vhsubps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xD0:
                //VADDSUBPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vaddsubpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                //VADDSUBPS
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vaddsubps_256(ymmsrc, ymmvsrc, &ymmres);
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
