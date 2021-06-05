//
//  vsse41.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/8.
//  Copyright © 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "vsse41.hpp"

int vsse41_instruction(x86_saved_state_t *state,
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
            /************* Move *************/
            case 0x2A:
                //VMOVNTDQA
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vmovntdqa_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x20:
                if (simd_prefix == 1) { //66
                    //VPMOVSXBW
                    if (leading_opcode == 2) {//0F38
                        vpmovsxbw_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                    //VPINSRB
                    if (leading_opcode == 3) {//0F3A
                        if(mod == 3) {
                            M64 r64;
                            _store_m64(num_src, &r64, state);
                            xmmsrc.u64[0] = r64.u64;
                        }
                        vpinsrb(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
               }
                break;
            case 0x21:
                //VPMOVSXBD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovsxbd_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                    //VINSERTPS
                    if (leading_opcode == 3) {//0F3A
                        vinsertps(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x22:
                if (simd_prefix == 1) { //66
                    //VPMOVSXBQ
                    if (leading_opcode == 2) {//0F38
                        vpmovsxbq_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                    //VPINSRD / VPINSRQ
                    if (leading_opcode == 3) {//0F3A
                        if(mod == 3) {
                            M64 r64;
                            _store_m64(num_src, &r64, state);
                            xmmsrc.u64[0] = r64.u64;
                        }
                        if (operand_size == 32) {
                            vpinsrd(xmmsrc, xmmvsrc, &xmmres, imm);
                            _load_xmm(num_dst, &xmmres);
                            ins_size++;
                        } else {
                            vpinsrq(xmmsrc, xmmvsrc, &xmmres, imm);
                            _load_xmm(num_dst, &xmmres);
                            ins_size++;
                        }
                    }
                }
                break;
            case 0x23:
                //VPMOVSXWD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovsxwd_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x24:
                //VPMOVSXWQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovsxwq_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x25:
                //VPMOVSXDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovsxdq_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
                
            case 0x30:
                //VPMOVZXBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxbw_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x31:
                //VPMOVZXBD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxbd_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x32:
                //VPMOVZXBQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxbq_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x33:
                //VPMOVZXWD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxwd_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x34:
                //VPMOVZXWQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxwq_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x35:
                //VPMOVZXDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxdq_128(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Convert *************/
            case 0x2B:
                //VPACKUSDW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpackusdw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* Compare *************/
            case 0x29:
                //VPCMPEQQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpcmpeqq_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x3C:
                //VPMAXSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmaxsb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xEE:
                //VPMAXSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmaxsw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x3D:
                //VPMAXSD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmaxsd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;

            case 0xDE:
                //VPMAXUB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmaxub_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x3E:
                //VPMAXUW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmaxuw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x3F:
                //VPMAXUD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmaxud_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x38:
                //VPMINSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpminsb_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0xEA:
                //VPMINSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpminsw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x39:
                //VPMINSD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpminsd_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;

            case 0xDA:
                //VPMINUB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpminub_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x3A:
                //VPMINUW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpminuw_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x3B:
                //VPMINUD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpminud_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            /************* multiply *************/
            case 0x28:
                //VPMULDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmuldq_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                }
                break;
            case 0x40:
                if (simd_prefix == 1) { //66
                    //VPMULLD
                    if (leading_opcode == 2) {//0F38
                        vpmulld_128(xmmsrc, xmmvsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
                    //VDPPS
                    if (leading_opcode == 3) {//0F3A
                        vdpps_128(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x41:
                if (simd_prefix == 1) { //66
                    //VDPPD
                    if (leading_opcode == 3) {//0F3A
                        vdppd(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                    //VPHMINPOSUW
                    if (leading_opcode == 2) {//0F38
                        vphminposuw(xmmsrc, &xmmres);
                        _load_xmm(num_dst, &xmmres);
                    }
               }
                break;
            /************* Round *************/
            case 0x08:
                //VROUNDPS
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vroundps_128(xmmsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x09:
                //VROUNDPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vroundpd_128(xmmsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x0A:
                //VROUNDSS
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vroundss(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x0B:
                //VROUNDSD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vroundsd(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            /************* Select *************/
            case 0x0C:
                //VBLENDPS
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vblendps_128(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x0D:
                //VBLENDPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vblendpd_128(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x0E:
                //VPBLENDW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vpblendw_128(xmmsrc, xmmvsrc, &xmmres, imm);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x4A:
                //VBLENDVPS
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        int immreg = (imm >> 4) & 15;
                        XMM immsrc;
                        _store_xmm(immreg, &immsrc);
                        vblendvps_128(xmmsrc, xmmvsrc, &xmmres, immsrc);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x4B:
                //VBLENDVPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        int immreg = (imm >> 4) & 15;
                        XMM immsrc;
                        _store_xmm(immreg, &immsrc);
                        vblendvpd_128(xmmsrc, xmmvsrc, &xmmres, immsrc);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            case 0x4C:
                //VPBLENDVB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        int immreg = (imm >> 4) & 15;
                        XMM immsrc;
                        _store_xmm(immreg, &immsrc);
                        vpblendvb_128(xmmsrc, xmmvsrc, &xmmres, immsrc);
                        _load_xmm(num_dst, &xmmres);
                        ins_size++;
                    }
                }
                break;
            /************* Extract *************/
            case 0x14:
                //VPEXTRB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vpextrb(xmmdst, &xmmres, imm);
                        if(mod == 3) {
                            M64 r64;
                            r64.u64 = xmmres.u64[0];
                            _load_m64(num_src, &r64, state);
                        } else {
                            rm_size = operand_size;
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                        ins_size++;
                    }
                }
                break;
            case 0x15:
                //VPEXTRW DST -> SRC
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vpextrw(xmmdst, &xmmres, imm);
                        if(mod == 3) {
                            M64 r64;
                            r64.u64 = xmmres.u64[0];
                            _load_m64(num_src, &r64, state);
                        } else {
                            rm_size = operand_size;
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                        ins_size++;
                    }
                }
                break;
            case 0x16:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        //VPEXTRD / VPEXTRQ
                        if (operand_size == 32) { //VPEXTRD
                            vpextrd(xmmdst, &xmmres, imm);
                            if(mod == 3) {
                                M64 r64;
                                r64.u64 = xmmres.u64[0];
                                _load_m64(num_src, &r64, state);
                            } else {
                                rm_size = operand_size;
                                _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                            }
                            ins_size++;
                        } else { //VPEXTRQ
                            vpextrq(xmmdst, &xmmres, imm);
                            if(mod == 3) {
                                M64 r64;
                                r64.u64 = xmmres.u64[0];
                                _load_m64(num_src, &r64, state);
                            } else {
                                rm_size = operand_size;
                                _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                            }
                            ins_size++;

                        }
                    }
                }
                break;
            case 0xC5:
                //VPEXTRW SRC -> DST
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpextrw(xmmsrc, &xmmres, imm);
                        M64 r64;
                        r64.u64 = xmmres.u64[0];
                        _load_m64(num_dst, &r64, state);
                        ins_size++;
                    }
                }
                break;
            case 0x17:
                if (simd_prefix == 1) { //66
                    //VEXTRACTPS DST -> SRC
                    if (leading_opcode == 3) {//0F3A
                        vextractps(xmmdst, &xmmres, imm);
                        if(mod == 3) {
                            M64 r64;
                            r64.u64 = xmmres.u64[0];
                            _load_m64(num_src, &r64, state);
                        } else {
                            rm_size = operand_size;
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                        ins_size++;
                    }
                    //VPTEST
                    if (leading_opcode == 2) {//0F38
                        vptest_128(xmmsrc, xmmdst, state);
                    }
                }
                break;
                /************* Other *************/
            case 0x42:
                //VMPSADBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vmpsadbw_128(xmmsrc, xmmvsrc, &xmmres, imm);
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
            case 0x2A:
                //VMOVNTDQA
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vmovntdqa_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x20:
                //VPMOVSXBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovsxbw_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x21:
                //VPMOVSXBD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovsxbd_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x22:
                //VPMOVSXBQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovsxbq_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x23:
                //VPMOVSXWD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovsxwd_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x24:
                //VPMOVSXWQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovsxwq_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x25:
                //VPMOVSXDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovsxdq_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;

            case 0x30:
                //VPMOVZXBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxbw_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x31:
                //VPMOVZXBD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxbd_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x32:
                //VPMOVZXBQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxbq_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x33:
                //VPMOVZXWD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxwd_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x34:
                //VPMOVZXWQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxwq_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x35:
                //VPMOVZXDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmovzxdq_256(ymmsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;

            /************* Convert *************/
            case 0x2B:
                //VPACKUSDW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpackusdw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* Compare *************/
            case 0x29:
                //VPCMPEQQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpcmpeqq_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x3C:
                //VPMAXSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmaxsb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xEE:
                //VPMAXSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmaxsw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x3D:
                //VPMAXSD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmaxsd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;

            case 0xDE:
                //VPMAXUB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpmaxub_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x3E:
                //VPMAXUW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmaxuw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x3F:
                //VPMAXUD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmaxud_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
                
            case 0x38:
                //VPMINSB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpminsb_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0xEA:
                //VPMINSW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpminsw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x39:
                //VPMINSD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpminsd_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;

            case 0xDA:
                //VPMINUB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 1) {//0F
                        vpminub_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x3A:
                //VPMINUW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpminuw_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x3B:
                //VPMINUD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpminud_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            /************* multiply *************/
            case 0x28:
                //VPMULDQ
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vpmuldq_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                }
                break;
            case 0x40:
                if (simd_prefix == 1) { //66
                    //VPMULLD
                    if (leading_opcode == 2) {//0F38
                        vpmulld_256(ymmsrc, ymmvsrc, &ymmres);
                        _load_ymm(num_dst, &ymmres);
                    }
                    //VDPPS
                    if (leading_opcode == 3) {//0F3A
                        vdpps_256(ymmsrc, ymmvsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
                
            /************* Round *************/
            case 0x08:
                //VROUNDPS
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vroundps_256(ymmsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            case 0x09:
                //VROUNDPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vroundpd_256(ymmsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            /************* Select *************/
            case 0x0C:
                //VBLENDPS
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vblendps_256(ymmsrc, ymmvsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            case 0x0D:
                //VBLENDPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vblendpd_256(ymmsrc, ymmvsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            case 0x0E:
                //VPBLENDW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vpblendw_256(ymmsrc, ymmvsrc, &ymmres, imm);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            case 0x4A:
                //VBLENDVPS
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        int immreg = (imm >> 4) & 15;
                        YMM immsrc;
                        _store_ymm(immreg, &immsrc);
                        vblendvps_256(ymmsrc, ymmvsrc, &ymmres, immsrc);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            case 0x4B:
                //VBLENDVPD
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        int immreg = (imm >> 4) & 15;
                        YMM immsrc;
                        _store_ymm(immreg, &immsrc);
                        vblendvpd_256(ymmsrc, ymmvsrc, &ymmres, immsrc);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            case 0x4C:
                //VPBLENDVB
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        int immreg = (imm >> 4) & 15;
                        YMM immsrc;
                        _store_ymm(immreg, &immsrc);
                        vpblendvb_256(ymmsrc, ymmvsrc, &ymmres, immsrc);
                        _load_ymm(num_dst, &ymmres);
                        ins_size++;
                    }
                }
                break;
            /************* Other *************/
            case 0x17:
                //VPTEST
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) {//0F38
                        vptest_256(ymmsrc, ymmdst, state);
                    }
                }
                break;
            case 0x42:
                //VMPSADBW
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) {//0F3A
                        vmpsadbw_256(ymmsrc, ymmvsrc, &ymmres, imm);
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
