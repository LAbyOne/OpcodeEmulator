//
//  avx.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/26.
//  Copyright © 2019 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "avx.hpp"

int avx_instruction(x86_saved_state_t *state,
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
    uint8_t num_dst = (*modrm >> 3) & 0x7;
    uint8_t num_src = *modrm & 0x7;
    
    if (high_reg) num_dst += 8;
    if (high_base) num_src += 8;
    
    uint64_t rmaddrs = 0;
    
    if (reg_size == 128) {
        XMM xmmsrc, xmmvsrc, xmmdst, xmmres;
        
        int consumed = get_consumed(modrm);
        imm = *((uint8_t*)&bytep[consumed]);
        ins_size += consumed;
        
        switch(opcode) {
            case 0x02: //vpblendd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpblendd_128(xmmsrc, xmmvsrc, &xmmres, imm);
                            _load_xmm(num_dst, &xmmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x04: //vpermilps
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermilps_128b(xmmsrc, &xmmres, imm);
                            _load_xmm(num_dst, &xmmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x05: //vpermilpd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermilpd_128b(xmmsrc, &xmmres, imm);
                            _load_xmm(num_dst, &xmmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x0C: //vpermilps
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermilps_128a(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x0D: //vpermilpd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermilpd_128a(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x0E: //vtestps
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vtestps_128(xmmsrc, xmmdst, state);
                        }
                    }
                }
                break;

            case 0x0F: //vtestpd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vtestpd_128(xmmsrc, xmmdst, state);
                        }
                    }
                }
                break;

            case 0x18: //vbroadcastss
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vbroadcastss_128(xmmsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x2C: //vmaskmovps SRC -> MASK -> DST
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vmaskmovps_load_128(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x2E: //vmaskmovps DST -> MASK -> SRC
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            xmmres = xmmsrc;
                            vmaskmovps_store_128(xmmdst, xmmvsrc, &xmmres);
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                    }
                }
                break;

            case 0x2D: //vmaskmovpd SRC -> MASK -> DST
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vmaskmovpd_load_128(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
                
            case 0x2F: //vmaskmovpd DST -> MASK -> SRC
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            xmmres = xmmsrc;
                            vmaskmovpd_store_128(xmmdst, xmmvsrc, &xmmres);
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                    }
                }
                break;

            case 0x45:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //vpsrlvd
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpsrlvd_128(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //vpsrlvq
                        if (operand_size == 64) { //W1
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpsrlvq_128(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x46:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //vpsravd
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpsravd_128(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x47:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //vpsllvd
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpsllvd_128(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //vpsllvq
                        if (operand_size == 64) { //W1
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpsllvq_128(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x78: //vpbroadcastb
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpbroadcastb_128(xmmsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
 
            case 0x79: //vpbroadcastw
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpbroadcastw_128(xmmsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x58: //vpbroadcastd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpbroadcastd_128(xmmsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x59: //vpbroadcastq
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpbroadcastq_128(xmmsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x77: //vzeroupper
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) { //0F
                        vzeroupper(state);
                    }
                }
                break;

            case 0x8C:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //vpmaskmovd SRC -> MASK -> DST
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpmaskmovd_load_128(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //vpmaskmovq SRC -> MASK -> DST
                        if (operand_size == 64) { //W1
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpmaskmovq_load_128(xmmsrc, xmmvsrc, &xmmres);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            case 0x8E:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //vpmaskmovd DST -> MASK -> SRC
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            xmmres = xmmsrc;
                            vpmaskmovd_store_128(xmmdst, xmmvsrc, &xmmres);
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
                        //vpmaskmovq DST -> MASK -> SRC
                        if (operand_size == 64) { //W1
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &xmmvsrc, &xmmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            xmmres = xmmsrc;
                            vpmaskmovq_store_128(xmmdst, xmmvsrc, &xmmres);
                            _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                        }
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
            case 0x00: //vpermq
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 64) { //W1
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermq(ymmsrc, &ymmres, imm);
                            _load_ymm(num_dst, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x01: //vpermpd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 64) { //W1
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermpd(ymmsrc, &ymmres, imm);
                            _load_ymm(num_dst, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x02: //vpblendd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpblendd_256(ymmsrc, ymmvsrc, &ymmres, imm);
                            _load_ymm(num_dst, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x04: //vpermilps
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermilps_256b(ymmsrc, &ymmres, imm);
                            _load_ymm(num_dst, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x05: //vpermilpd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermilpd_256b(ymmsrc, &ymmres, imm);
                            _load_ymm(num_dst, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x06: //vperm2f128
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vperm2f128(ymmsrc, ymmvsrc, &ymmres, imm);
                            _load_ymm(num_dst, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x0C: //vpermilps
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermilps_256a(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x0D: //vpermilpd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermilpd_256a(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x0E: //vtestps
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vtestps_256(ymmsrc, ymmdst, state);
                        }
                    }
                }
                break;

            case 0x0F: //vtestpd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vtestpd_256(ymmsrc, ymmdst, state);
                        }
                    }
                }
                break;

            case 0x16: //vpermps
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermps(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x18:
                if (simd_prefix == 1) { //66
                    //vbroadcastss
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size / 2;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vbroadcastss_256(xmmsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                    //vinsertf128
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size / 2;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vinsertf128(xmmsrc, ymmvsrc, &ymmres, imm);
                            _load_ymm(num_dst, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x19:
                if (simd_prefix == 1) { //66
                    //vbroadcastsd
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size / 2;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);

                            vbroadcastsd(xmmsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                    //vextractf128 DST256 -> SRC128
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vextractf128(ymmdst, &xmmres, imm);
                            if (mod == 3) {
                                _load_xmm(num_src, &xmmres);
                            } else {
                                uint16_t rm_size = reg_size / 2;
                                _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                            }
                            ins_size++;
                        }

                    }
                }
                break;

            case 0x1A: //vbroadcastf128
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size / 2;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vbroadcastf128(xmmsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x2C: //vmaskmovps SRC -> MASK -> DST
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vmaskmovps_load_256(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x2E: //vmaskmovps DST -> MASK -> SRC
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            ymmres = ymmsrc;
                            vmaskmovps_store_256(ymmdst, ymmvsrc, &ymmres);
                            _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                        }
                    }
                }
                break;

            case 0x2D: //vmaskmovpd SRC -> MASK -> DST
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vmaskmovpd_load_256(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
                
            case 0x2F: //vmaskmovpd DST -> MASK -> SRC
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            ymmres = ymmsrc;
                            vmaskmovpd_store_256(ymmdst, ymmvsrc, &ymmres);
                            _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                        }
                    }
                }
                break;
 
            case 0x36: //vpermd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpermd(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

           case 0x38:
                if (simd_prefix == 1) { //66
                    //vinserti128
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size / 2;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);

                            vinserti128(xmmsrc, ymmvsrc, &ymmres, imm);
                            _load_ymm(num_dst, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x39:
                if (simd_prefix == 1) { //66
                    //vextracti128 DST256 -> SRC128
                    if (leading_opcode == 3) { //0F3A
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vextracti128(ymmdst, &xmmres, imm);
                            if (mod == 3) {
                                _load_xmm(num_src, &xmmres);
                            } else {
                                uint16_t rm_size = reg_size / 2;
                                _load_maddr_from_xmm(rmaddrs, &xmmres, rm_size, state);
                            }
                            ins_size++;
                        }
                    }
                }
                break;
                
            case 0x45:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //vpsrlvd
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpsrlvd_256(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //vpsrlvq
                        if (operand_size == 64) { //W1
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpsrlvq_256(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x46:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //vpsravd
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpsravd_256(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                    if (leading_opcode == 3) { //0F3A
                        //vpsravd
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vperm2i128(ymmsrc, ymmvsrc, &ymmres, imm);
                            _load_ymm(num_dst, &ymmres);
                            ins_size++;
                        }
                    }
                }
                break;

            case 0x47:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //vpsllvd
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpsllvd_256(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //vpsllvq
                        if (operand_size == 64) { //W1
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpsllvq_256(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x77: //vzeroall
                if (simd_prefix == 0) { //None
                    if (leading_opcode == 1) { //0F
                        vzeroall(state);
                    }
                }
                break;

            case 0x78: //vpbroadcastb
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size / 2;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpbroadcastb_256(xmmsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x79: //vpbroadcastw
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size / 2;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpbroadcastw_256(xmmsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x58: //vpbroadcastd
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size / 2;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpbroadcastd_256(xmmsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x59: //vpbroadcastq
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size / 2;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpbroadcastq_256(xmmsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x5A: //vbroadcasti128
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size / 2;
                            get_vexregs(modrm, high_reg, high_index, high_base, &xmmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vbroadcasti128(xmmsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x8C:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //vpmaskmovd SRC -> MASK -> DST
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpmaskmovd_load_256(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //vpmaskmovq SRC -> MASK -> DST
                        if (operand_size == 64) { //W1
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            
                            vpmaskmovq_load_256(ymmsrc, ymmvsrc, &ymmres);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            case 0x8E:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //vpmaskmovd DST -> MASK -> SRC
                        if (operand_size == 32) { //W0
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            ymmres = ymmsrc;
                            vpmaskmovd_store_256(ymmdst, ymmvsrc, &ymmres);
                            _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);
                        }
                        //vpmaskmovq DST -> MASK -> SRC
                        if (operand_size == 64) { //W1
                            uint16_t rm_size = reg_size;
                            get_vexregs(modrm, high_reg, high_index, high_base, &ymmsrc, &ymmvsrc, &ymmdst, vexreg, state, kernel_trap, reg_size, rm_size, modbyte, &rmaddrs);
                            ymmres = ymmsrc;
                            vpmaskmovq_store_256(ymmdst, ymmvsrc, &ymmres);
                            _load_maddr_from_ymm(rmaddrs, &ymmres, rm_size, state);

                        }
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
