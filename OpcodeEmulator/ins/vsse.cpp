//
//  vsse.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/2.
//  Copyright (c) 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "vsse.hpp"

int vsse_instruction(x86_saved_state_t *state,
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
    uint8_t mod = *modrm >> 6; // ModRM.mod
    uint8_t modreg = (*modrm >> 3) & 0x7;
    uint8_t num_dst = (*modrm >> 3) & 0x7;
    uint8_t num_src = *modrm & 0x7;
    
    //get mxcsr round control
    int mxcsr_rc = getmxcsr();

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
            /************* Move *************/
            case 0x10:
                //VMOVUPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovups_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                // VMOVSS SRC -> DST
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        if (mod == 3)
                            vmovss_128a(xmmsrc, xmmvsrc, &xmmres);
                        else
                            vmovss_128b(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
                
            case 0x11:
                //VMOVUPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovups_128(xmmdst, &xmmres);
                        if (mod == 3) {
                            _load_xmm(num_src, &xmmres);
                        } else {
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                    }
                }
                // VMOVSS DST -> SRC
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        if (mod == 3) {
                            vmovss_128a(xmmdst, xmmvsrc, &xmmres);
                            _load_xmm(num_src, &xmmres);
                        } else {
                            vmovss_128b(xmmdst, &xmmres);
                            rm_size = 32;
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                    }
                }
               break;
            case 0x12:
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        //VMOVLPS SRC -> DST
                        if (mod == 0)
                            vmovlps_128a(xmmsrc, xmmvsrc, &xmmres);
                       //VMOVHLPS
                        if (mod == 3)
                            vmovhlps(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
            case 0x13: //VMOVLPS DST -> SRC
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovlps_128b(xmmdst, &xmmres);
                        rm_size = 64;
                        _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                    }
                }
            case 0x16:
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        //VMOVHPS SRC -> DST
                        if (mod == 0)
                            vmovhps_128a(xmmsrc, xmmvsrc, &xmmres);
                        //VMOVLHPS
                        if (mod == 3)
                            vmovlhps(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
            case 0x17: //VMOVHPS DST -> SRC
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovhps_128b(xmmdst, &xmmres);
                        rm_size = 64;
                        _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                    }
                }

            case 0x28: //VMOVAPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovups_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
                
            case 0x29: //VMOVAPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovups_128(xmmsrc, &xmmres);
                        if (mod == 3) {
                            _load_xmm(num_src, &xmmres);
                        } else {
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                    }
                }
                break;
            case 0x2B: //VMOVNTPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovntps_128(xmmdst, &xmmres);
                        _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                    }
                }
                break;
            case 0x50: //VMOVMSKPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovmskps_128(xmmsrc, &xmmres);
                        if (is_saved_state64(state)) {
                            M64 r64res;
                            r64res.u64 = xmmres.u64[0];
                            _load_m64(num_dst, &r64res, state);
                        } else {
                            M32 r32res;
                            r32res.u32 = xmmres.u32[0];
                            _load_m32(num_dst, &r32res, state);
                        }
                    }
                }
                break;
            /************* ADD *************/
            case 0x58:
                //VADDPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vaddps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VADDSS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vaddss(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* SUB *************/
            case 0x5C:
                //VSUBPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vsubps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VSUBSS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vsubss(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Multiply *************/
            case 0x59:
                //VMULPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmulps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VMULSS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vmulss(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Divide *************/
            case 0x5E:
                //VDIVPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vdivps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VDIVSS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vdivss(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* AND *************/
            case 0x54: //VANDPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vandps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x55: //VANDNPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vandnps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* OR/XOR *************/
            case 0x56: //VORPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vorps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x57: //VXORPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vxorps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Converts *************/
            case 0x2A:
                //VCVTSI2SS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        if (mod == 3) {
                            M64 r64src;
                            _store_m64(num_src, &r64src, state);
                            xmmsrc.u64[0] = r64src.u64;
                        }
                        vcvtsi2ss(xmmsrc, xmmvsrc, &xmmres, operand_size);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x2C:
                //VCVTTSS2SI
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vcvtss2si(xmmsrc, &xmmres, 3, operand_size);
                        M64 m64res;
                        m64res.u64 = xmmres.u64[0];
                        _load_m64(num_dst, &m64res, state);
                    }
                }
                break;
            case 0x2D:
                //VCVTSS2SI
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vcvtss2si(xmmsrc, &xmmres, mxcsr_rc, operand_size);
                        M64 m64res;
                        m64res.u64 = xmmres.u64[0];
                        _load_m64(num_dst, &m64res, state);
                    }
                }
                break;
            /************* MAX/MIN Return *************/
            case 0x5D:
                //VMINPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vminps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VMINSS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vminss(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x5F:
                //VMAXPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmaxps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VMAXSS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vmaxss(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Compare *************/
            case 0x2E: //VUCOMISS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vucomiss(xmmsrc, xmmdst, state);
                    }
                }
                break;
            case 0x2F: //VCOMISS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vucomiss(xmmsrc, xmmdst, state);
                    }
                }
                break;
            case 0xC2:
                //VCMPPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vcmpps_128(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                //VCMPSS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vcmpss(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            /************* Interleave *************/
            case 0x14: //VUNPCKLPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vunpcklps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x15: //VUNPCKHPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vunpckhps_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Select *************/
            case 0xC6: //VSHUFPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vshufps_128(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
                
            /************* Computes *************/
            case 0x53:
                //VRCPPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vrcpps_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VRCPSS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vrcpss(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x52:
                //VRSQRTPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vrsqrtps_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VRSQRTSS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vrsqrtss(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x51:
                //VSQRTPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vsqrtps_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VSQRTSS
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vsqrtss(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* MXCSR register *************/
            case 0xAE:
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        //VLDMXCSR
                        if (modreg == 2) {
                            vldmxcsr(xmmsrc);
                        }
                        //VSTMXCSR
                        if (modreg == 3) {
                            vstmxcsr(&xmmres);
                            rm_size = 32;
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
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
            /************* Move *************/
            case 0x10:
                //VMOVUPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovups_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
                
            case 0x11:
                //VMOVUPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovups_256(ymmsrc, &ymmres);
                        if (mod == 3) {
                            _load_ymm(num_src, &ymmres);
                        } else {
                            _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                        }
                    }
                }
                break;

            case 0x28: //VMOVAPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovups_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
                
            case 0x29: //VMOVAPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovups_256(ymmsrc, &ymmres);
                        if (mod == 3) {
                            _load_ymm(num_src, &ymmres);
                        } else {
                            _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                        }
                    }
                }
                break;
            case 0x2B: //VMOVNTPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovntps_256(ymmdst, &ymmres);
                        _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                    }
                }
                break;
            case 0x50: //VMOVMSKPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmovmskps_256(ymmsrc, &ymmres);
                        if (is_saved_state64(state)) {
                            M64 r64res;
                            r64res.u64 = ymmres.u64[0];
                            _load_m64(num_dst, &r64res, state);
                        } else {
                            M32 r32res;
                            r32res.u32 = ymmres.u32[0];
                            _load_m32(num_dst, &r32res, state);
                        }
                    }
                }
                break;
            /************* ADD *************/
            case 0x58: //VADDPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vaddps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;

            /************* SUB *************/
            case 0x5C: //VSUBPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vsubps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Multiply *************/
            case 0x59: //VMULPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmulps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Divide *************/
            case 0x5E: //VDIVPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vdivps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* AND *************/
            case 0x54: //VANDPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vandps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x55: //VANDNPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vandnps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;

            /************* OR/XOR *************/
            case 0x56: //VORPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vorps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x57: //VXORPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vxorps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* MAX/MIN Return *************/
            case 0x5D:
                //VMINPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vminps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x5F:
                //VMAXPS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vmaxps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Compare *************/
            case 0xC2: //VCMPPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vcmpps_256(ymmsrc, ymmvsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            /************* Interleave *************/
            case 0x14: //VUNPCKLPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vunpcklps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x15: //VUNPCKHPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vunpckhps_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Select *************/
            case 0xC6: //VSHUFPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vshufps_256(ymmsrc, ymmvsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            /************* Computes *************/
            case 0x53: //VRCPPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vrcpps_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x52: //VRSQRTPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vrsqrtps_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x51: //VSQRTPS
                if (simd_prefix == 0) { //none
                    if (leading_opcode == 1) {//0F
                        vsqrtps_256(ymmsrc, &ymmres);
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

/*********************************************************/
int maxsf(float SRC1, float SRC2) {
    //return 1 is SRC1
    //return 0 is SRC2
    if ((SRC1 == 0.0) && (SRC2 == 0.0)) return 0;
    if (isNaN_f32(SRC1)) return 0;
    if (isNaN_f32(SRC2)) return 0;
    if (SRC1 > SRC2) return 1;
    
    return 0;
}

int minsf(float SRC1, float SRC2) {
    //return 1 is SRC1
    //return 0 is SRC2
    if ((SRC1 == 0.0) && (SRC2 == 0.0)) return 0;
    if (isNaN_f32(SRC1)) return 0;
    if (isNaN_f32(SRC2)) return 0;
    if (SRC1 < SRC2) return 1;
    
    return 0;
}

//comparison predicate operand
int cmpoptsf(float SRC1, float SRC2, uint8_t immvar) {
    int opt = 0;
    
    switch(immvar) {
        case 0:
            if (SRC1 == SRC2) opt = 1;
            break;
        case 1:
            if (SRC1 < SRC2) opt = 1;
            break;
        case 2:
            if (SRC1 <= SRC2) opt = 1;
            break;
        case 3:
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 4:
            if (SRC1 != SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 5:
            if (SRC1 >= SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 6:
            if (SRC1 > SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 7:
            opt = 1;
            if (isNaN_f32(SRC1)) opt = 0;
            if (isNaN_f32(SRC2)) opt = 0;
            break;
        case 8:
            if (SRC1 == SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 9:
            if (SRC1 < SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 10:
            if (SRC1 <= SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 11:
            opt = 0;
            break;
        case 12:
            if (SRC1 != SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 0;
            if (isNaN_f32(SRC2)) opt = 0;
            break;
        case 13:
            if (SRC1 >= SRC2) opt = 1;
            break;
        case 14:
            if (SRC1 > SRC2) opt = 1;
            break;
        case 15:
            opt = 1;
            break;
        case 16:
            if (SRC1 == SRC2) opt = 1;
            break;
        case 17:
            if (SRC1 < SRC2) opt = 1;
            break;
        case 18:
            if (SRC1 <= SRC2) opt = 1;
            break;
        case 19:
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 20:
            if (SRC1 != SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 21:
            if (SRC1 >= SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 22:
            if (SRC1 > SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 23:
            opt = 1;
            if (isNaN_f32(SRC1)) opt = 0;
            if (isNaN_f32(SRC2)) opt = 0;
            break;
        case 24:
            if (SRC1 == SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 25:
            if (SRC1 < SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 26:
            if (SRC1 <= SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 1;
            if (isNaN_f32(SRC2)) opt = 1;
            break;
        case 27:
            opt = 0;
            break;
        case 28:
            if (SRC1 != SRC2) opt = 1;
            if (isNaN_f32(SRC1)) opt = 0;
            if (isNaN_f32(SRC2)) opt = 0;
            break;
        case 29:
            if (SRC1 >= SRC2) opt = 1;
            break;
        case 30:
            if (SRC1 > SRC2) opt = 1;
            break;
        case 31:
            opt = 1;
            break;
        default: return 0;
    }
    
    return opt;
}

/****** get approximate ******/
float rcp_sf(float fp32) {
    XMM BAK;
    XMM SRC;
    XMM RES;
    
    //backup xmm
    asm __volatile__ ("movdqu %%xmm0, %0" : "=m" (BAK));
    
    //get approximate rcp_sf
    SRC.fa32[0] = fp32;
    asm __volatile__ ("rcpss %0, %%xmm0" :: "m" (SRC));
    asm __volatile__ ("movdqu %%xmm0, %0" : "=m" (RES));
    float f = RES.fa32[0];
    
    //restore xmm
    asm __volatile__ ("movdqu %0, %%xmm0" :: "m" (BAK));
    
    return f;
}

/****** get approximate(sqrt) ******/
float rsqrt_sf(float fp32) {
    XMM BAK;
    XMM SRC;
    XMM RES;
    
    //backup xmm
    asm __volatile__ ("movdqu %%xmm0, %0" : "=m" (BAK));
    
    //get approximate(sqrt) rsqrt_sf
    SRC.fa32[0] = fp32;
    asm __volatile__ ("rsqrtss %0, %%xmm0" :: "m" (SRC));
    asm __volatile__ ("movdqu %%xmm0, %0" : "=m" (RES));
    float f = RES.fa32[0];
    
    //restore xmm
    asm __volatile__ ("movdqu %0, %%xmm0" :: "m" (BAK));
    
    return f;
}
