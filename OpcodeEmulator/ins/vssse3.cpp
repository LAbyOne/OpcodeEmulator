//
//  vssse3.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/8.
//  Copyright © 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "vssse3.hpp"

int vssse3_instruction(x86_saved_state_t *state,
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
            case 0x00:
                //VPSHUFB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpshufb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x01:
                //VPHADDW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphaddw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x02:
                //VPHADDD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphaddd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x03:
                //VPHADDSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphaddsw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x04:
                //VPMADDUBSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmaddubsw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x05:
                //VPHSUBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphsubw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x06:
                //VPHSUBD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphsubd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x07:
                //VPHSUBSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphsubsw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x08:
                //VPSIGNB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        xmmres = xmmdst;
                        vpsignb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x09:
                //VPSIGNW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        xmmres = xmmdst;
                        vpsignw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x0A:
                //VPSIGND
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        xmmres = xmmdst;
                        vpsignd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x0B:
                //VPMULHRSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmulhrsw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x1C:
                //VPABSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpabsb_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x1D:
                //VPABSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpabsw_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x1E:
                //VPABSD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpabsd_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x0F:
                //VPALIGNR
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vpalignr_128(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
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
            case 0x00:
                //VPSHUFB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpshufb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x01:
                //VPHADDW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphaddw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x02:
                //VPHADDD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphaddd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x03:
                //VPHADDSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphaddsw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x04:
                //VPMADDUBSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmaddubsw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x05:
                //VPHSUBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphsubw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x06:
                //VPHSUBD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphsubd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x07:
                //VPHSUBSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vphsubsw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x08:
                //VPSIGNB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        ymmres = ymmdst;
                        vpsignb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x09:
                //VPSIGNW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        ymmres = ymmdst;
                        vpsignw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x0A:
                //VPSIGND
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        ymmres = ymmdst;
                        vpsignd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x0B:
                //VPMULHRSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmulhrsw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x1C:
                //VPABSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpabsb_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x1D:
                //VPABSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpabsw_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x1E:
                //VPABSD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpabsd_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x0F:
                //VPALIGNR
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vpalignr_256(ymmsrc, ymmvsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
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
