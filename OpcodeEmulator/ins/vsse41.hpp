//
//  vsse41.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/8.
//  Copyright © 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef vsse41_hpp
#define vsse41_hpp

#include "opemu.hpp"
#include "fpins.hpp"

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
                       int kernel_trap);

/**********************************************/
/**  VSSE4.1  instructions implementation       **/
/**********************************************/
/************* Move *************/
inline void vmovntdqa_128(XMM src, XMM *res) {
    res->u128 = src.u128;
}
inline void vmovntdqa_256(YMM src, YMM *res) {
    res->u256 = src.u256;
}

inline void vpinsrb(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int sel = imm & 15;
    res->u128 = vsrc.u128;
    res->u8[sel] = src.u8[0];
}
inline void vpinsrd(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int sel = imm & 3;
    res->u128 = vsrc.u128;
    res->u32[sel] = src.u32[0];
}
inline void vpinsrq(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int sel = imm & 1;
    res->u128 = vsrc.u128;
    res->u64[sel] = src.u64[0];
}
inline void vinsertps(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int i;
    int xbit;

    int count_s = (imm >> 6) & 3;
    int count_d = (imm >> 4) & 3;
    int zmask = imm & 15;
    float tmp = src.fa32[count_s];
    XMM tmp2;

    tmp2.u128 = vsrc.u128;
    tmp2.fa32[count_d] = tmp;

    for (i = 0; i < 4; ++i) {
        xbit = (zmask >> i) & 1;
        if (xbit)
            res->fa32[i] = 0;
        else
            res->fa32[i] = tmp2.fa32[i];
    }
}

inline void vpmovsxbw_128(XMM src, XMM *res) {
    int i;

    for (i = 0; i < 8; ++i) {
        res->a16[i] = src.a8[i];
    }
}
inline void vpmovsxbw_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 16; ++i) {
        res->a16[i] = src.a8[i];
    }
}

inline void vpmovsxbd_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->a32[i] = src.a8[i];
    }
}
inline void vpmovsxbd_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i) {
        res->a32[i] = src.a8[i];
    }
}

inline void vpmovsxbq_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 2; ++i) {
        res->a64[i] = src.a8[i];
    }
}
inline void vpmovsxbq_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->a64[i] = src.a8[i];
    }
}

inline void vpmovsxwd_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->a32[i] = src.a16[i];
    }
}
inline void vpmovsxwd_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i) {
        res->a32[i] = src.a16[i];
    }
}

inline void vpmovsxwq_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 2; ++i) {
        res->a64[i] = src.a16[i];
    }
}
inline void vpmovsxwq_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->a64[i] = src.a16[i];
    }
}

inline void vpmovsxdq_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 2; ++i) {
        res->a64[i] = src.a32[i];
    }
}
inline void vpmovsxdq_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->a64[i] = src.a32[i];
    }
}

inline void vpmovzxbw_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i) {
        res->u16[i] = src.u8[i];
    }
}
inline void vpmovzxbw_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 16; ++i) {
        res->u16[i] = src.u8[i];
    }
}

inline void vpmovzxbd_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->u32[i] = src.u8[i];
    }
}
inline void vpmovzxbd_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i) {
        res->u32[i] = src.u8[i];
    }
}

inline void vpmovzxbq_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 2; ++i) {
        res->u64[i] = src.u8[i];
    }
}
inline void vpmovzxbq_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->u64[i] = src.u8[i];
    }
}

inline void vpmovzxwd_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->u32[i] = src.u16[i];
    }
}
inline void vpmovzxwd_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i) {
        res->u32[i] = src.u16[i];
    }
}

inline void vpmovzxwq_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 2; ++i) {
        res->u64[i] = src.u16[i];
    }
}
inline void vpmovzxwq_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->u64[i] = src.u16[i];
    }
}

inline void vpmovzxdq_128(XMM src, XMM *res) {
    int i;
    
    for (i = 0; i < 2; ++i) {
        res->u64[i] = src.u32[i];
    }
}
inline void vpmovzxdq_256(YMM src, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->u64[i] = src.u32[i];
    }
}

/************* Convert *************/
inline void vpackusdw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 4; ++i) {
        res->u16[i] = STUW(vsrc.a32[i]);
        res->u16[i+4] = STUW(src.a32[i]);
    }
}
inline void vpackusdw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 4; ++i) {
        res->u16[i] = STUW(vsrc.a32[i]);
        res->u16[i+4] = STUW(src.a32[i]);
    }
    for (i = 0; i < 4; ++i) {
        res->u16[i+8] = STUW(vsrc.a32[i+4]);
        res->u16[i+12] = STUW(src.a32[i+4]);
    }
}

/************* Compare *************/
inline void vpcmpeqq_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 2; ++i) {
        if (vsrc.u64[i] == src.u64[i])
            res->u64[i] = 0xffffffffffffffff;
        else
            res->u64[i] = 0;
    }
}
inline void vpcmpeqq_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 4; ++i) {
        if (vsrc.u64[i] == src.u64[i])
            res->u64[i] = 0xffffffffffffffff;
        else
            res->u64[i] = 0;
    }
}
inline void vpmaxsb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 16; ++i) {
        if (vsrc.a8[i] > src.a8[i])
            res->a8[i] = vsrc.a8[i];
        else
            res->a8[i] = src.a8[i];
    }
}
inline void vpmaxsb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 32; ++i) {
        if (vsrc.a8[i] > src.a8[i])
            res->a8[i] = vsrc.a8[i];
        else
            res->a8[i] = src.a8[i];
    }
}
inline void vpmaxsw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 8; ++i) {
        if (vsrc.a16[i] > src.a16[i])
            res->a16[i] = vsrc.a16[i];
        else
            res->a16[i] = src.a16[i];
    }
}
inline void vpmaxsw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 16; ++i) {
        if (vsrc.a16[i] > src.a16[i])
            res->a16[i] = vsrc.a16[i];
        else
            res->a16[i] = src.a16[i];
    }
}
inline void vpmaxsd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 4; ++i) {
        if (vsrc.a32[i] > src.a32[i])
            res->a32[i] = vsrc.a32[i];
        else
            res->a32[i] = src.a32[i];
    }
}
inline void vpmaxsd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 8; ++i) {
        if (vsrc.a32[i] > src.a32[i])
            res->a32[i] = vsrc.a32[i];
        else
            res->a32[i] = src.a32[i];
    }
}

inline void vpmaxub_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 16; ++i) {
        if (vsrc.u8[i] > src.u8[i])
            res->u8[i] = vsrc.u8[i];
        else
            res->u8[i] = src.u8[i];
    }
}
inline void vpmaxub_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 32; ++i) {
        if (vsrc.u8[i] > src.u8[i])
            res->u8[i] = vsrc.u8[i];
        else
            res->u8[i] = src.u8[i];
    }
}
inline void vpmaxuw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 8; ++i) {
        if (vsrc.u16[i] > src.u16[i])
            res->u16[i] = vsrc.u16[i];
        else
            res->u16[i] = src.u16[i];
    }
}
inline void vpmaxuw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 16; ++i) {
        if (vsrc.u16[i] > src.u16[i])
            res->u16[i] = vsrc.u16[i];
        else
            res->u16[i] = src.u16[i];
    }
}
inline void vpmaxud_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 4; ++i) {
        if (vsrc.u32[i] > src.u32[i])
            res->u32[i] = vsrc.u32[i];
        else
            res->u32[i] = src.u32[i];
    }
}
inline void vpmaxud_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 8; ++i) {
        if (vsrc.u32[i] > src.u32[i])
            res->u32[i] = vsrc.u32[i];
        else
            res->u32[i] = src.u32[i];
    }
}

inline void vpminsb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 16; ++i) {
        if (vsrc.a8[i] < src.a8[i])
            res->a8[i] = vsrc.a8[i];
        else
            res->a8[i] = src.a8[i];
    }
}
inline void vpminsb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 32; ++i) {
        if (vsrc.a8[i] < src.a8[i])
            res->a8[i] = vsrc.a8[i];
        else
            res->a8[i] = src.a8[i];
    }
}
inline void vpminsw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 8; ++i) {
        if (vsrc.a16[i] < src.a16[i])
            res->a16[i] = vsrc.a16[i];
        else
            res->a16[i] = src.a16[i];
    }
}
inline void vpminsw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 16; ++i) {
        if (vsrc.a16[i] < src.a16[i])
            res->a16[i] = vsrc.a16[i];
        else
            res->a16[i] = src.a16[i];
    }
}
inline void vpminsd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 4; ++i) {
        if (vsrc.a32[i] < src.a32[i])
            res->a32[i] = vsrc.a32[i];
        else
            res->a32[i] = src.a32[i];
    }
}
inline void vpminsd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 8; ++i) {
        if (vsrc.a32[i] < src.a32[i])
            res->a32[i] = vsrc.a32[i];
        else
            res->a32[i] = src.a32[i];
    }
}

inline void vpminub_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 16; ++i) {
        if (vsrc.u8[i] < src.u8[i])
            res->u8[i] = vsrc.u8[i];
        else
            res->u8[i] = src.u8[i];
    }
}
inline void vpminub_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 32; ++i) {
        if (vsrc.u8[i] < src.u8[i])
            res->u8[i] = vsrc.u8[i];
        else
            res->u8[i] = src.u8[i];
    }
}
inline void vpminuw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 8; ++i) {
        if (vsrc.u16[i] < src.u16[i])
            res->u16[i] = vsrc.u16[i];
        else
            res->u16[i] = src.u16[i];
    }
}
inline void vpminuw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 16; ++i) {
        if (vsrc.u16[i] < src.u16[i])
            res->u16[i] = vsrc.u16[i];
        else
            res->u16[i] = src.u16[i];
    }
}
inline void vpminud_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    for (i = 0; i < 4; ++i) {
        if (vsrc.u32[i] < src.u32[i])
            res->u32[i] = vsrc.u32[i];
        else
            res->u32[i] = src.u32[i];
    }
}
inline void vpminud_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    for (i = 0; i < 8; ++i) {
        if (vsrc.u32[i] < src.u32[i])
            res->u32[i] = vsrc.u32[i];
        else
            res->u32[i] = src.u32[i];
    }
}

/************* multiply *************/
inline void vpmuldq_128(XMM src, XMM vsrc, XMM *res) {
    res->a64[0] = vsrc.a32[0] * src.a32[0];
    res->a64[1] = vsrc.a32[2] * src.a32[2];
}
inline void vpmuldq_256(YMM src, YMM vsrc, YMM *res) {
    res->a64[0] = vsrc.a32[0] * src.a32[0];
    res->a64[1] = vsrc.a32[2] * src.a32[2];
    res->a64[2] = vsrc.a32[4] * src.a32[4];
    res->a64[3] = vsrc.a32[6] * src.a32[6];
}

inline void vpmulld_128(XMM src, XMM vsrc, XMM *res) {
    XMM tmp;
    int i;
    for (i = 0; i < 4; ++i) {
        tmp.a64[0] = vsrc.a32[i] * src.a32[i];
        res->a32[i] = tmp.a32[0];
    }
}
inline void vpmulld_256(YMM src, YMM vsrc, YMM *res) {
    XMM tmp;
    int i;
    for (i = 0; i < 8; ++i) {
        tmp.a64[0] = vsrc.a32[i] * src.a32[i];
        res->a32[i] = tmp.a32[0];
    }
}

inline void vdpps_128(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int i;
    int immvar;
    int imm0var = imm & 15;
    int imm4var = imm >> 4;

    XMM temp1, temp2, temp3, temp4;
    XMM SRC1, SRC2;
    
    //127:0 bit
    SRC1.u128 = vsrc.u128;
    SRC2.u128 = src.u128;

    for (i = 0; i < 4; ++i) {
        immvar = (imm4var > i) & 1;
        if (immvar == 1)
            temp1.fa32[i] = SRC1.fa32[i] * SRC2.fa32[i];
        else
            temp1.fa32[i] = 0.0;
    }
    
    temp2.fa32[0] = temp1.fa32[0] + temp1.fa32[1];
    temp3.fa32[0] = temp1.fa32[2] + temp1.fa32[3];
    temp4.fa32[0] = temp2.fa32[0] + temp3.fa32[0];
    
    for (i = 0; i < 4; ++i) {
        immvar = (imm0var > i) & 1;
        if (immvar == 1)
            res->fa32[i] = temp4.fa32[0];
        else
            res->fa32[i] = 0.0;
    }
}

inline void vdpps_256(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int i;
    int immvar;
    int imm0var = imm & 15;
    int imm4var = imm >> 4;
    
    XMM temp1, temp2, temp3, temp4;
    XMM SRC1, SRC2;
    
    //127:0 bit
    SRC1.u128 = vsrc.u128[0];
    SRC2.u128 = src.u128[0];
    
    for (i = 0; i < 4; ++i) {
        immvar = (imm4var > i) & 1;
        if (immvar == 1)
            temp1.fa32[i] = SRC1.fa32[i] * SRC2.fa32[i];
        else
            temp1.fa32[i] = 0.0;
    }
    
    temp2.fa32[0] = temp1.fa32[0] + temp1.fa32[1];
    temp3.fa32[0] = temp1.fa32[2] + temp1.fa32[3];
    temp4.fa32[0] = temp2.fa32[0] + temp3.fa32[0];
    
    for (i = 0; i < 4; ++i) {
        immvar = (imm0var > i) & 1;
        if (immvar == 1)
            res->fa32[i] = temp4.fa32[0];
        else
            res->fa32[i] = 0.0;
    }
    
    //255:128 bit
    SRC1.u128 = vsrc.u128[1];
    SRC2.u128 = src.u128[1];

    for (i = 0; i < 4; ++i) {
        immvar = (imm4var > i) & 1;
        if (immvar == 1)
            temp1.fa32[i] = SRC1.fa32[i] * SRC2.fa32[i];
        else
            temp1.fa32[i] = 0.0;
    }
    
    temp2.fa32[0] = temp1.fa32[0] + temp1.fa32[1];
    temp3.fa32[0] = temp1.fa32[2] + temp1.fa32[3];
    temp4.fa32[0] = temp2.fa32[0] + temp3.fa32[0];

    for (i = 0; i < 4; ++i) {
        immvar = (imm0var > i) & 1;
        if (immvar == 1)
            res->fa32[i+4] = temp4.fa32[0];
        else
            res->fa32[i+4] = 0.0;
    }
}

inline void vdppd(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int i;
    int immvar;
    int imm0var = imm & 15;
    int imm4var = imm >> 4;
    
    XMM temp1, temp2;
    XMM SRC1, SRC2;
    
    //127:0 bit
    SRC1.u128 = vsrc.u128;
    SRC2.u128 = src.u128;
    
    for (i = 0; i < 2; ++i) {
        immvar = (imm4var > i) & 1;
        if (immvar == 1)
            temp1.fa64[i] = SRC1.fa64[i] * SRC2.fa64[i];
        else
            temp1.fa64[i] = 0.0;
    }
    
    temp2.fa64[0] = temp1.fa64[0] + temp1.fa64[1];
    
    for (i = 0; i < 2; ++i) {
        immvar = (imm0var > i) & 1;
        if (immvar == 1)
            res->fa64[i] = temp2.fa64[0];
        else
            res->fa64[i] = 0.0;
    }
}

inline void vroundps_128(XMM src, XMM *res, uint8_t imm) {
    int i;
    int rc;
    int msi = (imm >> 2) & 1;
    if (msi == 0) {
        rc = imm & 3;
    } else {
        //get mxcsr round control
        rc = getmxcsr();
    }
    
    for (i = 0; i < 4; ++i) {
        res->fa32[i] = round_fp32(src.fa32[i], rc);
    }
}

inline void vroundps_256(YMM src, YMM *res, uint8_t imm) {
    int i;
    int rc;
    int msi = (imm >> 2) & 1;
    if (msi == 0) {
        rc = imm & 3;
    } else {
        //get mxcsr round control
        rc = getmxcsr();
    }
    
    for (i = 0; i < 8; ++i) {
        res->fa32[i] = round_fp32(src.fa32[i], rc);
    }
}

inline void vroundpd_128(XMM src, XMM *res, uint8_t imm) {
    int i;
    int rc;
    int msi = (imm >> 2) & 1;
    if (msi == 0) {
        rc = imm & 3;
    } else {
        //get mxcsr round control
        rc = getmxcsr();
    }
    
    for (i = 0; i < 2; ++i) {
        res->fa64[i] = round_fp64(src.fa64[i], rc);
    }
}
inline void vroundpd_256(YMM src, YMM *res, uint8_t imm) {
    int i;
    int rc;
    int msi = (imm >> 2) & 1;
    if (msi == 0) {
        rc = imm & 3;
    } else {
        //get mxcsr round control
        rc = getmxcsr();
    }
    
    for (i = 0; i < 4; ++i) {
        res->fa64[i] = round_fp64(src.fa64[i], rc);
    }
}

inline void vroundss(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int rc;
    int msi = (imm >> 2) & 1;
    if (msi == 0) {
        rc = imm & 3;
    } else {
        //get mxcsr round control
        rc = getmxcsr();
    }
    res->u128 = vsrc.u128;
    res->fa32[0] = round_fp32(src.fa32[0], rc);
}

inline void vroundsd(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int rc;
    int msi = (imm >> 2) & 1;
    if (msi == 0) {
        rc = imm & 3;
    } else {
        //get mxcsr round control
        rc = getmxcsr();
    }
    res->u128 = vsrc.u128;
    res->fa64[0] = round_fp64(src.fa64[0], rc);
}

/************* Select *************/
inline void vblendps_128(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int i;
    int xbit = 0;
    for (i = 0; i < 4; ++i) {
        xbit = (imm >> i) & 1;
        if (xbit)
            res->fa32[i] = src.fa32[i];
        else
            res->fa32[i] = vsrc.fa32[i];
    }
}
inline void vblendps_256(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int i;
    int xbit = 0;
    for (i = 0; i < 8; ++i) {
        xbit = (imm >> i) & 1;
        if (xbit)
            res->fa32[i] = src.fa32[i];
        else
            res->fa32[i] = vsrc.fa32[i];
    }
}

inline void vblendpd_128(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int i;
    int xbit = 0;
    for (i = 0; i < 2; ++i) {
        xbit = (imm >> i) & 1;
        if (xbit)
            res->fa64[i] = src.fa64[i];
        else
            res->fa64[i] = vsrc.fa64[i];
    }
}
inline void vblendpd_256(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int i;
    int xbit = 0;
    for (i = 0; i < 4; ++i) {
        xbit = (imm >> i) & 1;
        if (xbit)
            res->fa64[i] = src.fa64[i];
        else
            res->fa64[i] = vsrc.fa64[i];
    }
}

inline void vpblendw_128(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int i;
    int xbit = 0;
    
    for (i = 0; i < 8; ++i) {
        xbit = (imm >> i) & 1;
        if (xbit)
            res->u16[i] = src.u16[i];
        else
            res->u16[i] = vsrc.u16[i];
    }
}
inline void vpblendw_256(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int i;
    int xbit = 0;
    
    for (i = 0; i < 16; ++i) {
        xbit = (imm >> i) & 1;
        if (xbit)
            res->u16[i] = src.u16[i];
        else
            res->u16[i] = vsrc.u16[i];
    }
}

inline void vblendvps_128(XMM src, XMM vsrc, XMM *res, XMM immsrc) {
    int i;
    int xbit = 0;
    uint32_t mask;
    
    for (i = 0; i < 4; ++i) {
        mask = immsrc.u32[i];
        xbit = (mask >> 31) & 1;
        if (xbit)
            res->fa32[i] = src.fa32[i];
        else
            res->fa32[i] = vsrc.fa32[i];
    }
}
inline void vblendvps_256(YMM src, YMM vsrc, YMM *res, YMM immsrc) {
    int i;
    int xbit = 0;
    uint32_t mask;
    
    for (i = 0; i < 8; ++i) {
        mask = immsrc.u32[i];
        xbit = (mask >> 31) & 1;
        if (xbit)
            res->fa32[i] = src.fa32[i];
        else
            res->fa32[i] = vsrc.fa32[i];
    }
}

inline void vblendvpd_128(XMM src, XMM vsrc, XMM *res, XMM immsrc) {
    int i;
    int xbit = 0;
    uint64_t mask;
    
    for (i = 0; i < 2; ++i) {
        mask = immsrc.u64[i];
        xbit = (mask >> 63) & 1;
        if (xbit)
            res->fa64[i] = src.fa64[i];
        else
            res->fa64[i] = vsrc.fa64[i];
    }
}
inline void vblendvpd_256(YMM src, YMM vsrc, YMM *res, YMM immsrc) {
    int i;
    int xbit = 0;
    uint64_t mask;
    
    for (i = 0; i < 4; ++i) {
        mask = immsrc.u64[i];
        xbit = (mask >> 63) & 1;
        if (xbit)
            res->fa64[i] = src.fa64[i];
        else
            res->fa64[i] = vsrc.fa64[i];
    }
}

inline void vpblendvb_128(XMM src, XMM vsrc, XMM *res, XMM immsrc) {
    int i;
    int xbit = 0;
    uint8_t mask;

    for (i = 0; i < 16; ++i) {
        mask = immsrc.u8[i];
        xbit = (mask >> 7) & 1;
        if (xbit)
            res->u8[i] = src.u8[i];
        else
            res->u8[i] = vsrc.u8[i];
    }
}
inline void vpblendvb_256(YMM src, YMM vsrc, YMM *res, YMM immsrc) {
    int i;
    int xbit = 0;
    uint8_t mask;
    
    for (i = 0; i < 32; ++i) {
        mask = immsrc.u8[i];
        xbit = (mask >> 7) & 1;
        if (xbit)
            res->u8[i] = src.u8[i];
        else
            res->u8[i] = vsrc.u8[i];
    }
}

/************* Extract *************/
inline void vpextrb(XMM dst, XMM *res, uint8_t imm) {
    int sel = imm & 15;
    res->u128 = 0;
    res->u8[0] = dst.u8[sel];
}
inline void vpextrw(XMM dst, XMM *res, uint8_t imm) {
    int sel = imm & 7;
    res->u128 = 0;
    res->u16[0] = dst.u16[sel];
}
inline void vpextrd(XMM dst, XMM *res, uint8_t imm) {
    int sel = imm & 3;
    res->u128 = 0;
    res->u32[0] = dst.u32[sel];
}
inline void vpextrq(XMM dst, XMM *res, uint8_t imm) {
    int sel = imm & 1;
    res->u128 = 0;
    res->u64[0] = dst.u64[sel];
}
inline void vextractps(XMM dst, XMM *res, uint8_t imm) {
    int sel = imm & 3;
    res->u128 = 0;
    res->fa32[0] = dst.fa32[sel];
}

/************* Other *************/
inline void vphminposuw(XMM src, XMM *res) {
    int i;
    int index = 0;
    uint16_t min = src.u16[0];
    
    for (i = 1; i < 8; ++i) {
        if (src.u16[i] < min)
            index = i;
    }
    min = src.u16[index];
    
    res->u128 = 0;
    res->u16[0] = min;
    res->u16[1] = index;
}

inline void vptest_128(XMM src, XMM dst, x86_saved_state_t *state) {
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
    
    XMM AND1, AND2;
    
    AND1.u128 = src.u128 & dst.u128;
    AND2.u128 = src.u128 & ~(dst.u128);

    if (AND1.u128 == 0) {
        FLAGS |= 0x00000040; //set ZF = 1
    } else {
        FLAGS = (FLAGS | 0x00000040) ^ 0x00000040;  //set ZF = 0
    }
    
    if (AND2.u128 == 0) {
        FLAGS |= 0x00000001; //set CF = 1
    } else {
        FLAGS = (FLAGS | 0x00000001) ^ 0x00000001;  //set CF = 0
    }

    FLAGS = (FLAGS | 0x00000800) ^ 0x00000800;  //set OF = 0
    FLAGS = (FLAGS | 0x00000010) ^ 0x00000010;  //set AF = 0
    FLAGS = (FLAGS | 0x00000080) ^ 0x00000080;  //set SF = 0
    FLAGS = (FLAGS | 0x00000004) ^ 0x00000004;  //set PF = 0
    
    if (is_saved_state64(state)) {
        saved_state64(state)->isf.rflags = FLAGS;
    } else {
        EFLAGS = FLAGS & 0xFFFFFFFF;
        saved_state32(state)->efl = EFLAGS;
    }
}

inline void vptest_256(YMM src, YMM dst, x86_saved_state_t *state) {
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
    
    XMM AND11, AND12, AND21, AND22;
    
    AND11.u128 = src.u128[0] & dst.u128[0];
    AND12.u128 = src.u128[0] & dst.u128[0];

    AND21.u128 = src.u128[1] & ~(dst.u128[1]);
    AND22.u128 = src.u128[1] & ~(dst.u128[1]);
    
    if ( (AND11.u128 == 0) && (AND12.u128 == 0) ) {
        FLAGS |= 0x00000040; //set ZF = 1
    } else {
        FLAGS = (FLAGS | 0x00000040) ^ 0x00000040;  //set ZF = 0
    }
    
    if  ( (AND21.u128 == 0) && (AND22.u128 == 0) )  {
        FLAGS |= 0x00000001; //set CF = 1
    } else {
        FLAGS = (FLAGS | 0x00000001) ^ 0x00000001;  //set CF = 0
    }

    FLAGS = (FLAGS | 0x00000800) ^ 0x00000800;  //set OF = 0
    FLAGS = (FLAGS | 0x00000010) ^ 0x00000010;  //set AF = 0
    FLAGS = (FLAGS | 0x00000080) ^ 0x00000080;  //set SF = 0
    FLAGS = (FLAGS | 0x00000004) ^ 0x00000004;  //set PF = 0
    
    if (is_saved_state64(state)) {
        saved_state64(state)->isf.rflags = FLAGS;
    } else {
        EFLAGS = FLAGS & 0xFFFFFFFF;
        saved_state32(state)->efl = EFLAGS;
    }
}

inline void vmpsadbw_128(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int i;
    int8_t X;
    int blk1 = (imm >> 2) & 1 ;  //imm8[2]
    int blk2 = imm & 3;          //imm8[1:0]
    XMM src1_byte, src2_byte;
    uint8_t temp0, temp1, temp2, temp3;
    
    src1_byte.u32[0] = vsrc.u32[blk1];
    src1_byte.u32[0] = vsrc.u32[blk1+1];
    src1_byte.u32[0] = vsrc.u32[blk1+2];
    
    src2_byte.u32[0] = src.u32[blk2];
    
    for (i = 0; i < 8; ++i) {
        X = src1_byte.a8[i] - src2_byte.a8[0];
        temp0 = ABS(X);
        X = src1_byte.a8[1+i] - src2_byte.a8[1];
        temp1 = ABS(X);
        X = src1_byte.a8[2+i] - src2_byte.a8[2];
        temp2 = ABS(X);
        X = src1_byte.a8[3+i] - src2_byte.a8[3];
        temp3 = ABS(X);
        res->u16[i] = temp0 + temp1 + temp2 + temp3;
    }
}

inline void vmpsadbw_256(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int i;
    int8_t X;
    int blk1;
    int blk2;
    XMM src1, src2, src1_byte, src2_byte;
    XMM dest;
    uint8_t temp0, temp1, temp2, temp3;
    
    //[127:0] bit
    blk1 = (imm >> 2) & 1; //imm8[2]
    blk2 = imm & 3;        //imm8[1:0]
    src1.u128 = vsrc.u128[0];
    src2.u128 = src.u128[0];
    
    src1_byte.u32[0] = src1.u32[blk1];
    src1_byte.u32[0] = src1.u32[blk1+1];
    src1_byte.u32[0] = src1.u32[blk1+2];
    
    src2_byte.u32[0] = src2.u32[blk2];
    
    for (i = 0; i < 8; ++i) {
        X = src1_byte.a8[i] - src2_byte.a8[0];
        temp0 = ABS(X);
        X = src1_byte.a8[i+1] - src2_byte.a8[1];
        temp1 = ABS(X);
        X = src1_byte.a8[i+2] - src2_byte.a8[2];
        temp2 = ABS(X);
        X = src1_byte.a8[i+3] - src2_byte.a8[3];
        temp3 = ABS(X);
        dest.u16[i] = temp0 + temp1 + temp2 + temp3;
    }
    
    res->u128[0] = dest.u128;
    
    //[255:128] bit
    blk1 = (imm >> 5) & 1; //imm8[5]
    blk2 = (imm >> 3) & 3; //imm8[4:3]

    src1.u128 = vsrc.u128[1];
    src2.u128 = src.u128[1];

    src1_byte.u32[0] = src1.u32[blk1];
    src1_byte.u32[0] = src1.u32[blk1+1];
    src1_byte.u32[0] = src1.u32[blk1+2];
    
    src2_byte.u32[0] = src2.u32[blk2];

    for (i = 0; i < 8; ++i) {
        X = src1_byte.a8[i] - src2_byte.a8[0];
        temp0 = ABS(X);
        X = src1_byte.a8[i+1] - src2_byte.a8[1];
        temp1 = ABS(X);
        X = src1_byte.a8[i+2] - src2_byte.a8[2];
        temp2 = ABS(X);
        X = src1_byte.a8[i+3] - src2_byte.a8[3];
        temp3 = ABS(X);
        dest.u16[i] = temp0 + temp1 + temp2 + temp3;
    }
    
    res->u128[1] = dest.u128;
}

#endif /* vsse41_hpp */
