//
//  vsse2.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/8.
//  Copyright © 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "vsse2.hpp"

int vsse2_instruction(x86_saved_state_t *state,
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
    uint8_t mod = *modrm >> 6;
    uint8_t modreg = (*modrm >> 3) & 0x7;
    uint8_t num_dst = (*modrm >> 3) & 0x7;
    uint8_t num_src = *modrm & 0x7;
    
    if (high_reg) num_dst += 8;
    if (high_base) num_src += 8;
    
    uint64_t rmaddrs = 0;
    
    //get mxcsr round control
    int mxcsr_rc = getmxcsr();

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
                //VMOVUPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovupd_128a(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VMOVSD
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        if (mod == 3) {
                            vmovsd_128a(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        } else {
                            vmovsd_128b(xmmsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
                
            case 0x11:
                //VMOVUPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovupd_128b(xmmsrc, &xmmres);
                        if (mod == 3) {
                            _load_xmm(num_src, &xmmres);
                        } else {
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                    }
                }
                //VMOVSD
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        if (mod == 3) {
                            vmovsd_128a(xmmdst, xmmvsrc, &xmmres);
                            _load_xmm(num_src, &xmmres);
                        } else {
                            vmovsd_128b(xmmdst, &xmmres);
                            rm_size = 64;
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                    }
                }
                break;

            case 0x12: //VMOVLPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovlpd_128a(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x13: //VMOVLPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovlpd_128b(xmmdst, &xmmres);
                        rm_size = 64;
                        _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                    }
                }
                break;

            case 0x16: //VMOVHPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovhpd_128a(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x17: //VMOVHPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovhpd_128b(xmmdst, &xmmres);
                        rm_size = 64;
                        _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                    }
                }
                break;

            case 0x28: //VMOVAPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovupd_128a(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
                
            case 0x29: //VMOVAPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovupd_128b(xmmsrc, &xmmres);
                        if (mod == 3) {
                            _load_xmm(num_src, &xmmres);
                        } else {
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                    }
                }
                break;

            case 0x6E: //VMOVD/VMOVQ DST <- SRC
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        M64 tmp;
                        _store_m64(num_src, &tmp, state);
                        xmmsrc.u64[0] = tmp.u64;
                        vmovda(xmmsrc, &xmmres, operand_size);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
                
            case 0x7E: //VMOVD/VMOVQ SRC <- DST
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovdb(xmmdst, &xmmres, operand_size);
                        if (mod == 3) {
                            M64 r64res;
                            r64res.u64 = xmmres.u64[0];
                            _load_m64(num_src, &r64res, state);
                        } else {
                            rm_size = operand_size;
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                    }
                }
                break;
                
            case 0x6F: //VMOVDQU/VMOVDQA DST <- SRC
                if ((simd_prefix == 1) || (simd_prefix == 2)) { //66 or F3
                    if (leading_opcode == 1) {//0F
                        vmovdqu_128a(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;

            case 0x7F: //VMOVDQU/VMOVDQA SRC <- DST
                if ((simd_prefix == 1) || (simd_prefix == 2)) { //66 or F3
                    if (leading_opcode == 1) {//0F
                        vmovdqu_128b(xmmdst, &xmmres);
                        if (mod == 3) {
                            _load_xmm(num_src, &xmmres);
                        } else {
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                    }
                }
                break;
                
            case 0xF7: //VMASKMOVDQU
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint64_t dest = saved_state64(state)->rdi; //Memory location
                        XMM var;
                        copyin(dest, (char*) &var.u128, 16);
                        vmaskmovdqu(xmmsrc, xmmdst, &xmmres, var);
                        rm_size = 128;
                       _load_maddr_from_xmm(dest, &xmmres, rm_size, state);
                    }
                }
                break;
                
            case 0x2B: //VMOVNTPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovntdq_128(xmmdst, &xmmres);
                        rm_size = 128;
                        _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                    }
                }
                break;
            case 0xE7: //VMOVNTDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovntdq_128(xmmdst, &xmmres);
                        rm_size = 128;
                        _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                    }
                }
                break;
                
            case 0x50: //VMOVMSKPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovmskpd_128(xmmsrc, &xmmres);
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
            case 0xD7: //VPMOVMSKB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmovmskb_128(xmmsrc, &xmmres);
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

           /************* Converts integers byte/word *************/
            case 0x63: //VPACKSSWB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpacksswb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x67: //VPACKUSWB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpackuswb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x6B: //VPACKSSWD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpackssdw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
                
            /************* Converts floating-point *************/
            case 0x2A:
                //VCVTSI2SD
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        if (mod == 3) {
                            M64 r64src;
                            _store_m64(num_src, &r64src, state);
                            xmmsrc.u64[0] = r64src.u64;
                        }
                        vcvtsi2sd(xmmsrc, xmmvsrc, &xmmres, operand_size);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x2C:
                //VCVTTSD2SI
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vcvtsd2si(xmmsrc, &xmmres, 3, operand_size);
                        M64 m64res;
                        m64res.u64 = xmmres.u64[0];
                        _load_m64(num_dst, &m64res, state);
                    }
                }
                break;
            case 0x2D:
                //VCVTSD2SI
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vcvtsd2si(xmmsrc, &xmmres, mxcsr_rc, operand_size);
                        M64 m64res;
                        m64res.u64 = xmmres.u64[0];
                        _load_m64(num_dst, &m64res, state);
                    }
                }
                break;
           case 0xE6:
                //VCVTTPD2DQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vcvtpd2dq_128(xmmsrc, &xmmres, 3);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VCVTDQ2PD
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vcvtdq2pd_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VCVTPD2DQ
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vcvtpd2dq_128(xmmsrc, &xmmres, mxcsr_rc);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x5A:
                //VCVTPS2PD
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vcvtps2pd_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VCVTPD2PS
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vcvtpd2ps_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VCVTSS2SD
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vcvtss2sd(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VCVTSD2SS
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vcvtsd2ss(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
               break;
            case 0x5B:
                //VCVTDQ2PS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vcvtdq2ps_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VCVTPS2DQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vcvtps2dq_128(xmmsrc, &xmmres, mxcsr_rc);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VCVTTPS2DQ
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vcvtps2dq_128(xmmsrc, &xmmres, 3);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
                
            /************* Computes *************/
            case 0x51:
                //VSQRTPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vsqrtpd_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VSQRTSD
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vsqrtsd(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xF6: //VPSADBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsadbw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Insert *************/
            case 0xC4: //VPINSRW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        if (operand_size == 32) { //W0
                            if (mod == 3) {
                                M64 r64src;
                                _store_m64(num_src, &r64src, state);
                                xmmsrc.u64[0] = r64src.u64;
                            }
                            vpinsrw(xmmsrc, xmmvsrc, &xmmres, imm);
                            _load_xmm(num_dst, &xmmres);
                            ins_size++;
                        }
                    }
                }
                break;

            /************* ADD *************/
            case 0xFC: //VPADDB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xFD: //VPADDW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xFE: //VPADDD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xD4: //VPADDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddq_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xEC: //VPADDSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddsb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xED: //VPADDSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddsw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xDC: //VPADDUSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddusb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xDD: //VPADDUSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddusw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* SUB *************/
            case 0xF8: //VPSUBB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xF9: //VPSUBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xFA: //VPSUBD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xFB: //VPSUBQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubq_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
                
            case 0xE8: //VPSUBSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubsb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xE9: //VPSUBSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubsw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xD8: //VPSUBUSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubusb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xD9: //VPSUBUSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubusw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x5C:
                //VSUBPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vsubpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VSUBSD
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vsubsd(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Multiply *************/
            case 0x59: //VMULPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmulpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VMULSD
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vmulsd(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
               break;
            case 0xF5: //VPMADDWD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmaddwd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xE4: //VPMULHUW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmulhuw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xE5: //VPMULHW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmulhw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xD5: //VPMULLW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmullw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xF4: //VPMULUDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmuludq_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Divide *************/
            case 0x5E:
                //VDIVPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vdivpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VDIVSD
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vdivsd(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* AND *************/
            case 0x54: //VANDPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vandpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x55: //VANDNPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vandnpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xDB: //VPAND
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpand_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xDF: //VPANDN
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpandn_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* OR/XOR *************/
            case 0x56: //VORPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vorpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x57: //VXORPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vxorpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xEB: //VPOR
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpor_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xEF: //VPXOR
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpxor_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Average *************/
            case 0xE0: //VPAVGB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpavgb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xE3: //VPAVGB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpavgw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Shuffle *************/
            case 0x70:
                //VPSHUFD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpshufd_128(xmmsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                //VPSHUFHW
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vpshufhw_128(xmmsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                //VPSHUFLW
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vpshuflw_128(xmmsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x71:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        //VPSRLW
                        if (modreg == 2) {
                            vpsrlw_128(xmmsrc, &xmmres, imm);
                            _load_xmm(vexreg, &xmmres);
                            ins_size++;
                        }
                        //VPSRAW
                        if (modreg == 4) {
                            vpsraw_128(xmmsrc, &xmmres, imm);
                            _load_xmm(vexreg, &xmmres);
                            ins_size++;
                        }
                        //VPSLLW
                        if (modreg == 6) {
                            vpsllw_128(xmmsrc, &xmmres, imm);
                            _load_xmm(vexreg, &xmmres);
                            ins_size++;
                        }
                    }
                }
                break;
            case 0x72:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        //VPSRLD
                        if (modreg == 2) {
                            vpsrld_128(xmmsrc, &xmmres, imm);
                            _load_xmm(vexreg, &xmmres);
                            ins_size++;
                        }
                        //VPSRAD
                        if (modreg == 4) {
                            vpsrad_128(xmmsrc, &xmmres, imm);
                            _load_xmm(vexreg, &xmmres);
                            ins_size++;
                        }
                       //VPSLLD
                        if (modreg == 6) {
                            vpslld_128(xmmsrc, &xmmres, imm);
                            _load_xmm(vexreg, &xmmres);
                            ins_size++;
                        }
                    }
                }
                break;
            case 0x73:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        //VPSRLQ
                        if (modreg == 2) {
                            vpsrlq_128(xmmsrc, &xmmres, imm);
                            _load_xmm(vexreg, &xmmres);
                            ins_size++;
                        }
                        //VPSRLDQ
                        if (modreg == 3) {
                            vpsrldq_128(xmmsrc, &xmmres, imm);
                            _load_xmm(vexreg, &xmmres);
                            ins_size++;
                        }
                        //VPSLLQ
                        if (modreg == 6) {
                            vpsllq_128(xmmsrc, &xmmres, imm);
                            _load_xmm(vexreg, &xmmres);
                            ins_size++;
                        }
                        //VPSLLDQ
                        if (modreg == 7) {
                            vpslldq_128(xmmsrc, &xmmres, imm);
                            _load_xmm(vexreg, &xmmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0xC6: //VSHUFPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vshufpd_128(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;

            case 0xD1: //VPSRLW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = xmmsrc.u8[0];
                        vpsrlw_128(xmmvsrc, &xmmres, count);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xD2: //VPSRLD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = xmmsrc.u8[0];
                        vpsrld_128(xmmvsrc, &xmmres, count);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xD3: //VPSRLQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = xmmsrc.u8[0];
                        vpsrlq_128(xmmvsrc, &xmmres, count);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xE1: //VPSRAW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = xmmsrc.u8[0];
                        vpsraw_128(xmmvsrc, &xmmres, count);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xE2: //VPSRAD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = xmmsrc.u8[0];
                        vpsrad_128(xmmvsrc, &xmmres, count);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xF1: //VPSLLW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = xmmsrc.u8[0];
                        vpsllw_128(xmmvsrc, &xmmres, count);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xF2: //VPSLLD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = xmmsrc.u8[0];
                        vpslld_128(xmmvsrc, &xmmres, count);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xF3: //VPSLLQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = xmmsrc.u8[0];
                        vpsllq_128(xmmvsrc, &xmmres, count);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Interleave *************/
            case 0x60: //VPUNPCKLBW Byte
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpcklbw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x61: //VPUNPCKLWD Words
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpcklwd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x62: //VPUNPCKLDQ Doublewords
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpckldq_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x6C: //VPUNPCKLQDQ Quadword
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpcklqdq_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x68: //VPUNPCKHBW Byte
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpckhbw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x69: //VPUNPCKHWD Words
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpckhwd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x6A: //VPUNPCKHDQ Doublewords
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpckhdq_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x6D: //VPUNPCKHQDQ Quadword
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpckhqdq_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x14: //VUNPCKLPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vunpcklpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x15: //VUNPCKHPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vunpckhpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* MAX/MIN Return *************/
            case 0x5D:
                //VMINPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vminpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VMINSD
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vminsd(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x5F:
                //VMAXPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmaxpd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VMAXSD
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vmaxsd(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
               break;
            /************* Compare *************/
            case 0x2E: //VUCOMISD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vucomusd(xmmsrc, xmmdst, state);
                    }
                }
                break;
            case 0x2F: //VCOMISD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vucomusd(xmmsrc, xmmdst, state);
                    }
                }
                break;
            case 0x64: //VPCMPGTB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpgtb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x65: //VPCMPGTW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpgtw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x66: //VPCMPGTD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpgtd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x74: //VPCMPEQB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpeqb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x75: //VPCMPEQW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpeqw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x76: //VPCMPEQD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpeqd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xC2:
                //VCMPPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vcmppd_128(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                //VCMPSD
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vcmpsd(xmmsrc, xmmvsrc, &xmmres, imm);
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
            /************* Move *************/
            case 0x10: //VMOVUPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovupd_256a(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
                
            case 0x11: //VMOVUPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovupd_256b(ymmsrc, &ymmres);
                        if (mod == 3) {
                            _load_ymm(num_src, &ymmres);
                        } else {
                            _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                        }
                    }
                }
                break;

            case 0x28: //VMOVAPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovupd_256a(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
                
            case 0x29: //VMOVAPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovupd_256b(ymmsrc, &ymmres);
                        if (mod == 3) {
                            _load_ymm(num_src, &ymmres);
                        } else {
                            _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                        }
                    }
                }
                break;

            case 0x6F: //VMOVDQU/VMOVDQA DST <- SRC
                if ((simd_prefix == 1) || (simd_prefix == 2)) { //66 or F3
                    if (leading_opcode == 1) {//0F
                        vmovdqu_256a(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
                
            case 0x7F: //VMOVDQU/VMOVDQA SRC <- DST
                if ((simd_prefix == 1) || (simd_prefix == 2)) { //66 or F3
                    if (leading_opcode == 1) {//0F
                        vmovdqu_256b(ymmdst, &ymmres);
                        if (mod == 3) {
                            _load_ymm(num_src, &ymmres);
                        } else {
                            _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                        }
                    }
                }
                break;
                
            case 0x2B: //VMOVNTPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovntdq_256(ymmdst, &ymmres);
                        rm_size = 256;
                        _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                    }
                }
                break;
            case 0xE7: //VMOVNTDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovntdq_256(ymmdst, &ymmres);
                        rm_size = 256;
                        _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                    }
                }
                break;
                
            case 0x50: //VMOVMSKPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmovmskpd_256(ymmsrc, &ymmres);
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
            case 0xD7: //VPMOVMSKB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmovmskb_256(ymmsrc, &ymmres);
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

            /************* Converts integers byte/word *************/
            case 0x63: //VPACKSSWB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpacksswb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x67: //VPACKUSWB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpackuswb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x6B: //VPACKSSWD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpackssdw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;

            /************* Converts floating-point *************/
            case 0xE6:
                //VCVTTPD2DQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        XMM xmmres;
                        vcvtpd2dq_256(ymmsrc, &xmmres, 3);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                //VCVTDQ2PD
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        XMM xmmsrc;
                        rm_size = 128;
                        get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                        vcvtdq2pd_256(xmmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                //VCVTPD2DQ
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        XMM xmmres;
                        vcvtpd2dq_256(ymmsrc, &xmmres, mxcsr_rc);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x5A:
                //VCVTPS2PD
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        XMM xmmsrc;
                        rm_size = 128;
                        get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);

                        vcvtps2pd_256(xmmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
               //VCVTPD2PS
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vcvtpd2ps_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x5B:
                //VCVTDQ2PS
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) {//0F
                        vcvtdq2ps_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                //VCVTPS2DQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vcvtps2dq_256(ymmsrc, &ymmres, mxcsr_rc);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                //VCVTTPS2DQ
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vcvtps2dq_256(ymmsrc, &ymmres, 3);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
              break;
            
            /************* Converts floating-point *************/
            case 0x51:
                //VSQRTPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vsqrtpd_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
            case 0xF6: //VPSADBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsadbw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;

            /************* ADD *************/
            case 0xD4: //VPADDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddq_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xFC: //VPADDB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xFD: //VPADDW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xFE: //VPADDD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xEC: //VPADDSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddsb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xED: //VPADDSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddsw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xDC: //VPADDUSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddusb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xDD: //VPADDUSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpaddusw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* SUB *************/
            case 0xF8: //VPSUBB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xF9: //VPSUBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xFA: //VPSUBD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xFB: //VPSUBQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubq_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xE8: //VPSUBSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubsb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xE9: //VPSUBSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubsw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xD8: //VPSUBUSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubusb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xD9: //VPSUBUSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpsubusw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x5C: //VSUBPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vsubpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Multiply *************/
            case 0x59: //VMULPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmulpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xF5: //VPMADDWD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmaddwd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xE4: //VPMULHUW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmulhuw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xE5: //VPMULHW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmulhw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xD5: //VPMULLW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmullw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xF4: //VPMULUDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmuludq_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Divide *************/
            case 0x5E: //VDIVPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vdivpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* AND *************/
            case 0x54: //VANDPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vandpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x55: //VANDNPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vandnpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xDB: //VPAND
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpand_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xDF: //VPANDN
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpandn_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* OR/XOR *************/
            case 0x56: //VORPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vorpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x57: //VXORPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vxorpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xEB: //VPOR
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpor_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xEF: //VPXOR
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpxor_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Average *************/
            case 0xE0: //VPAVGB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpavgb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xE3: //VPAVGB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpavgw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Shuffle *************/
            case 0x70:
                //VPSHUFD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpshufd_256(ymmsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                //VPSHUFHW
                if (simd_prefix == 2) { //F3
                    if (leading_opcode == 1) {//0F
                        vpshufhw_256(ymmsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                //VPSHUFLW
                if (simd_prefix == 3) { //F2
                    if (leading_opcode == 1) {//0F
                        vpshuflw_256(ymmsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
               break;
            case 0x71:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        //VPSRLW
                        if (modreg == 2) {
                            vpsrlw_256(ymmsrc, &ymmres, imm);
                            _load_ymm(vexreg, &ymmres);
                            ins_size++;
                        }
                        //VPSRAW
                        if (modreg == 4) {
                            vpsraw_256(ymmsrc, &ymmres, imm);
                            _load_ymm(vexreg, &ymmres);
                            ins_size++;
                        }
                        //VPSLLW
                        if (modreg == 6) {
                            vpsllw_256(ymmsrc, &ymmres, imm);
                            _load_ymm(vexreg, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;
            case 0x72:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        //VPSRLD
                        if (modreg == 2) {
                            vpsrld_256(ymmsrc, &ymmres, imm);
                            _load_ymm(vexreg, &ymmres);
                            ins_size++;
                        }
                        //VPSRAD
                        if (modreg == 4) {
                            vpsrad_256(ymmsrc, &ymmres, imm);
                            _load_ymm(vexreg, &ymmres);
                            ins_size++;
                        }
                        //VPSLLD
                        if (modreg == 6) {
                            vpslld_256(ymmsrc, &ymmres, imm);
                            _load_ymm(vexreg, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;
            case 0x73:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        //VPSRLQ
                        if (modreg == 2) {
                            vpsrlq_256(ymmsrc, &ymmres, imm);
                            _load_ymm(vexreg, &ymmres);
                            ins_size++;
                        }
                        //VPSRLDQ
                        if (modreg == 3) {
                            vpsrldq_256(ymmsrc, &ymmres, imm);
                            _load_ymm(vexreg, &ymmres);
                            ins_size++;
                        }
                       //VPSLLQ
                        if (modreg == 6) {
                            vpsllq_256(ymmsrc, &ymmres, imm);
                            _load_ymm(vexreg, &ymmres);
                            ins_size++;
                        }
                        //VPSLLDQ
                        if (modreg == 7) {
                            vpslldq_256(ymmsrc, &ymmres, imm);
                            _load_ymm(vexreg, &ymmres);
                            ins_size++;
                        }
                   }
                }
                break;
            case 0xC6: //VSHUFPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vshufpd_256(ymmsrc, ymmvsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            case 0xD1: //VPSRLW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = ymmsrc.u8[0];
                        vpsrlw_256(ymmvsrc, &ymmres, count);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xD2: //VPSRLD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = ymmsrc.u8[0];
                        vpsrld_256(ymmvsrc, &ymmres, count);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xD3: //VPSRLQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = ymmsrc.u8[0];
                        vpsrlq_256(ymmvsrc, &ymmres, count);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xE1: //VPSRAW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = ymmsrc.u8[0];
                        vpsraw_256(ymmvsrc, &ymmres, count);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xE2: //VPSRAD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = ymmsrc.u8[0];
                        vpsrad_256(ymmvsrc, &ymmres, count);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xF1: //VPSLLW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = ymmsrc.u8[0];
                        vpsllw_256(ymmvsrc, &ymmres, count);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xF2: //VPSLLD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = ymmsrc.u8[0];
                        vpslld_256(ymmvsrc, &ymmres, count);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xF3: //VPSLLQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        uint8_t count = ymmsrc.u8[0];
                        vpsllq_256(ymmvsrc, &ymmres, count);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Interleave *************/
            case 0x60: //VPUNPCKLBW Byte
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpcklbw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x61: //VPUNPCKLWD Words
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpcklwd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x62: //VPUNPCKLDQ Doublewords
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpckldq_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x6C: //VPUNPCKLQDQ Quadword
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpcklqdq_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
            case 0x68: //VPUNPCKHBW Byte
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpckhbw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x69: //VPUNPCKHWD Words
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpckhwd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x6A: //VPUNPCKHDQ Doublewords
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpckhdq_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x6D: //VPUNPCKHQDQ Quadword
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpunpckhqdq_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x14: //VUNPCKLPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vunpcklpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x15: //VUNPCKHPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vunpckhpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* MAX/MIN Return *************/
            case 0x5D: //VMINPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vminpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x5F: //VMAXPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vmaxpd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Compare *************/
            case 0x64: //VPCMPGTB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpgtb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x65: //VPCMPGTW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpgtw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x66: //VPCMPGTD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpgtd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x74: //VPCMPEQB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpeqb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x75: //VPCMPEQW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpeqw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x76: //VPCMPEQD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpcmpeqd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xC2: //VCMPPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vcmppd_256(ymmsrc, ymmvsrc, &ymmres, imm);
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


/*********************************************************/
int maxdf(double SRC1, double SRC2) {
    //return 1 is SRC1
    //return 0 is SRC2
    if ((SRC1 == 0.0) && (SRC2 == 0.0)) return 0;
    if (isNaN_f64(SRC1)) return 0;
    if (isNaN_f64(SRC2)) return 0;
    if (SRC1 > SRC2) return 1;
    
    return 0;
}

int mindf(double SRC1, double SRC2) {
    //return 1 is SRC1
    //return 0 is SRC2
    if ((SRC1 == 0.0) && (SRC2 == 0.0)) return 0;
    if (isNaN_f64(SRC1)) return 0;
    if (isNaN_f64(SRC2)) return 0;
    if (SRC1 < SRC2) return 1;

    return 0;
}

//comparison predicate operand
int cmpoptdf(double SRC1, double SRC2, uint8_t immvar) {
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
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 4:
            if (SRC1 != SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 5:
            if (SRC1 >= SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 6:
            if (SRC1 > SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 7:
            opt = 1;
            if (isNaN_f64(SRC1)) opt = 0;
            if (isNaN_f64(SRC2)) opt = 0;
            break;
        case 8:
            if (SRC1 == SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 9:
            if (SRC1 < SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 10:
            if (SRC1 <= SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
           break;
        case 11:
            opt = 0;
            break;
        case 12:
            if (SRC1 != SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 0;
            if (isNaN_f64(SRC2)) opt = 0;
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
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 20:
            if (SRC1 != SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
           break;
        case 21:
            if (SRC1 >= SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 22:
            if (SRC1 > SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 23:
            opt = 1;
            if (isNaN_f64(SRC1)) opt = 0;
            if (isNaN_f64(SRC2)) opt = 0;
            break;
        case 24:
            if (SRC1 == SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 25:
            if (SRC1 < SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 26:
            if (SRC1 <= SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 1;
            if (isNaN_f64(SRC2)) opt = 1;
            break;
        case 27:
            opt = 0;
            break;
        case 28:
            if (SRC1 != SRC2) opt = 1;
            if (isNaN_f64(SRC1)) opt = 0;
            if (isNaN_f64(SRC2)) opt = 0;
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
