//
//  avx.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/26.
//  Copyright © 2019 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef avx_hpp
#define avx_hpp

#include "opemu.hpp"

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
                    int kernel_trap);

/**********************************************/
/**  AVX instructions implementation         **/
/**********************************************/
inline void vbroadcastss_128(XMM src, XMM *res) {
    res->fa32[0] = src.fa32[0];
    res->fa32[1] = src.fa32[0];
    res->fa32[2] = src.fa32[0];
    res->fa32[3] = src.fa32[0];
}

inline void vbroadcastss_256(XMM src, YMM *res) {
    res->fa32[0] = src.fa32[0];
    res->fa32[1] = src.fa32[0];
    res->fa32[2] = src.fa32[0];
    res->fa32[3] = src.fa32[0];
    res->fa32[4] = src.fa32[0];
    res->fa32[5] = src.fa32[0];
    res->fa32[6] = src.fa32[0];
    res->fa32[7] = src.fa32[0];

}

inline void vbroadcastsd(XMM src, YMM *res) {
    res->fa64[0] = src.fa64[0];
    res->fa64[1] = src.fa64[0];
    res->fa64[2] = src.fa64[0];
    res->fa64[3] = src.fa64[0];
    
}

inline void vbroadcastf128(XMM src, YMM *res) {
    res->a128[0] = src.a128;
    res->a128[1] = src.a128;
}

inline void vpbroadcastb_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 16; ++i)
        res->u8[i] = src.u8[0];
}
inline void vpbroadcastb_256(XMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 32; ++i)
        res->u8[i] = src.u8[0];
}

inline void vpbroadcastw_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i)
        res->u16[i] = src.u16[0];
}
inline void vpbroadcastw_256(XMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 16; ++i)
        res->u16[i] = src.u16[0];
}

inline void vpbroadcastd_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i)
        res->u32[i] = src.u32[0];
}
inline void vpbroadcastd_256(XMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i)
        res->u32[i] = src.u32[0];
}

inline void vpbroadcastq_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 2; ++i)
        res->u64[i] = src.u64[0];
}
inline void vpbroadcastq_256(XMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i)
        res->u64[i] = src.u64[0];
}

inline void vbroadcasti128(XMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 2; ++i)
        res->u128[i] = src.u128;
}

inline void vextractf128(YMM dst, XMM *res, uint8_t imm) {
    int IMM0 = imm & 1;
    
    if (IMM0 == 1) {
        res->fa64[0] = dst.fa64[2];
        res->fa64[1] = dst.fa64[3];
    } else {
        res->fa64[0] = dst.fa64[0];
        res->fa64[1] = dst.fa64[1];
    }
}

inline void vextracti128(YMM dst, XMM *res, uint8_t imm) {
    int IMM0 = imm & 1;
    
    if (IMM0 == 1)
        res->u128 = dst.u128[1];
    else
        res->u128 = dst.u128[0];
}

inline void vinsertf128(XMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int IMM0 = imm & 1;
    
    YMM TEMP;
    TEMP.u256 = vsrc.u256;
    
    if (IMM0 == 1)
        TEMP.u128[1] = src.u128;
    else
        TEMP.u128[0] = src.u128;
    
    res->fa64[0] = TEMP.fa64[0];
    res->fa64[1] = TEMP.fa64[1];
    res->fa64[2] = TEMP.fa64[2];
    res->fa64[3] = TEMP.fa64[3];
}

inline void vinserti128(XMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int IMM0 = imm & 1;
    
    YMM TEMP;
    TEMP.u256 = vsrc.u256;

    if (IMM0 == 1)
        TEMP.u128[1] = src.u128;
    else
        TEMP.u128[0] = src.u128;

    res->u256 = TEMP.u256;
}

inline void vmaskmovps_load_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int cs;
    uint32_t mask_bit = 0;
    
    for (i = 0; i < 4; ++i) {
        mask_bit = vsrc.u32[i];
        cs = mask_bit >> 31 & 1;
        if (cs == 1) {
            res->fa32[i] = src.fa32[i];
        } else {
            res->u32[i] = 0;
        }
    }
}

inline void vmaskmovps_load_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int cs;
    uint32_t mask_bit = 0;
    
    for (i = 0; i < 8; ++i) {
        mask_bit = vsrc.u32[i];
        cs = mask_bit >> 31 & 1;
        if (cs == 1) {
            res->fa32[i] = src.fa32[i];
        } else {
            res->u32[i] = 0;
        }
    }

}

inline void vmaskmovps_store_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int cs;
    uint32_t mask_bit = 0;
    
    for (i = 0; i < 4; ++i) {
        mask_bit = vsrc.u32[i];
        cs = mask_bit >> 31 & 1;
        if (cs == 1) {
            res->fa32[i] = src.fa32[i];
        }
    }
}

inline void vmaskmovps_store_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int cs;
    uint32_t mask_bit = 0;
    
    for (i = 0; i < 8; ++i) {
        mask_bit = vsrc.u32[i];
        cs = mask_bit >> 31 & 1;
        if (cs == 1) {
            res->fa32[i] = src.fa32[i];
        }
    }
}

inline void vmaskmovpd_load_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int cs;
    uint64_t mask_bit = 0;
    
    for (i = 0; i < 2; ++i) {
        mask_bit = vsrc.u64[i];
        cs = mask_bit >> 63 & 1;
        if (cs == 1) {
            res->fa64[i] = src.fa64[i];
        } else {
            res->u64[i] = 0;
        }
    }
}

inline void vmaskmovpd_load_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int cs;
    uint64_t mask_bit = 0;
    
    for (i = 0; i < 4; ++i) {
        mask_bit = vsrc.u64[i];
        cs = mask_bit >> 63 & 1;
        if (cs == 1) {
            res->fa64[i] = src.fa64[i];
        } else {
            res->u64[i] = 0;
        }
    }
}

inline void vmaskmovpd_store_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int cs;
    uint64_t mask_bit = 0;
    
    for (i = 0; i < 2; ++i) {
        mask_bit = vsrc.u64[i];
        cs = mask_bit >> 63 & 1;
        if (cs == 1) {
            res->fa64[i] = src.fa64[i];
        }
    }
}

inline void vmaskmovpd_store_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int cs;
    uint64_t mask_bit = 0;
    
    for (i = 0; i < 4; ++i) {
        mask_bit = vsrc.u64[i];
        cs = mask_bit >> 63 & 1;
        if (cs == 1) {
            res->fa64[i] = src.fa64[i];
        }
    }
}

inline void vpblendd_128(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int i;
    int cs;
    
    for (i = 0; i < 4; ++i) {
        cs = (imm >> i) & 1;
        if (cs == 1) {
            res->u32[i] = src.u32[i];
        } else {
            res->u32[i] = vsrc.u32[i];
        }
    }
}
inline void vpblendd_256(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int i;
    int cs;
    
    for (i = 0; i < 8; ++i) {
        cs = (imm >> i) & 1;
        if (cs == 1) {
            res->u32[i] = src.u32[i];
        } else {
            res->u32[i] = vsrc.u32[i];
        }
    }
}

inline void vpermilpd_128a(XMM src, XMM vsrc, XMM *res) {
    int i;
    int cs;
    uint64_t mask = 0;
    
    for (i = 0; i < 2; ++i) {
        mask = src.u64[i];
        cs = (mask >> 1) & 1;
        if (cs == 1) {
            res->fa64[i] = vsrc.fa64[1];
        } else {
            res->fa64[i] = vsrc.fa64[0];
        }
    }
}
inline void vpermilpd_256a(YMM src, YMM vsrc, YMM *res) {
    int i;
    int cs;
    uint64_t mask = 0;
    
    for (i = 0; i < 2; ++i) {
        mask = src.u64[i];
        cs = (mask >> 1) & 1;
        if (cs == 1) {
            res->fa64[i] = vsrc.fa64[1];
        } else {
            res->fa64[i] = vsrc.fa64[0];
        }
    }
    for (i = 2; i < 4; ++i) {
        mask = src.u64[i];
        cs = (mask >> 1) & 1;
        if (cs == 1) {
            res->fa64[i] = vsrc.fa64[3];
        } else {
            res->fa64[i] = vsrc.fa64[2];
        }
    }

}

inline void vpermilpd_128b(XMM src, XMM *res, uint8_t imm) {
    int i;
    int cs;
    int mask = imm & 3;
    
    for (i = 0; i < 2; ++i) {
        cs = (mask >> i) & 1;
        if (cs == 1) {
            res->fa64[i] = src.fa64[1];
        } else {
            res->fa64[i] = src.fa64[0];
        }
    }
}
inline void vpermilpd_256b(YMM src, YMM *res, uint8_t imm) {
    int i;
    int cs;
    int mask = imm & 15;
    
    for (i = 0; i < 2; ++i) {
        cs = (mask >> i) & 1;
        if (cs == 1) {
            res->fa64[i] = src.fa64[1];
        } else {
            res->fa64[i] = src.fa64[0];
        }
    }
    for (i = 2; i < 4; ++i) {
        cs = (mask >> i) & 1;
        if (cs == 1) {
            res->fa64[i] = src.fa64[3];
        } else {
            res->fa64[i] = src.fa64[2];
        }
    }
}

inline void vpermilps_128a(XMM src, XMM vsrc, XMM *res) {
    int i;
    int cs;
    uint64_t mask = 0;
    
    for (i = 0; i < 4; ++i) {
        mask = src.u32[i];
        cs = mask & 3;
        res->fa32[i] = vsrc.fa32[cs];
    }
}
inline void vpermilps_256a(YMM src, YMM vsrc, YMM *res) {
    int i;
    int cs;
    uint64_t mask = 0;
    
    for (i = 0; i < 4; ++i) {
        mask = src.u32[i];
        cs = mask & 3;
        res->fa32[i] = vsrc.fa32[cs];
    }
    for (i = 4; i < 8; ++i) {
        mask = src.u32[i];
        cs = mask & 3;
        res->fa32[i] = vsrc.fa32[cs+4];
    }

}

inline void vpermilps_128b(XMM src, XMM *res, uint8_t imm) {
    int i;
    int cs;
    int rof = 0;
    
    for (i = 0; i < 4; ++i) {
        rof = i * 2;
        cs = (imm >> rof) & 3;
        
        res->fa32[i] = src.fa32[cs];
    }
}

inline void vpermilps_256b(YMM src, YMM *res, uint8_t imm) {
    int i;
    int cs;
    int rof = 0;
    
    for (i = 0; i < 4; ++i) {
        rof = i * 2;
        cs = (imm >> rof) & 3;
        
        res->fa32[i] = src.fa32[cs];
    }
    for (i = 4; i < 8; ++i) {
        rof = (i - 4) * 2;
        cs = (imm >> rof) & 3;

        res->fa32[i] = src.fa32[cs+4];
    }
}

inline void vpmaskmovd_load_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int cs;
    uint32_t mask_bit = 0;
    
    for (i = 0; i < 4; ++i) {
        mask_bit = vsrc.u32[i];
        cs = mask_bit >> 31 & 1;
        if (cs == 1) {
            res->u32[i] = src.u32[i];
        } else {
            res->u32[i] = 0;
        }
    }
}
inline void vpmaskmovd_load_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int cs;
    uint32_t mask_bit = 0;
    
    for (i = 0; i < 8; ++i) {
        mask_bit = vsrc.u32[i];
        cs = mask_bit >> 31 & 1;
        if (cs == 1) {
            res->u32[i] = src.u32[i];
        } else {
            res->u32[i] = 0;
        }
    }
}

inline void vpmaskmovq_load_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int cs;
    uint64_t mask_bit = 0;
    
    for (i = 0; i < 2; ++i) {
        mask_bit = vsrc.u64[i];
        cs = mask_bit >> 63 & 1;
        if (cs == 1) {
            res->u64[i] = src.u64[i];
        } else {
            res->u64[i] = 0;
        }
    }
}

inline void vpmaskmovq_load_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int cs;
    uint64_t mask_bit = 0;
    
    for (i = 0; i < 4; ++i) {
        mask_bit = vsrc.u64[i];
        cs = mask_bit >> 63 & 1;
        if (cs == 1) {
            res->u64[i] = src.u64[i];
        } else {
            res->u64[i] = 0;
        }
    }
}

inline void vpmaskmovd_store_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int cs;
    uint32_t mask_bit = 0;
    
    for (i = 0; i < 4; ++i) {
        mask_bit = vsrc.u32[i];
        cs = mask_bit >> 31 & 1;
        if (cs == 1) {
            res->u32[i] = src.u32[i];
        }
    }
}
inline void vpmaskmovd_store_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int cs;
    uint32_t mask_bit = 0;
    
    for (i = 0; i < 8; ++i) {
        mask_bit = vsrc.u32[i];
        cs = mask_bit >> 31 & 1;
        if (cs == 1) {
            res->u32[i] = src.u32[i];
        }
    }
}

inline void vpmaskmovq_store_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int cs;
    uint64_t mask_bit = 0;
    
    for (i = 0; i < 2; ++i) {
        mask_bit = vsrc.u64[i];
        cs = mask_bit >> 63 & 1;
        if (cs == 1) {
            res->u64[i] = src.u64[i];
        }
    }
}

inline void vpmaskmovq_store_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int cs;
    uint64_t mask_bit = 0;
    
    for (i = 0; i < 4; ++i) {
        mask_bit = vsrc.u64[i];
        cs = mask_bit >> 63 & 1;
        if (cs == 1) {
            res->u64[i] = src.u64[i];
        }
    }
}

inline void vpsllvd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    uint32_t ZeroExtend = 0;
    uint32_t SRC1 = 0, COUNT = 0;
    
    for (i = 0; i < 4; ++i) {
        SRC1 = vsrc.u32[i];
        COUNT = src.u32[i];
        ZeroExtend = SRC1 << COUNT;
        res->u32[i] = ZeroExtend;
    }
}
inline void vpsllvd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    uint32_t ZeroExtend = 0;
    uint32_t SRC1 = 0, COUNT = 0;
    
    for (i = 0; i < 8; ++i) {
        SRC1 = vsrc.u32[i];
        COUNT = src.u32[i];
        ZeroExtend = SRC1 << COUNT;
        res->u32[i] = ZeroExtend;
    }
}

inline void vpsllvq_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    uint64_t ZeroExtend = 0;
    uint64_t SRC1 = 0, COUNT = 0;
    
    for (i = 0; i < 2; ++i) {
        SRC1 = vsrc.u64[i];
        COUNT = src.u64[i];
        ZeroExtend = SRC1 << COUNT;
        res->u64[i] = ZeroExtend;
    }
}
inline void vpsllvq_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    uint64_t ZeroExtend = 0;
    uint64_t SRC1 = 0, COUNT = 0;
    
    for (i = 0; i < 4; ++i) {
        SRC1 = vsrc.u64[i];
        COUNT = src.u64[i];
        ZeroExtend = SRC1 << COUNT;
        res->u64[i] = ZeroExtend;
    }
}

inline void vpsravd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int32_t SignExtend;
    int32_t SRC1 = 0, COUNT = 0;

    for (i = 0; i < 4; ++i) {
        SRC1 = vsrc.a32[i];
        COUNT = src.a32[i];
        SignExtend = SRC1 << COUNT;
        res->a32[i] = SignExtend;
    }
}
inline void vpsravd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int32_t SignExtend;
    int32_t SRC1 = 0, COUNT = 0;
    
    for (i = 0; i < 8; ++i) {
        SRC1 = vsrc.a32[i];
        COUNT = src.a32[i];
        SignExtend = SRC1 >> COUNT;
        res->a32[i] = SignExtend;
    }
}

inline void vpsrlvd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    uint32_t ZeroExtend = 0;
    uint32_t SRC1 = 0, COUNT = 0;
    
    for (i = 0; i < 4; ++i) {
        SRC1 = vsrc.u32[i];
        COUNT = src.u32[i];
        ZeroExtend = SRC1 >> COUNT;
        res->u32[i] = ZeroExtend;
    }
}
inline void vpsrlvd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    uint32_t ZeroExtend = 0;
    uint32_t SRC1 = 0, COUNT = 0;
    
    for (i = 0; i < 8; ++i) {
        SRC1 = vsrc.u32[i];
        COUNT = src.u32[i];
        ZeroExtend = SRC1 >> COUNT;
        res->u32[i] = ZeroExtend;
    }
}

inline void vpsrlvq_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    uint64_t ZeroExtend = 0;
    uint64_t SRC1 = 0, COUNT = 0;
    
    for (i = 0; i < 2; ++i) {
        SRC1 = vsrc.u64[i];
        COUNT = src.u64[i];
        ZeroExtend = SRC1 >> COUNT;
        res->u64[i] = ZeroExtend;
    }
}
inline void vpsrlvq_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    uint64_t ZeroExtend = 0;
    uint64_t SRC1 = 0, COUNT = 0;
    
    for (i = 0; i < 4; ++i) {
        SRC1 = vsrc.u64[i];
        COUNT = src.u64[i];
        ZeroExtend = SRC1 >> COUNT;
        res->u64[i] = ZeroExtend;
    }
}

inline void vtestps_128(XMM src, XMM dst, x86_saved_state_t *state) {
    int tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
    XMM TEMP;
    uint64_t FLAGS = 0;
    uint64_t RFLAGS = 0;
    uint32_t EFLAGS = 0;
    
    if (is_saved_state64(state)) {
        RFLAGS = saved_state64(state)->isf.rflags;
        FLAGS = RFLAGS;
    } else {
        EFLAGS = saved_state32(state)->efl;
        FLAGS = EFLAGS;
    }

    TEMP.u128 = src.u128 & dst.u128;
    tmp0 = (TEMP.u32[0] >> 31) & 1;
    tmp1 = (TEMP.u32[1] >> 31) & 1;
    tmp2 = (TEMP.u32[2] >> 31) & 1;
    tmp3 = (TEMP.u32[3] >> 31) & 1;

    if ((tmp0 = 0) && (tmp1 = 0) && (tmp2 = 0) && (tmp3 = 0)) {
        //ZF = 1
        FLAGS = FLAGS | 0x0040;
    } else {
        //ZF = 0
        FLAGS = (FLAGS | 0x0040) ^ 0x0040;
    }
    TEMP.u128 = src.u128 & ~dst.u128;
    tmp0 = (TEMP.u32[0] >> 31) & 1;
    tmp1 = (TEMP.u32[1] >> 31) & 1;
    tmp2 = (TEMP.u32[2] >> 31) & 1;
    tmp3 = (TEMP.u32[3] >> 31) & 1;

    if ((tmp0 = 0) && (tmp1 = 0) && (tmp2 = 0) && (tmp3 = 0)) {
        //CF = 1
        FLAGS = FLAGS | 0x0001;
    } else {
        //CF = 0
        FLAGS = (FLAGS | 0x0001) ^ 0x0001;
    }
    //AF←OF←PF←SF←0
    FLAGS = (FLAGS | 0x0010) ^ 0x0010;
    FLAGS = (FLAGS | 0x0800) ^ 0x0800;
    FLAGS = (FLAGS | 0x0004) ^ 0x0004;
    FLAGS = (FLAGS | 0x0080) ^ 0x0080;
    
    if (is_saved_state64(state)) {
        RFLAGS = FLAGS;
        saved_state64(state)->isf.rflags = RFLAGS;
    } else {
        EFLAGS = FLAGS & 0xffffffff;
        saved_state32(state)->efl = EFLAGS;
    }
}
inline void vtestps_256(YMM src, YMM dst, x86_saved_state_t *state) {
    int tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0, tmp5 = 0, tmp6 = 0, tmp7 = 0;
    YMM TEMP;

    uint64_t FLAGS = 0;
    uint64_t RFLAGS = 0;
    uint32_t EFLAGS = 0;
    
    if (is_saved_state64(state)) {
        RFLAGS = saved_state64(state)->isf.rflags;
        FLAGS = RFLAGS;
    } else {
        EFLAGS = saved_state32(state)->efl;
        FLAGS = EFLAGS;
    }

    TEMP.u128[0] = src.u128[0] & dst.u128[0];
    TEMP.u128[1] = src.u128[1] & dst.u128[1];
    tmp0 = (TEMP.u32[0] >> 31) & 1;
    tmp1 = (TEMP.u32[1] >> 31) & 1;
    tmp2 = (TEMP.u32[2] >> 31) & 1;
    tmp3 = (TEMP.u32[3] >> 31) & 1;
    tmp4 = (TEMP.u32[4] >> 31) & 1;
    tmp5 = (TEMP.u32[5] >> 31) & 1;
    tmp6 = (TEMP.u32[6] >> 31) & 1;
    tmp7 = (TEMP.u32[7] >> 31) & 1;

    
    if ((tmp0 = 0) && (tmp1 = 0) && (tmp2 = 0) && (tmp3 = 0) && (tmp4 = 0) && (tmp5 = 0) && (tmp6 = 0) && (tmp7 = 0)) {
        //ZF = 1
        FLAGS = FLAGS | 0x0040;
    } else {
        //ZF = 0
        FLAGS = (FLAGS | 0x0040) ^ 0x0040;
    }

    TEMP.u128[0] = src.u128[0] & ~dst.u128[0];
    TEMP.u128[1] = src.u128[1] & ~dst.u128[1];
    tmp0 = (TEMP.u32[0] >> 31) & 1;
    tmp1 = (TEMP.u32[1] >> 31) & 1;
    tmp2 = (TEMP.u32[2] >> 31) & 1;
    tmp3 = (TEMP.u32[3] >> 31) & 1;
    tmp4 = (TEMP.u32[4] >> 31) & 1;
    tmp5 = (TEMP.u32[5] >> 31) & 1;
    tmp6 = (TEMP.u32[6] >> 31) & 1;
    tmp7 = (TEMP.u32[7] >> 31) & 1;

    if ((tmp0 = 0) && (tmp1 = 0) && (tmp2 = 0) && (tmp3 = 0) && (tmp4 = 0) && (tmp5 = 0) && (tmp6 = 0) && (tmp7 = 0)) {
        //CF = 1
        FLAGS = FLAGS | 0x0001;
    } else {
        //CF = 0
        FLAGS = (FLAGS | 0x0001) ^ 0x0001;
    }
    //AF←OF←PF←SF←0
    FLAGS = (FLAGS | 0x0010) ^ 0x0010;
    FLAGS = (FLAGS | 0x0800) ^ 0x0800;
    FLAGS = (FLAGS | 0x0004) ^ 0x0004;
    FLAGS = (FLAGS | 0x0080) ^ 0x0080;

    if (is_saved_state64(state)) {
        RFLAGS = FLAGS;
        saved_state64(state)->isf.rflags = RFLAGS;
    } else {
        EFLAGS = FLAGS & 0xffffffff;
        saved_state32(state)->efl = EFLAGS;
    }

}

inline void vtestpd_128(XMM src, XMM dst, x86_saved_state_t *state) {
    int tmp0 = 0, tmp1 = 0;
    XMM TEMP;
    uint64_t FLAGS = 0;
    uint64_t RFLAGS = 0;
    uint32_t EFLAGS = 0;
    
    if (is_saved_state64(state)) {
        RFLAGS = saved_state64(state)->isf.rflags;
        FLAGS = RFLAGS;
    } else {
        EFLAGS = saved_state32(state)->efl;
        FLAGS = EFLAGS;
    }
    
    TEMP.u128 = src.u128 & dst.u128;
    tmp0 = (TEMP.u64[0] >> 63) & 1;
    tmp1 = (TEMP.u64[1] >> 63) & 1;

    if ((tmp0 = 0) && (tmp1 = 0)) {
        //ZF = 1
        FLAGS = FLAGS | 0x0040;
    } else {
        //ZF = 0
        FLAGS = (FLAGS | 0x0040) ^ 0x0040;
    }
    TEMP.u128 = src.u128 & ~dst.u128;
    tmp0 = (TEMP.u64[0] >> 63) & 1;
    tmp1 = (TEMP.u64[1] >> 63) & 1;

    if ((tmp0 = 0) && (tmp1 = 0)) {
        //CF = 1
        FLAGS = FLAGS | 0x0001;
    } else {
        //CF = 0
        FLAGS = (FLAGS | 0x0001) ^ 0x0001;
    }
    //AF←OF←PF←SF←0
    FLAGS = (FLAGS | 0x0010) ^ 0x0010;
    FLAGS = (FLAGS | 0x0800) ^ 0x0800;
    FLAGS = (FLAGS | 0x0004) ^ 0x0004;
    FLAGS = (FLAGS | 0x0080) ^ 0x0080;

    if (is_saved_state64(state)) {
        RFLAGS = FLAGS;
        saved_state64(state)->isf.rflags = RFLAGS;
    } else {
        EFLAGS = FLAGS & 0xffffffff;
        saved_state32(state)->efl = EFLAGS;
    }
}
inline void vtestpd_256(YMM src, YMM dst, x86_saved_state_t *state) {
    int tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
    YMM TEMP;
    
    uint64_t FLAGS = 0;
    uint64_t RFLAGS = 0;
    uint32_t EFLAGS = 0;
    
    if (is_saved_state64(state)) {
        RFLAGS = saved_state64(state)->isf.rflags;
        FLAGS = RFLAGS;
    } else {
        EFLAGS = saved_state32(state)->efl;
        FLAGS = EFLAGS;
    }
    
    TEMP.u128[0] = src.u128[0] & dst.u128[0];
    TEMP.u128[1] = src.u128[1] & dst.u128[1];
    tmp0 = (TEMP.u64[0] >> 63) & 1;
    tmp1 = (TEMP.u64[1] >> 63) & 1;
    tmp2 = (TEMP.u64[2] >> 63) & 1;
    tmp3 = (TEMP.u64[3] >> 63) & 1;

    
    if ((tmp0 = 0) && (tmp1 = 0) && (tmp2 = 0) && (tmp3 = 0)) {
        //ZF = 1
        FLAGS = FLAGS | 0x0040;
    } else {
        //ZF = 0
        FLAGS = (FLAGS | 0x0040) ^ 0x0040;
    }
    
    TEMP.u128[0] = src.u128[0] & ~dst.u128[0];
    TEMP.u128[1] = src.u128[1] & ~dst.u128[1];
    tmp0 = (TEMP.u64[0] >> 63) & 1;
    tmp1 = (TEMP.u64[1] >> 63) & 1;
    tmp2 = (TEMP.u64[2] >> 63) & 1;
    tmp3 = (TEMP.u64[3] >> 63) & 1;

    if ((tmp0 = 0) && (tmp1 = 0) && (tmp2 = 0) && (tmp3 = 0)) {
        //CF = 1
        FLAGS = FLAGS | 0x0001;
    } else {
        //CF = 0
        FLAGS = (FLAGS | 0x0001) ^ 0x0001;
    }
    //AF←OF←PF←SF←0
    FLAGS = (FLAGS | 0x0010) ^ 0x0010;
    FLAGS = (FLAGS | 0x0800) ^ 0x0800;
    FLAGS = (FLAGS | 0x0004) ^ 0x0004;
    FLAGS = (FLAGS | 0x0080) ^ 0x0080;

    if (is_saved_state64(state)) {
        RFLAGS = FLAGS;
        saved_state64(state)->isf.rflags = RFLAGS;
    } else {
        EFLAGS = FLAGS & 0xffffffff;
        saved_state32(state)->efl = EFLAGS;
    }
    
}

inline void vperm2f128(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    YMM tmp;
    int imm1_0 = imm & 3;
    int imm5_4 = (imm >> 4) & 3;
    int imm3 = (imm >> 3) & 1;
    int imm7 = (imm >> 7) & 1;
    
    if (imm1_0 == 0) {
        tmp.u128[0] = vsrc.u128[0];
    } else if (imm1_0 == 1) {
        tmp.u128[0] = vsrc.u128[1];
    } else if (imm1_0 == 2) {
        tmp.u128[0] = src.u128[0];
    } else if (imm1_0 == 3) {
        tmp.u128[0] = src.u128[1];
    }
    
    if (imm5_4 == 0) {
        tmp.u128[1] = vsrc.u128[0];
    } else if (imm1_0 == 1) {
        tmp.u128[1] = vsrc.u128[1];
    } else if (imm1_0 == 2) {
        tmp.u128[1] = src.u128[0];
    } else if (imm1_0 == 3) {
        tmp.u128[1] = src.u128[1];
    }

    if (imm3 == 1) {
        tmp.u128[0] = 0;
    }
    if (imm7 == 1) {
        tmp.u128[1] = 0;
    }
    
    res->fa64[0] = tmp.fa64[0];
    res->fa64[1] = tmp.fa64[1];
    res->fa64[2] = tmp.fa64[2];
    res->fa64[3] = tmp.fa64[3];
}

inline void vperm2i128(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int imm1_0 = imm & 3;
    int imm5_4 = (imm >> 4) & 3;
    int imm3 = (imm >> 3) & 1;
    int imm7 = (imm >> 7) & 1;
    
    if (imm1_0 == 0) {
        res->u128[0] = vsrc.u128[0];
    } else if (imm1_0 == 1) {
        res->u128[0] = vsrc.u128[1];
    } else if (imm1_0 == 2) {
        res->u128[0] = src.u128[0];
    } else if (imm1_0 == 3) {
        res->u128[0] = src.u128[1];
    }
    
    if (imm5_4 == 0) {
        res->u128[1] = vsrc.u128[0];
    } else if (imm1_0 == 1) {
        res->u128[1] = vsrc.u128[1];
    } else if (imm1_0 == 2) {
        res->u128[1] = src.u128[0];
    } else if (imm1_0 == 3) {
        res->u128[1] = src.u128[1];
    }
    
    if (imm3 == 1) {
        res->u128[0] = 0;
    }
    if (imm7 == 1) {
        res->u128[1] = 0;
    }
}

inline void vpermd(YMM src, YMM vsrc, YMM *res) {
    int i;
    int idx = 0;

    for (i = 0; i < 8; ++i) {
        idx = vsrc.u32[i] & 7;
        res->u32[i] = src.u32[idx];
    }
}

inline void vpermps(YMM src, YMM vsrc, YMM *res) {
    int i;
    int idx = 0;
    
    for (i = 0; i < 8; ++i) {
        idx = vsrc.u32[i] & 7;
        res->fa32[i] = src.fa32[idx];
    }
}

inline void vpermq(YMM src, YMM *res, uint8_t imm) {
    int i;
    int j = 0;
    int idx = 0;
    
    for (i = 0; i < 4; ++i) {
        j = i * 2;
        idx = (imm >> j) & 3;
        res->u64[i] = src.u64[idx];
    }
}

inline void vpermpd(YMM src, YMM *res, uint8_t imm) {
    int i;
    int j = 0;
    int idx = 0;
    
    for (i = 0; i < 4; ++i) {
        j = i * 2;
        idx = (imm >> j) & 3;
        res->fa64[i] = src.fa64[idx];
    }
}

inline void vzeroupper(x86_saved_state_t *state) {
    int i;
    int limit = 0;
    YMM TEMP;
    
    if (is_saved_state64(state)) {
        limit = 16;
    } else {
        limit = 8;
    }
    
    for (i = 0; i < limit; ++i) {
        _store_ymm(i, &TEMP);
        TEMP.u128[1] = 0;
        _load_ymm(i, &TEMP);
    }
}
inline void vzeroall(x86_saved_state_t *state) {
    int i;
    int limit = 0;
    YMM TEMP;
    
    if (is_saved_state64(state)) {
        limit = 16;
    } else {
        limit = 8;
    }
    
    for (i = 0; i < limit; ++i) {
        _store_ymm(i, &TEMP);
        TEMP.u128[0] = 0;
        TEMP.u128[1] = 0;
        _load_ymm(i, &TEMP);
    }
}

#endif /* avx_hpp */
