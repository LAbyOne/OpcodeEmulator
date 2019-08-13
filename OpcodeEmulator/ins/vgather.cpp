//
//  vgather.cpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/25.
//  Copyright © 2019 lvs1974. All rights reserved.
//  Made in Taiwan.

#include "vgather.hpp"

int vgather_instruction(x86_saved_state_t *state,
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
    
    //uint8_t imm;
    //uint8_t mod = *modrm >> 6; // ModRM.mod
    //uint8_t modreg = (*modrm >> 3) & 0x7;
    uint8_t num_dst = (*modrm >> 3) & 0x7;
    //uint8_t num_src = *modrm & 0x7;

    //get vindex regs
    uint8_t sib =  *((uint8_t*)&modrm[1]);
    uint8_t index = (sib >> 3) & 0x7; //SIB Index

    if (high_reg) num_dst += 8;
    //if (high_base) num_src += 8;
    if (high_index) index += 8;
    
    XMM xmmvsrc, xmmindex, xmmres;
    YMM ymmvsrc, ymmindex, ymmres;
    
    int consumed = get_consumed(modrm);
    ins_size += consumed;

    switch(opcode) {
        case 0x90:
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) { //0F38
                    if (operand_size == 64) { //W1
                        if (reg_size == 128){ //VEX.128
                            //VPGATHERDQ 128 xmm1, vm32x, xmm2
                            //_store_xmm(num_dst, &xmmdst);
                            _store_xmm(vexreg, &xmmvsrc);
                            _store_xmm(index, &xmmindex);
                            vpgatherdq128(xmmvsrc, xmmindex, &xmmres, state, modrm, high_base, ins_size);
                            _load_xmm(num_dst, &xmmres);

                        } else { //VEX.256
                            //VPGATHERDQ 256 ymm1, vm32x, ymm2
                            //_store_ymm(num_dst, &ymmdst);
                            _store_ymm(vexreg, &ymmvsrc);
                            _store_xmm(index, &xmmindex);
                            vpgatherdq256(ymmvsrc, xmmindex, &ymmres, state, modrm, high_base, ins_size);
                            _load_ymm(num_dst, &ymmres);
                        }
                    } else { //W0
                        if (reg_size == 128){ //VEX.128
                            //VPGATHERDD 128 xmm1, vm32x, xmm2
                            //_store_xmm(num_dst, &xmmdst);
                            _store_xmm(vexreg, &xmmvsrc);
                            _store_xmm(index, &xmmindex);
                            vpgatherdd128(xmmvsrc, xmmindex, &xmmres, state, modrm, high_base, ins_size);
                            _load_xmm(num_dst, &xmmres);
                        } else { //VEX.256
                            //VPGATHERDD 256 ymm1, vm32y, ymm2
                            //_store_ymm(num_dst, &ymmdst);
                            _store_ymm(vexreg, &ymmvsrc);
                            _store_ymm(index, &ymmindex);
                            vpgatherdd256(ymmvsrc, ymmindex, &ymmres, state, modrm, high_base, ins_size);
                            _load_ymm(num_dst, &ymmres);
                       }
                    }
                }
            }
            break;

        case 0x91:
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) { //0F38
                    if (operand_size == 64) { //W1
                        if (reg_size == 128){ //VEX.128
                            //VPGATHERQQ 128 xmm1, vm64x, xmm2
                            //_store_xmm(num_dst, &xmmdst);
                            _store_xmm(vexreg, &xmmvsrc);
                            _store_xmm(index, &xmmindex);
                            vpgatherqq128(xmmvsrc, xmmindex, &xmmres, state, modrm, high_base, ins_size);
                            _load_xmm(num_dst, &xmmres);
                        } else { //VEX.256
                            //VPGATHERQQ 256 ymm1, vm64y, ymm2
                            //_store_ymm(num_dst, &ymmdst);
                            _store_ymm(vexreg, &ymmvsrc);
                            _store_ymm(index, &ymmindex);
                            vpgatherqq256(ymmvsrc, ymmindex, &ymmres, state, modrm, high_base, ins_size);
                            _load_ymm(num_dst, &ymmres);
                        }
                    } else { //W0
                        if (reg_size == 128){ //VEX.128
                            //VPGATHERQD 128 xmm1, vm64x, xmm2
                            //_store_xmm(num_dst, &xmmdst);
                            _store_xmm(vexreg, &xmmvsrc);
                            _store_xmm(index, &xmmindex);
                            vpgatherqd128(xmmvsrc, xmmindex, &xmmres, state, modrm, high_base, ins_size);
                            _load_xmm(num_dst, &xmmres);
                        } else { //VEX.256
                            //VPGATHERQD 256 xmm1, vm64y, xmm2
                            //_store_xmm(num_dst, &xmmdst);
                            _store_xmm(vexreg, &xmmvsrc);
                            _store_ymm(index, &ymmindex);
                            vpgatherqd256(xmmvsrc, ymmindex, &xmmres, state, modrm, high_base, ins_size);
                            _load_xmm(num_dst, &xmmres);
                       }
                    }
                }
            }
            break;

        case 0x92:
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) { //0F38
                    if (operand_size == 64) { //W1
                        if (reg_size == 128){ //VEX.128
                            //VGATHERDPD 128 xmm1, vm32x, xmm2
                            //_store_xmm(num_dst, &xmmdst);
                            _store_xmm(vexreg, &xmmvsrc);
                            _store_xmm(index, &xmmindex);
                            vgatherdpd128(xmmvsrc, xmmindex, &xmmres, state, modrm, high_base, ins_size);
                            _load_xmm(num_dst, &xmmres);
                        } else { //VEX.256
                            //VGATHERDPD 256 ymm1, vm32x, ymm2
                            //_store_ymm(num_dst, &ymmdst);
                            _store_ymm(vexreg, &ymmvsrc);
                            _store_xmm(index, &xmmindex);
                            vgatherdpd256(ymmvsrc, xmmindex, &ymmres, state, modrm, high_base, ins_size);
                            _load_ymm(num_dst, &ymmres);
                        }
                    } else { //W0
                        if (reg_size == 128){ //VEX.128
                            //VGATHERDPS 128 xmm1, vm32x, xmm2
                            //_store_xmm(num_dst, &xmmdst);
                            _store_xmm(vexreg, &xmmvsrc);
                            _store_xmm(index, &xmmindex);
                            vgatherdps128(xmmvsrc, xmmindex, &xmmres, state, modrm, high_base, ins_size);
                            _load_xmm(num_dst, &xmmres);
                       } else { //VEX.256
                            //VGATHERDPS 256 ymm1, vm32y, ymm2
                           //_store_ymm(num_dst, &ymmdst);
                           _store_ymm(vexreg, &ymmvsrc);
                           _store_ymm(index, &ymmindex);
                           vgatherdps256(ymmvsrc, ymmindex, &ymmres, state, modrm, high_base, ins_size);
                           _load_ymm(num_dst, &ymmres);
                       }
                    }
                }
            }
            break;

        case 0x93:
            if (simd_prefix == 1) { //66
                if (leading_opcode == 2) { //0F38
                    if (operand_size == 64) { //W1
                        if (reg_size == 128){ //VEX.128
                            //VGATHERQPD 128 xmm1, vm64x, xmm2
                            //_store_xmm(num_dst, &xmmdst);
                            _store_xmm(vexreg, &xmmvsrc);
                            _store_xmm(index, &xmmindex);
                            vgatherqpd128(xmmvsrc, xmmindex, &xmmres, state, modrm, high_base, ins_size);
                            _load_xmm(num_dst, &xmmres);
                        } else { //VEX.256
                            //VGATHERQPD 256 ymm1, vm64y, ymm2
                            //_store_ymm(num_dst, &ymmdst);
                            _store_ymm(vexreg, &ymmvsrc);
                            _store_ymm(index, &ymmindex);
                            vgatherqpd256(ymmvsrc, ymmindex, &ymmres, state, modrm, high_base, ins_size);
                            _load_ymm(num_dst, &ymmres);
                        }
                    } else { //W0
                        if (reg_size == 128){ //VEX.128
                            //VGATHERQPS 128 xmm1, vm64x, xmm2
                            //_store_xmm(num_dst, &xmmdst);
                            _store_xmm(vexreg, &xmmvsrc);
                            _store_xmm(index, &xmmindex);
                            vgatherqps128(xmmvsrc, xmmindex, &xmmres, state, modrm, high_base, ins_size);
                            _load_xmm(num_dst, &xmmres);
                        } else { //VEX.256
                            //VGATHERQPS 256 xmm1, vm64y, xmm2
                            //_store_xmm(num_dst, &xmmdst);
                            _store_xmm(vexreg, &xmmvsrc);
                            _store_ymm(index, &ymmindex);
                            vgatherqps256(xmmvsrc, ymmindex, &xmmres, state, modrm, high_base, ins_size);
                            _load_xmm(num_dst, &xmmres);
                        }
                    }
                }
            }
            break;

        default: return 0;
    }

    return ins_size;
}
