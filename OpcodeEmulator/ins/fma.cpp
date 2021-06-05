//
//  fma.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/5/1.
//  Copyright © 2019 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "fma.hpp"

int fma_instruction(x86_saved_state_t *state,
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
            /*********************** vfmaddsub pd/ps ***********************/
            case 0x96:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADDSUB132PD
                        if (operand_size == 64) { //W1
                            vfmaddsub132pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMADDSUB132PS
                        if (operand_size == 32) { //W0
                            vfmaddsub132ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xA6:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADDSUB213PD
                        if (operand_size == 64) { //W1
                            vfmaddsub213pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMADDSUB213PS
                        if (operand_size == 32) { //W0
                            vfmaddsub213ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xB6:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADDSUB231PD
                        if (operand_size == 64) { //W1
                            vfmaddsub231pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMADDSUB231PS
                        if (operand_size == 32) { //W0
                            vfmaddsub231ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            /*********************** vfmsubadd pd/ps ***********************/
            case 0x97:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUBADD132PD
                        if (operand_size == 64) { //W1
                            vfmsubadd132pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMSUBADD132PS
                        if (operand_size == 32) { //W0
                            vfmsubadd132ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xA7:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUBADD213PD
                        if (operand_size == 64) { //W1
                            vfmsubadd213pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMSUBADD213PS
                        if (operand_size == 32) { //W0
                            vfmsubadd213ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xB7:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUBADD231PD
                        if (operand_size == 64) { //W1
                            vfmsubadd231pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMSUBADD231PS
                        if (operand_size == 32) { //W0
                            vfmsubadd231ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            /*********************** vfmadd pd/ps ***********************/
            case 0x98:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADD132PD
                        if (operand_size == 64) { //W1
                            vfmadd132pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMADD132PS
                        if (operand_size == 32) { //W0
                            vfmadd132ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xA8:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADD213PD
                        if (operand_size == 64) { //W1
                            vfmadd213pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMADD213PS
                        if (operand_size == 32) { //W0
                            vfmadd213ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xB8:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADD231PD
                        if (operand_size == 64) { //W1
                            vfmadd231pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMADD231PS
                        if (operand_size == 32) { //W0
                            vfmadd231ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
                
            /*********************** vfmadd sd/ss ***********************/
            case 0x99:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADD132SD
                        if (operand_size == 64) { //W1
                            vfmadd132sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMADD132SS
                        if (operand_size == 32) { //W0
                            vfmadd132ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xA9:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADD213SD
                        if (operand_size == 64) { //W1
                            vfmadd213sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMADD213SS
                        if (operand_size == 32) { //W0
                            vfmadd213ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xB9:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADD231SD
                        if (operand_size == 64) { //W1
                            vfmadd231sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMADD231SS
                        if (operand_size == 32) { //W0
                            vfmadd231ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
                
            /*********************** vfmsub pd/ps ***********************/
            case 0x9A:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUB132PD
                        if (operand_size == 64) { //W1
                            vfmsub132pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMSUB132PS
                        if (operand_size == 32) { //W0
                            vfmsub132ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xAA:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUB213PD
                        if (operand_size == 64) { //W1
                            vfmsub213pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMSUB213PS
                        if (operand_size == 32) { //W0
                            vfmsub213ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xBA:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUB231PD
                        if (operand_size == 64) { //W1
                            vfmsub231pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMSUB231PS
                        if (operand_size == 32) { //W0
                            vfmsub231ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
                
            /*********************** vfmsub sd/ss ***********************/
            case 0x9B:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUB132SD
                        if (operand_size == 64) { //W1
                            vfmsub132sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMSUB132SS
                        if (operand_size == 32) { //W0
                            vfmsub132ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xAB:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUB213SD
                        if (operand_size == 64) { //W1
                            vfmsub213sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMSUB213SS
                        if (operand_size == 32) { //W0
                            vfmsub213ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xBB:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUB231SD
                        if (operand_size == 64) { //W1
                            vfmsub231sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFMSUB231SS
                        if (operand_size == 32) { //W0
                            vfmsub231ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
                
            /*********************** vfnmadd pd/ps ***********************/
            case 0x9C:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMADD132PD
                        if (operand_size == 64) { //W1
                            vfnmadd132pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMADD132PS
                        if (operand_size == 32) { //W0
                            vfnmadd132ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xAC:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMADD213PD
                        if (operand_size == 64) { //W1
                            vfnmadd213pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMADD213PS
                        if (operand_size == 32) { //W0
                            vfnmadd213ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xBC:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMADD231PD
                        if (operand_size == 64) { //W1
                            vfnmadd231pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMADD231PS
                        if (operand_size == 32) { //W0
                            vfnmadd231ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            
            /*********************** vfnmadd sd/ss ***********************/
            case 0x9D:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMADD132SD
                        if (operand_size == 64) { //W1
                            vfnmadd132sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMADD132SS
                        if (operand_size == 32) { //W0
                            vfnmadd132ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xAD:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMADD213SD
                        if (operand_size == 64) { //W1
                            vfnmadd213sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMADD213SS
                        if (operand_size == 32) { //W0
                            vfnmadd213ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xBD:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMADD231SD
                        if (operand_size == 64) { //W1
                            vfnmadd231sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMADD231SS
                        if (operand_size == 32) { //W0
                            vfnmadd231ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;

            /*********************** vfnmsub pd/ps ***********************/
            case 0x9E:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMSUB132PD
                        if (operand_size == 64) { //W1
                            vfnmsub132pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMSUB132PS
                        if (operand_size == 32) { //W0
                            vfnmsub132ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xAE:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMSUB213PD
                        if (operand_size == 64) { //W1
                            vfnmsub213pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMSUB213PS
                        if (operand_size == 32) { //W0
                            vfnmsub213ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xBE:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMSUB231PD
                        if (operand_size == 64) { //W1
                            vfnmsub231pd_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMSUB231PS
                        if (operand_size == 32) { //W0
                            vfnmsub231ps_128(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
           
            /*********************** vfnmsub sd/ss ***********************/
            case 0x9F:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMSUB132SD
                        if (operand_size == 64) { //W1
                            vfnmsub132sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMSUB132SS
                        if (operand_size == 32) { //W0
                            vfnmsub132ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xAF:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMSUB213SD
                        if (operand_size == 64) { //W1
                            vfnmsub213sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMSUB213SS
                        if (operand_size == 32) { //W0
                            vfnmsub213ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
                break;
            case 0xBF:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMSUB231SD
                        if (operand_size == 64) { //W1
                            vfnmsub231sd(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
                        }
                        //VFNMSUB231SS
                        if (operand_size == 32) { //W0
                            vfnmsub231ss(xmmsrc, xmmvsrc, xmmdst, &xmmres, mxcsr_rc);
                            _load_xmm(num_dst, &xmmres);
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
            /*********************** vfmaddsub pd/ps ***********************/
            case 0x96:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADDSUB132PD
                        if (operand_size == 64) { //W1
                            vfmaddsub132pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMADDSUB132PS
                        if (operand_size == 32) { //W0
                            vfmaddsub132ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xA6:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADDSUB213PD
                        if (operand_size == 64) { //W1
                            vfmaddsub213pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMADDSUB213PS
                        if (operand_size == 32) { //W0
                            vfmaddsub213ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xB6:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADDSUB231PD
                        if (operand_size == 64) { //W1
                            vfmaddsub231pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMADDSUB231PS
                        if (operand_size == 32) { //W0
                            vfmaddsub231ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            /*********************** vfmsubadd pd/ps ***********************/
            case 0x97:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUBADD132PD
                        if (operand_size == 64) { //W1
                            vfmsubadd132pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMSUBADD132PS
                        if (operand_size == 32) { //W0
                            vfmsubadd132ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xA7:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUBADD213PD
                        if (operand_size == 64) { //W1
                            vfmsubadd213pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMSUBADD213PS
                        if (operand_size == 32) { //W0
                            vfmsubadd213ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xB7:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUBADD231PD
                        if (operand_size == 64) { //W1
                            vfmsubadd231pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMSUBADD231PS
                        if (operand_size == 32) { //W0
                            vfmsubadd231ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            /*********************** vfmadd pd/ps ***********************/
            case 0x98:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADD132PD
                        if (operand_size == 64) { //W1
                            vfmadd132pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMADD132PS
                        if (operand_size == 32) { //W0
                            vfmadd132ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xA8:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADD213PD
                        if (operand_size == 64) { //W1
                            vfmadd213pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMADD213PS
                        if (operand_size == 32) { //W0
                            vfmadd213ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xB8:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMADD231PD
                        if (operand_size == 64) { //W1
                            vfmadd231pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMADD231PS
                        if (operand_size == 32) { //W0
                            vfmadd231ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            /*********************** vfmsub pd/ps ***********************/
            case 0x9A:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUB132PD
                        if (operand_size == 64) { //W1
                            vfmsub132pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMSUB132PS
                        if (operand_size == 32) { //W0
                            vfmsub132ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xAA:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUB213PD
                        if (operand_size == 64) { //W1
                            vfmsub213pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMSUB213PS
                        if (operand_size == 32) { //W0
                            vfmsub213ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xBA:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFMSUB231PD
                        if (operand_size == 64) { //W1
                            vfmsub231pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFMSUB231PS
                        if (operand_size == 32) { //W0
                            vfmsub231ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            /*********************** vfnmadd pd/ps ***********************/
            case 0x9C:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMADD132PD
                        if (operand_size == 64) { //W1
                            vfnmadd132pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFNMADD132PS
                        if (operand_size == 32) { //W0
                            vfnmadd132ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xAC:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMADD213PD
                        if (operand_size == 64) { //W1
                            vfnmadd213pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFNMADD213PS
                        if (operand_size == 32) { //W0
                            vfnmadd213ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xBC:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMADD231PD
                        if (operand_size == 64) { //W1
                            vfnmadd231pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFNMADD231PS
                        if (operand_size == 32) { //W0
                            vfnmadd231ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;

            /*********************** vfnmsub pd/ps ***********************/
            case 0x9E:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMSUB132PD
                        if (operand_size == 64) { //W1
                            vfnmsub132pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFNMSUB132PS
                        if (operand_size == 32) { //W0
                            vfnmsub132ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xAE:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMSUB213PD
                        if (operand_size == 64) { //W1
                            vfnmsub213pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFNMSUB213PS
                        if (operand_size == 32) { //W0
                            vfnmsub213ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                    }
                }
                break;
            case 0xBE:
                if (simd_prefix == 1) { //66
                    if (leading_opcode == 2) { //0F38
                        //VFNMSUB231PD
                        if (operand_size == 64) { //W1
                            vfnmsub231pd_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
                        }
                        //VFNMSUB231PS
                        if (operand_size == 32) { //W0
                            vfnmsub231ps_256(ymmsrc, ymmvsrc, ymmdst, &ymmres, mxcsr_rc);
                            _load_ymm(num_dst, &ymmres);
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
