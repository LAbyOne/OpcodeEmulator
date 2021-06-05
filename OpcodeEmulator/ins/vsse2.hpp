//
//  vsse2.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/8.
//  Copyright © 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef vsse2_hpp
#define vsse2_hpp

#include "opemu.hpp"
#include "fpins.hpp"

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
                      int kernel_trap);

int maxdf(double SRC1, double SRC2);
int mindf(double SRC1, double SRC2);
int cmpoptdf(double SRC1, double SRC2, uint8_t immvar);

/**********************************************/
/**  VSSE2  instructions implementation      **/
/**********************************************/
/************* Move *************/
inline void vmovdqu_128a(XMM src, XMM *res) {
    res->u128 = src.u128;
}
inline void vmovdqu_128b(XMM dst, XMM *res) {
    res->u128 = dst.u128;
}

inline void vmovdqu_256a(YMM src, YMM *res) {
    res->u256 = src.u256;
}
inline void vmovdqu_256b(YMM dst, YMM *res) {
    res->u256 = dst.u256;
}

inline void vmovda(XMM src, XMM *res, uint8_t operand_size) {
    if (operand_size == 64) {
        //VMOVQ
        res->u64[0] = src.u64[0];
        res->u64[1] = 0;
    } else {
        //VMOVD
        res->u32[0] = src.u32[0];
        res->u32[1] = 0;
        res->u32[2] = 0;
        res->u32[3] = 0;
    }
}

inline void vmovdb(XMM dst, XMM *res, uint8_t operand_size) {
    if (operand_size == 64) {
        //VMOVQ
        res->u64[0] = dst.u64[0];
        res->u64[1] = 0;
    } else {
        //VMOVD
        res->u32[0] = dst.u32[0];
        res->u32[1] = 0;
        res->u32[2] = 0;
        res->u32[3] = 0;
    }
}

inline void vmovupd_128a(XMM src, XMM *res) {
    res->fa64[0] = src.fa64[0];
    res->fa64[1] = src.fa64[1];
}
inline void vmovupd_128b(XMM dst, XMM *res) {
    res->fa64[0] = dst.fa64[0];
    res->fa64[1] = dst.fa64[1];
}

inline void vmovupd_256a(YMM src, YMM *res) {
    res->fa64[0] = src.fa64[0];
    res->fa64[1] = src.fa64[1];
    res->fa64[2] = src.fa64[2];
    res->fa64[3] = src.fa64[3];
}
inline void vmovupd_256b(YMM dst, YMM *res) {
    res->fa64[0] = dst.fa64[0];
    res->fa64[1] = dst.fa64[1];
    res->fa64[2] = dst.fa64[2];
    res->fa64[3] = dst.fa64[3];
}

inline void vmovsd_128a(XMM src, XMM vsrc, XMM *res) {
    
    res->fa64[0] = src.fa64[0];
    res->fa64[1] = vsrc.fa64[1];
}
inline void vmovsd_128b(XMM src, XMM *res) {
    
    res->fa64[0] = src.fa64[0];
    res->fa64[1] = 0;
}

inline void vmovlpd_128a(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = src.fa64[0];
    res->fa64[1] = vsrc.fa64[1];
}
inline void vmovlpd_128b(XMM dst, XMM *res) {
    res->fa64[0] = dst.fa64[0];
}

inline void vmovhpd_128a(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = vsrc.fa64[0];
    res->fa64[1] = src.fa64[0];
}
inline void vmovhpd_128b(XMM dst, XMM *res) {
    res->fa64[0] = dst.fa64[1];
}

//MASK = src
//SRC = dst
//DEST = [RDI]
inline void vmaskmovdqu(XMM src, XMM dst, XMM *res, XMM var) {
    int i;
    int xbit;
    uint8_t MASK;

    res->u128 = var.u128;

    for (i = 0; i < 16; ++i) {
        MASK = src.u8[i];
        xbit = (MASK >> 7) & 1;
        if (xbit == 1) {
            res->u8[i] = dst.u8[i];
        }
    }
}

inline void vmovntdq_128(XMM dst, XMM *res) {
    res->u128 = dst.u128;
}
inline void vmovntdq_256(YMM dst, YMM *res) {
    res->u128[0] = dst.u128[0];
    res->u128[1] = dst.u128[1];
}

inline void vmovmskpd_128(XMM src, XMM *res) {
    int i;
    int xbit;
    uint64_t mask;
    uint64_t dest = 0;
    
    for (i = 0; i < 2; ++i) {
        mask = src.u64[i];
        xbit = (mask >> 63) & 1;
        dest |= (xbit << i);
    }
    res->u64[0] = dest;
}
inline void vmovmskpd_256(YMM src, YMM *res) {
    int i;
    int xbit;
    uint64_t mask;
    uint64_t dest = 0;
    
    for (i = 0; i < 4; ++i) {
        mask = src.u64[i];
        xbit = (mask >> 63) & 1;
        dest |= (xbit << i);
    }
    res->u64[0] = dest;
}

inline void vpmovmskb_128(XMM src, XMM *res) {
    int i;
    int xbit;
    uint8_t mask;
    uint64_t dest = 0;

    for (i = 0; i < 16; ++i) {
        mask = src.u8[i];
        xbit = (mask >> 7) & 1;
        dest |= (xbit << i);
    }
    res->u64[0] = dest;
}
inline void vpmovmskb_256(YMM src, YMM *res) {
    int i;
    int xbit;
    uint8_t mask;
    uint64_t dest = 0;
    
    for (i = 0; i < 32; ++i) {
        mask = src.u8[i];
        xbit = (mask >> 7) & 1;
        dest |= (xbit << i);
    }
    res->u64[0] = dest;

}

/************* Converts integers byte/word *************/
inline void vpacksswb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int16_t var = 0;
    XMM TEMP1, TEMP2;
    
    for (i = 0; i < 8; ++i) {
        var = STSB(vsrc.a16[i]);
        TEMP1.a8[i] = var;
    }
    
    for (i = 0; i < 8; ++i) {
        var = STSB(src.a16[i]);
        TEMP2.a8[i] = var;
    }
    res->a64[0] = TEMP1.a64[0];
    res->a64[1] = TEMP2.a64[0];
}
inline void vpacksswb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int16_t var = 0;
    XMM TEMP1, TEMP2;
    
    for (i = 0; i < 16; ++i) {
        var = STSB(vsrc.a16[i]);
        TEMP1.a8[i] = var;
    }
    
    for (i = 0; i < 16; ++i) {
        var = STSB(src.a16[i]);
        TEMP2.a8[i] = var;
    }
    
    res->a64[0] = TEMP1.a64[0];
    res->a64[1] = TEMP2.a64[0];
    res->a64[2] = TEMP1.a64[1];
    res->a64[3] = TEMP2.a64[1];
}
inline void vpackuswb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int16_t var = 0;
    XMM TEMP1, TEMP2;
    
    for (i = 0; i < 8; ++i) {
        var = STUB(vsrc.a16[i]);
        TEMP1.u8[i] = var;
    }
    
    for (i = 0; i < 8; ++i) {
        var = STUB(src.a16[i]);
        TEMP2.u8[i] = var;
    }
    res->u64[0] = TEMP1.u64[0];
    res->u64[1] = TEMP2.u64[0];
}
inline void vpackuswb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int16_t var = 0;
    XMM TEMP1, TEMP2;
    
    for (i = 0; i < 16; ++i) {
        var = STUB(vsrc.a16[i]);
        TEMP1.u8[i] = var;
    }
    
    for (i = 0; i < 16; ++i) {
        var = STUB(src.a16[i]);
        TEMP2.u8[i] = var;
    }
    
    res->u64[0] = TEMP1.u64[0];
    res->u64[1] = TEMP2.u64[0];
    res->u64[2] = TEMP1.u64[1];
    res->u64[3] = TEMP2.u64[1];
}
inline void vpackssdw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int32_t var = 0;
    XMM TEMP1, TEMP2;

    for (i = 0; i < 4; ++i) {
        var = STSW(vsrc.a32[i]);
        TEMP1.a16[i] = var;
    }
    for (i = 0; i < 4; ++i) {
        var = STSW(src.a32[i]);
        TEMP2.a16[i] = var;
    }

    res->a64[0] = TEMP1.a64[0];
    res->a64[1] = TEMP2.a64[0];
}
inline void vpackssdw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int32_t var = 0;
    XMM TEMP1, TEMP2;
    
    for (i = 0; i < 8; ++i) {
        var = STSW(vsrc.a32[i]);
        TEMP1.a16[i] = var;
    }
    for (i = 0; i < 8; ++i) {
        var = STSW(src.a32[i]);
        TEMP2.a16[i] = var;
    }
    
    res->a64[0] = TEMP1.a64[0];
    res->a64[1] = TEMP2.a64[0];
    res->a64[2] = TEMP1.a64[1];
    res->a64[3] = TEMP2.a64[1];
}

/************* Converts floating-point *************/
inline void vcvtdq2pd_128(XMM src, XMM *res) {
    int i;
    int32_t ix = 0;
    double df = 0;
    
    for (i = 0; i < 2; ++i) {
        ix = src.a32[i];
        df = ix;
        res->fa64[i] = df;
    }
}
inline void vcvtdq2pd_256(XMM src, YMM *res) {
    int i;
    int32_t ix = 0;
    double df = 0;
    
    for (i = 0; i < 4; ++i) {
        ix = src.a32[i];
        df = ix;
        res->fa64[i] = df;
    }
}

inline void vcvtdq2ps_128(XMM src, XMM *res) {
    int i;
    int32_t ix = 0;
    float f = 0;
    
    for (i = 0; i < 4; ++i) {
        ix = src.a32[i];
        f = ix;
        res->fa32[i] = f;
    }
}
inline void vcvtdq2ps_256(YMM src, YMM *res) {
    int i;
    int32_t ix = 0;
    float f = 0;
    
    for (i = 0; i < 8; ++i) {
        ix = src.a32[i];
        f = ix;
        res->fa32[i] = f;
    }
}

inline void vcvtpd2dq_128(XMM src, XMM *res, int rc) {
    int i;
    int32_t ix = 0;
    double df = 0;
    
    for (i = 0; i < 2; ++i) {
        df = round_fp64(src.fa64[i], rc);
        ix = df;
        res->a32[i] = ix;
    }
}
inline void vcvtpd2dq_256(YMM src, XMM *res, int rc) {
    int i;
    int32_t ix = 0;
    double df = 0;
    
    for (i = 0; i < 4; ++i) {
        df = round_fp64(src.fa64[i], rc);
        ix = df;
        res->a32[i] = ix;
    }
}

inline void vcvtpd2ps_128(XMM src, XMM *res) {
    int i;
    float f = 0;
    
    for (i = 0; i < 2; ++i) {
        f = src.fa64[i];
        res->fa32[i] = f;
    }
}
inline void vcvtpd2ps_256(YMM src, YMM *res) {
    int i;
    float f = 0;
    
    for (i = 0; i < 4; ++i) {
        f = src.fa64[i];
        res->fa32[i] = f;
    }
}

inline void vcvtps2dq_128(XMM src, XMM *res, int rc) {
    int i;
    int32_t ix = 0;
    float f = 0;
    
    for (i = 0; i < 4; ++i) {
        f = round_fp32(src.fa32[i], rc);
        ix = f;
        res->a32[i] = ix;
    }
}
inline void vcvtps2dq_256(YMM src, YMM *res, int rc) {
    int i;
    int32_t ix = 0;
    float f = 0;
    
    for (i = 0; i < 8; ++i) {
        f = round_fp32(src.fa32[i], rc);
        ix = f;
        res->a32[i] = ix;
    }
}

inline void vcvtps2pd_128(XMM src, XMM *res) {
    int i;
    float f = 0;
    
    for (i = 0; i < 2; ++i) {
        f = src.fa32[i];
        res->fa64[i] = f;
    }
}
inline void vcvtps2pd_256(XMM src, YMM *res) {
    int i;
    float f = 0;
    
    for (i = 0; i < 4; ++i) {
        f = src.fa32[i];
        res->fa64[i] = f;
    }
}

inline void vcvtsd2si(XMM src, XMM *res, int rc, uint8_t operand_size) {
    double df = 0;

    if (operand_size == 64) {
        int64_t ix = 0;
        df = round_fp64(src.fa64[0], rc);
        ix= df;
        res->a64[0] = ix;
    } else {
        int32_t ix = 0;
        df = round_fp64(src.fa64[0], rc);
        ix = df;
        res->a32[0] = ix;
        res->a32[1] = 0;
    }
}
inline void vcvtsd2ss(XMM src, XMM vsrc, XMM *res) {
    double df = 0;
    float f = 0;
    res->u128 = vsrc.u128;
    
    df = src.fa64[0];
    f = df;
    res->fa32[0] = f;
}

inline void vcvtsi2sd(XMM src, XMM vsrc, XMM *res, uint8_t operand_size) {
    double df = 0;
    res->u128 = vsrc.u128;
    
    if (operand_size == 64) {
        int64_t ix = src.a64[0];
        df = ix;
        res->fa64[0] = df;
    } else {
        int32_t ix = src.a32[0];
        df = ix;
        res->fa64[0] = df;
    }
}
inline void vcvtss2sd(XMM src, XMM vsrc, XMM *res) {
    double df = 0;
    float f = 0;
    res->u128 = vsrc.u128;
    
    f = src.fa32[0];
    df = f;
    res->fa64[0] = df;
}

/************* Computes *************/
inline void vpsadbw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int8_t y = 0;
    uint8_t x = 0;
    uint16_t sum = 0;
    XMM tmp;
    
    for (i = 0; i < 16; ++i) {
        y = vsrc.a8[i] - src.a8[i];
        x = ABS(y);
        tmp.u8[i] = x;
    }
    sum = tmp.u8[0] + tmp.u8[1] + tmp.u8[2] + tmp.u8[3] + tmp.u8[4] + tmp.u8[5] + tmp.u8[6] + tmp.u8[7];
    res->u16[0] = sum;
    res->u16[1] = 0;
    res->u16[2] = 0;
    res->u16[3] = 0;
    sum = tmp.u8[8] + tmp.u8[9] + tmp.u8[10] + tmp.u8[11] + tmp.u8[12] + tmp.u8[13] + tmp.u8[14] + tmp.u8[15];
    res->u16[4] = sum;
    res->u16[5] = 0;
    res->u16[6] = 0;
    res->u16[7] = 0;
}

inline void vpsadbw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int8_t y = 0;
    uint8_t x = 0;
    uint16_t sum = 0;
    YMM tmp;
    
    for (i = 0; i < 32; ++i) {
        y = vsrc.a8[i] - src.a8[i];
        x = ABS(y);
        tmp.u8[i] = x;
    }
    sum = tmp.u8[0] + tmp.u8[1] + tmp.u8[2] + tmp.u8[3] + tmp.u8[4] + tmp.u8[5] + tmp.u8[6] + tmp.u8[7];
    res->u16[0] = sum;
    res->u16[1] = 0;
    res->u16[2] = 0;
    res->u16[3] = 0;
    sum = tmp.u8[8] + tmp.u8[9] + tmp.u8[10] + tmp.u8[11] + tmp.u8[12] + tmp.u8[13] + tmp.u8[14] + tmp.u8[15];
    res->u16[4] = sum;
    res->u16[5] = 0;
    res->u16[6] = 0;
    res->u16[7] = 0;
    sum = tmp.u8[16] + tmp.u8[17] + tmp.u8[18] + tmp.u8[19] + tmp.u8[20] + tmp.u8[21] + tmp.u8[22] + tmp.u8[23];
    res->u16[8] = sum;
    res->u16[9] = 0;
    res->u16[10] = 0;
    res->u16[11] = 0;
    sum = tmp.u8[24] + tmp.u8[25] + tmp.u8[16] + tmp.u8[27] + tmp.u8[28] + tmp.u8[29] + tmp.u8[30] + tmp.u8[31];
    res->u16[12] = sum;
    res->u16[13] = 0;
    res->u16[14] = 0;
    res->u16[15] = 0;

}

inline void vsqrtpd_128(XMM src, XMM *res) {
    int i;
    double df;

    for (i = 0; i < 2; ++i) {
        df = sqrt_df(src.fa64[i]);
        res->fa64[i] = df;
    }
}

inline void vsqrtpd_256(YMM src, YMM *res) {
    int i;
    double df;
    
    for (i = 0; i < 4; ++i) {
        df = sqrt_df(src.fa64[i]);
        res->fa64[i] = df;
    }
}

inline void vsqrtsd(XMM src, XMM vsrc, XMM *res) {
    double df;
    df = sqrt_df(src.fa64[0]);
    res->fa64[0] = df;
    res->fa64[1] = vsrc.fa64[1];
}

/************* Insert *************/
inline void vpinsrw(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int sel = imm & 7;
    uint16_t word = src.u16[0];
    
    res->u128 = vsrc.u128;
    res->u16[sel] = word;
}

/************* ADD *************/
inline void vpaddb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    
    for (i = 0; i < 16; ++i) {
        res->u8[i] = vsrc.u8[i] + src.u8[i];
    }
}
inline void vpaddb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    
    for (i = 0; i < 32; ++i) {
        res->u8[i] = vsrc.u8[i] + src.u8[i];
    }
}

inline void vpaddw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i) {
        res->u16[i] = vsrc.u16[i] + src.u16[i];
    }
}
inline void vpaddw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    
    for (i = 0; i < 16; ++i) {
        res->u16[i] = vsrc.u16[i] + src.u16[i];
    }
}

inline void vpaddd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->u32[i] = vsrc.u32[i] + src.u32[i];
    }
}
inline void vpaddd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i) {
        res->u32[i] = vsrc.u32[i] + src.u32[i];
    }
}

inline void vpaddq_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    
    for (i = 0; i < 2; ++i) {
        res->u64[i] = vsrc.u64[i] + src.u64[i];
    }
}
inline void vpaddq_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->u64[i] = vsrc.u64[i] + src.u64[i];
    }
}

inline void vpaddsb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int16_t var;

    for (i = 0; i < 16; ++i) {
        var = vsrc.a8[i] + src.a8[i];
        res->a8[i] = STSB(var);
    }
}
inline void vpaddsb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int16_t var;

    for (i = 0; i < 32; ++i) {
        var = vsrc.a8[i] + src.a8[i];
        res->a8[i] = STSB(var);
    }
}
inline void vpaddsw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int32_t var;

    for (i = 0; i < 8; ++i) {
        var = vsrc.a16[i] + src.a16[i];
        res->a16[i] = STSW(var);
    }
}
inline void vpaddsw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int32_t var;
    
    for (i = 0; i < 16; ++i) {
        var = vsrc.a16[i] + src.a16[i];
        res->a16[i] = STSW(var);
    }
}
inline void vpaddusb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    uint16_t var;
    
    for (i = 0; i < 16; ++i) {
        var = vsrc.u8[i] + src.u8[i];
        res->u8[i] = STUB(var);
    }
}
inline void vpaddusb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    uint16_t var;
    
    for (i = 0; i < 32; ++i) {
        var = vsrc.u8[i] + src.u8[i];
        res->u8[i] = STUB(var);
    }
}
inline void vpaddusw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    uint32_t var;
    
    for (i = 0; i < 8; ++i) {
        var = vsrc.u16[i] + src.u16[i];
        res->u16[i] = STUW(var);
    }
}
inline void vpaddusw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    uint32_t var;
    
    for (i = 0; i < 16; ++i) {
        var = vsrc.u16[i] + src.u16[i];
        res->u16[i] = STUW(var);
    }
}
/************* SUB *************/
inline void vpsubb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    
    for (i = 0; i < 16; ++i) {
        res->a8[i] = vsrc.a8[i] - src.a8[i];
    }
}
inline void vpsubb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    
    for (i = 0; i < 32; ++i) {
        res->a8[i] = vsrc.a8[i] - src.a8[i];
    }
}
inline void vpsubw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i) {
        res->a16[i] = vsrc.a16[i] - src.a16[i];
    }
}
inline void vpsubw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    
    for (i = 0; i < 16; ++i) {
        res->a16[i] = vsrc.a16[i] - src.a16[i];
    }
}
inline void vpsubd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->a32[i] = vsrc.a32[i] - src.a32[i];
    }
}
inline void vpsubd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    
    for (i = 0; i < 8; ++i) {
        res->a32[i] = vsrc.a32[i] - src.a32[i];
    }
}
inline void vpsubq_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    
    for (i = 0; i < 2; ++i) {
        res->a64[i] = vsrc.a64[i] - src.a64[i];
    }
}
inline void vpsubq_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->a64[i] = vsrc.a64[i] - src.a64[i];
    }
}
inline void vpsubsb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int8_t var;
    
    for (i = 0; i < 16; ++i) {
        var = vsrc.a8[i] - src.a8[i];
        res->a8[i] = STSB(var);
    }
}
inline void vpsubsb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int8_t var;
    
    for (i = 0; i < 32; ++i) {
        var = vsrc.a8[i] - src.a8[i];
        res->a8[i] = STSB(var);
    }
}
inline void vpsubsw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int16_t var;
    
    for (i = 0; i < 8; ++i) {
        var = vsrc.a16[i] - src.a16[i];
        res->a16[i] = STSW(var);
    }
}
inline void vpsubsw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int16_t var;
    
    for (i = 0; i < 16; ++i) {
        var = vsrc.a16[i] - src.a16[i];
        res->a16[i] = STSW(var);
    }
}
inline void vpsubusb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    uint8_t var;
    
    for (i = 0; i < 16; ++i) {
        var = vsrc.u8[i] - src.u8[i];
        res->u8[i] = STUB(var);
    }
}
inline void vpsubusb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    uint8_t var;
    
    for (i = 0; i < 32; ++i) {
        var = vsrc.u8[i] - src.u8[i];
        res->u8[i] = STUB(var);
    }
}
inline void vpsubusw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    uint16_t var;
    
    for (i = 0; i < 8; ++i) {
        var = vsrc.u16[i] - src.u16[i];
        res->u16[i] = STUW(var);
    }
}
inline void vpsubusw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    uint16_t var;
    
    for (i = 0; i < 16; ++i) {
        var = vsrc.u16[i] - src.u16[i];
        res->u16[i] = STUW(var);
    }
}
inline void vsubpd_128(XMM src, XMM vsrc, XMM *res) {
    int i;

    for (i = 0; i < 2; ++i) {
        res->fa64[i] = vsrc.fa64[i] - src.fa64[i];
    }
}
inline void vsubpd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->fa64[i] = vsrc.fa64[i] - src.fa64[i];
    }
}
inline void vsubsd(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = vsrc.fa64[0] - src.fa64[0];
    res->fa64[1] = vsrc.fa64[1];
}
/************* Multiply *************/
inline void vmulpd_128(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = vsrc.fa64[0] * src.fa64[0];
    res->fa64[1] = vsrc.fa64[1] * src.fa64[1];

}
inline void vmulpd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    
    for (i = 0; i < 4; ++i) {
        res->fa64[i] = vsrc.fa64[i] * src.fa64[i];
    }
}
inline void vmulsd(XMM src, XMM vsrc, XMM *res) {
    res->fa64[1] = vsrc.fa64[1];
    res->fa64[0] = vsrc.fa64[0] * src.fa64[0];
}
inline void vpmaddwd_128(XMM src, XMM vsrc, XMM *res) {
    res->u32[0] = (vsrc.u16[0] * src.u16[0]) + (vsrc.u16[1] * src.u16[1]);
    res->u32[1] = (vsrc.u16[2] * src.u16[2]) + (vsrc.u16[3] * src.u16[3]);
    res->u32[2] = (vsrc.u16[4] * src.u16[4]) + (vsrc.u16[5] * src.u16[5]);
    res->u32[3] = (vsrc.u16[6] * src.u16[6]) + (vsrc.u16[7] * src.u16[7]);
}
inline void vpmaddwd_256(YMM src, YMM vsrc, YMM *res) {
    res->u32[0] = (vsrc.u16[0] * src.u16[0]) + (vsrc.u16[1] * src.u16[1]);
    res->u32[1] = (vsrc.u16[2] * src.u16[2]) + (vsrc.u16[3] * src.u16[3]);
    res->u32[2] = (vsrc.u16[4] * src.u16[4]) + (vsrc.u16[5] * src.u16[5]);
    res->u32[3] = (vsrc.u16[6] * src.u16[6]) + (vsrc.u16[7] * src.u16[7]);
    res->u32[4] = (vsrc.u16[8] * src.u16[8]) + (vsrc.u16[9] * src.u16[9]);
    res->u32[5] = (vsrc.u16[10] * src.u16[10]) + (vsrc.u16[11] * src.u16[11]);
    res->u32[6] = (vsrc.u16[12] * src.u16[12]) + (vsrc.u16[13] * src.u16[13]);
    res->u32[7] = (vsrc.u16[14] * src.u16[14]) + (vsrc.u16[15] * src.u16[15]);
}
inline void vpmulhuw_128(XMM src, XMM vsrc, XMM *res) {
    XMM tmp;
    int i;
    
    for (i = 0; i < 8; ++i) {
        tmp.u32[0] = vsrc.u16[i] * src.u16[i];
        res->u16[i] = tmp.u16[1];
    }
}
inline void vpmulhuw_256(YMM src, YMM vsrc, YMM *res) {
    XMM tmp;
    int i;
    
    for (i = 0; i < 16; ++i) {
        tmp.u32[0] = vsrc.u16[i] * src.u16[i];
        res->u16[i] = tmp.u16[1];
    }
}
inline void vpmulhw_128(XMM src, XMM vsrc, XMM *res) {
    XMM tmp;
    int i;
    
    for (i = 0; i < 8; ++i) {
        tmp.a32[0] = vsrc.a16[i] * src.a16[i];
        res->a16[i] = tmp.a16[1];
    }
}
inline void vpmulhw_256(YMM src, YMM vsrc, YMM *res) {
    XMM tmp;
    int i;
    
    for (i = 0; i < 16; ++i) {
        tmp.a32[0] = vsrc.a16[i] * src.a16[i];
        res->a16[i] = tmp.a16[1];
    }
}
inline void vpmullw_128(XMM src, XMM vsrc, XMM *res) {
    XMM tmp;
    int i;
    
    for (i = 0; i < 8; ++i) {
        tmp.a32[0] = vsrc.a16[i] * src.a16[i];
        res->a16[i] = tmp.a16[0];
    }
}
inline void vpmullw_256(YMM src, YMM vsrc, YMM *res) {
    XMM tmp;
    int i;
    
    for (i = 0; i < 16; ++i) {
        tmp.a32[0] = vsrc.a16[i] * src.a16[i];
        res->a16[i] = tmp.a16[0];
    }
}
inline void vpmuludq_128(XMM src, XMM vsrc, XMM *res) {
    res->u64[0] = vsrc.u32[0] * src.u32[0];
    res->u64[1] = vsrc.u32[2] * src.u32[2];
}
inline void vpmuludq_256(YMM src, YMM vsrc, YMM *res) {
    res->u64[0] = vsrc.u32[0] * src.u32[0];
    res->u64[1] = vsrc.u32[2] * src.u32[2];
    res->u64[2] = vsrc.u32[4] * src.u32[4];
    res->u64[3] = vsrc.u32[6] * src.u32[6];
}

/************* Divide *************/
inline void vdivpd_128(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = vsrc.fa64[0] / src.fa64[0];
    res->fa64[1] = vsrc.fa64[1] / src.fa64[1];
}
inline void vdivpd_256(YMM src, YMM vsrc, YMM *res) {
    res->fa64[0] = vsrc.fa64[0] / src.fa64[0];
    res->fa64[1] = vsrc.fa64[1] / src.fa64[1];
    res->fa64[2] = vsrc.fa64[2] / src.fa64[2];
    res->fa64[3] = vsrc.fa64[3] / src.fa64[3];
}
inline void vdivsd(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = vsrc.fa64[0] / src.fa64[0];
    res->fa64[1] = vsrc.fa64[1];
}

/************* AND *************/
inline void vandpd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    XMM tmp;
    
    for (i = 0; i < 2; ++i) {
        tmp.u64[0] = vsrc.u64[i] & src.u64[i];
        res->fa64[i] = tmp.fa64[0];
    }
}
inline void vandpd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    XMM tmp;
    
    for (i = 0; i < 4; ++i) {
        tmp.u64[0] = vsrc.u64[i] & src.u64[i];
        res->fa64[i] = tmp.fa64[0];
    }
}
inline void vandnpd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    XMM tmp;
    
    for (i = 0; i < 2; ++i) {
        tmp.u64[0] = (~(vsrc.u64[i])) & src.u64[i];
        res->fa64[i] = tmp.fa64[0];
    }
}
inline void vandnpd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    XMM tmp;
    
    for (i = 0; i < 4; ++i) {
        tmp.u64[0] = (~(vsrc.u64[i])) & src.u64[i];
        res->fa64[i] = tmp.fa64[0];
    }
}
inline void vpand_128(XMM src, XMM vsrc, XMM *res) {
    res->u128 = vsrc.u128 & src.u128;
}
inline void vpand_256(YMM src, YMM vsrc, YMM *res) {
    res->u128[0] = vsrc.u128[0] & src.u128[0];
    res->u128[1] = vsrc.u128[1] & src.u128[1];
}
inline void vpandn_128(XMM src, XMM vsrc, XMM *res) {
    res->u128 = (~(vsrc.u128)) & src.u128;
}
inline void vpandn_256(YMM src, YMM vsrc, YMM *res) {
    res->u128[0] = (~(vsrc.u128[0])) & src.u128[0];
    res->u128[1] = (~(vsrc.u128[1])) & src.u128[1];
}

/************* OR/XOR *************/
inline void vorpd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    XMM tmp;
    
    for (i = 0; i < 2; ++i) {
        tmp.u64[0] = vsrc.u64[i] | src.u64[i];
        res->fa64[i] = tmp.fa64[0];
    }
}
inline void vorpd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    XMM tmp;
    
    for (i = 0; i < 4; ++i) {
        tmp.u64[0] = vsrc.u64[i] | src.u64[i];
        res->fa64[i] = tmp.fa64[0];
    }
}
inline void vxorpd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    XMM tmp;
    
    for (i = 0; i < 2; ++i) {
        tmp.u64[0] = vsrc.u64[i] ^ src.u64[i];
        res->fa64[i] = tmp.fa64[0];
    }
}
inline void vxorpd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    XMM tmp;
    
    for (i = 0; i < 4; ++i) {
        tmp.u64[0] = vsrc.u64[i] ^ src.u64[i];
        res->fa64[i] = tmp.fa64[0];
    }
}
inline void vpor_128(XMM src, XMM vsrc, XMM *res) {
    res->u128 = vsrc.u128 | src.u128;
}
inline void vpor_256(YMM src, YMM vsrc, YMM *res) {
    res->u128[0] = vsrc.u128[0] | src.u128[0];
    res->u128[1] = vsrc.u128[1] | src.u128[1];
}
inline void vpxor_128(XMM src, XMM vsrc, XMM *res) {
    res->u128 = vsrc.u128 ^ src.u128;
}
inline void vpxor_256(YMM src, YMM vsrc, YMM *res) {
    res->u128[0] = vsrc.u128[0] ^ src.u128[0];
    res->u128[1] = vsrc.u128[1] ^ src.u128[1];
}

/************* Average *************/
inline void vpavgb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    uint16_t tmp;
    for (i = 0; i < 16; ++i) {
        tmp = (vsrc.u8[i] + src.u8[i] + 1) >> 1;
        res->u8[i] = tmp & 0xff;
    }
}
inline void vpavgb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    uint16_t tmp;
    for (i = 0; i < 32; ++i) {
        tmp = (vsrc.u8[i] + src.u8[i] + 1) >> 1;
        res->u8[i] = tmp & 0xff;
    }
}
inline void vpavgw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    uint32_t tmp;
    for (i = 0; i < 8; ++i) {
        tmp = (vsrc.u16[i] + src.u16[i] + 1) >> 1;
        res->u16[i] = tmp & 0xffff;
    }
}
inline void vpavgw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    uint32_t tmp;
    for (i = 0; i < 16; ++i) {
        tmp = (vsrc.u16[i] + src.u16[i] + 1) >> 1;
        res->u16[i] = tmp & 0xffff;
    }
}
/************* Shuffle *************/
inline void vpshufd_128(XMM src, XMM *res, uint8_t imm) {
    int i;
    int order;
    
    for (i = 0; i < 4; ++i) {
        order = (imm >> (i * 2)) & 3;
        res->u32[i] = src.u32[order];
    }
}
inline void vpshufd_256(YMM src, YMM *res, uint8_t imm) {
    int i;
    int order;

    for (i = 0; i < 4; ++i) {
        order = (imm >> (i * 2)) & 3;
        res->u32[i] = src.u32[order];
    }
    for (i = 0; i < 4; ++i) {
        order = (imm >> (i * 2)) & 3;
        res->u32[i+4] = src.u32[order];
    }
}
inline void vpshufhw_128(XMM src, XMM *res, uint8_t imm) {
    int i;
    int order;
    
    XMM TMP, SRC;
    SRC.u64[0] = src.u64[1];
    
    for (i = 0; i < 4; ++i) {
        order = (imm >> (i * 2)) & 3;
        TMP.u16[i] = SRC.u16[order];
    }
    res->u64[0] = src.u64[0];
    res->u64[1] = TMP.u64[0];
}
inline void vpshufhw_256(YMM src, YMM *res, uint8_t imm) {
    int i;
    int order;
    
    XMM TMP1, TMP2, SRC;
    SRC.u64[0] = src.u64[1];
    
    for (i = 0; i < 4; ++i) {
        order = (imm >> (i * 2)) & 3;
        TMP1.u16[i] = SRC.u16[order];
    }
    SRC.u64[0] = src.u64[3];

    for (i = 0; i < 4; ++i) {
        order = (imm >> (i * 2)) & 3;
        TMP2.u16[i] = SRC.u16[order];
    }
    
    res->u64[0] = src.u64[0];
    res->u64[1] = TMP1.u64[0];
    res->u64[2] = src.u64[2];
    res->u64[3] = TMP2.u64[0];
}
inline void vpshuflw_128(XMM src, XMM *res, uint8_t imm) {
    int i;
    int order;
    
    XMM TMP, SRC;
    SRC.u64[0] = src.u64[0];
    
    for (i = 0; i < 4; ++i) {
        order = (imm >> (i * 2)) & 3;
        TMP.u16[i] = SRC.u16[order];
    }
    res->u64[0] = TMP.u64[0];
    res->u64[1] = src.u64[1];
}
inline void vpshuflw_256(YMM src, YMM *res, uint8_t imm) {
    int i;
    int order;
    
    XMM TMP1, TMP2, SRC;
    SRC.u64[0] = src.u64[0];
    
    for (i = 0; i < 4; ++i) {
        order = (imm >> (i * 2)) & 3;
        TMP1.u16[i] = SRC.u16[order];
    }
    SRC.u64[0] = src.u64[2];
    
    for (i = 0; i < 4; ++i) {
        order = (imm >> (i * 2)) & 3;
        TMP2.u16[i] = SRC.u16[order];
    }
    
    res->u64[0] = TMP1.u64[0];
    res->u64[1] = src.u64[1];
    res->u64[2] = TMP2.u64[0];
    res->u64[3] = src.u64[3];
}

// Left Logical
inline void vpsllw_128(XMM src, XMM *res, uint8_t count) {
    int i;
    uint16_t tmp = 0;
    if (count > 15) count = 16;
    
    for (i = 0; i < 8; ++i) {
        tmp = src.u16[i];
        res->u16[i] = tmp << count;
    }
}

inline void vpsllw_256(YMM src, YMM *res, uint8_t count) {
    int i;
    uint16_t tmp = 0;
    if (count > 15) count = 16;
    
    for (i = 0; i < 16; ++i) {
        tmp = src.u16[i];
        res->u16[i] = tmp << count;
    }
}

inline void vpslld_128(XMM src, XMM *res, uint8_t count) {
    int i;
    uint32_t tmp = 0;
    if (count > 31) count = 32;
    
    for (i = 0; i < 4; ++i) {
        tmp = src.u32[i];
        res->u32[i] = tmp << count;
    }
}
inline void vpslld_256(YMM src, YMM *res, uint8_t count) {
    int i;
    uint32_t tmp = 0;
    if (count > 31) count = 32;
    
    for (i = 0; i < 8; ++i) {
        tmp = src.u32[i];
        res->u32[i] = tmp << count;
    }
}

inline void vpsllq_128(XMM src, XMM *res, uint8_t count) {
    int i;
    uint64_t tmp = 0;
    if (count > 63) count = 64;
    
    for (i = 0; i < 2; ++i) {
        tmp = src.u64[i];
        res->u64[i] = tmp << count;
    }
}
inline void vpsllq_256(YMM src, YMM *res, uint8_t count) {
    int i;
    uint64_t tmp = 0;
    if (count > 63) count = 64;
    
    for (i = 0; i < 4; ++i) {
        tmp = src.u64[i];
        res->u64[i] = tmp << count;
    }
}

// Right Logical
inline void vpsrlw_128(XMM src, XMM *res, uint8_t count) {
    int i;
    uint16_t tmp = 0;
    if (count > 15) count = 16;
    
    for (i = 0; i < 8; ++i) {
        tmp = src.u16[i];
        res->u16[i] = tmp >> count;
    }
}

inline void vpsrlw_256(YMM src, YMM *res, uint8_t count) {
    int i;
    uint16_t tmp = 0;
    if (count > 15) count = 16;
    
    for (i = 0; i < 16; ++i) {
        tmp = src.u16[i];
        res->u16[i] = tmp >> count;
    }
}

inline void vpsrld_128(XMM src, XMM *res, uint8_t count) {
    int i;
    uint32_t tmp = 0;
    if (count > 31) count = 32;
    
    for (i = 0; i < 4; ++i) {
        tmp = src.u32[i];
        res->u32[i] = tmp >> count;
    }
}
inline void vpsrld_256(YMM src, YMM *res, uint8_t count) {
    int i;
    uint32_t tmp = 0;
    if (count > 31) count = 32;
    
    for (i = 0; i < 8; ++i) {
        tmp = src.u32[i];
        res->u32[i] = tmp >> count;
    }
}

inline void vpsrlq_128(XMM src, XMM *res, uint8_t count) {
    int i;
    uint64_t tmp = 0;
    if (count > 63) count = 64;
    
    for (i = 0; i < 2; ++i) {
        tmp = src.u64[i];
        res->u64[i] = tmp >> count;
    }
}
inline void vpsrlq_256(YMM src, YMM *res, uint8_t count) {
    int i;
    uint64_t tmp = 0;
    if (count > 63) count = 64;
    
    for (i = 0; i < 4; ++i) {
        tmp = src.u64[i];
        res->u64[i] = tmp >> count;
    }
}

// Right Logical (Sign Bits)
inline void vpsraw_128(XMM src, XMM *res, uint8_t count) {
    int i;
    int16_t tmp = 0;
    if (count > 15) count = 16;
    
    for (i = 0; i < 8; ++i) {
        tmp = src.a16[i];
        res->a16[i] = tmp >> count;
    }
}

inline void vpsraw_256(YMM src, YMM *res, uint8_t count) {
    int i;
    int16_t tmp = 0;
    if (count > 15) count = 16;
    
    for (i = 0; i < 16; ++i) {
        tmp = src.a16[i];
        res->a16[i] = tmp >> count;
    }
}

inline void vpsrad_128(XMM src, XMM *res, uint8_t count) {
    int i;
    int32_t tmp = 0;
    if (count > 31) count = 32;
    
    for (i = 0; i < 4; ++i) {
        tmp = src.a32[i];
        res->a32[i] = tmp >> count;
    }
}
inline void vpsrad_256(YMM src, YMM *res, uint8_t count) {
    int i;
    int32_t tmp = 0;
    if (count > 31) count = 32;
    
    for (i = 0; i < 8; ++i) {
        tmp = src.a32[i];
        res->a32[i] = tmp >> count;
    }
}

// Left Logical
inline void vpslldq_128(XMM src, XMM *res, uint8_t count) {
    if (count > 15) count = 16;
    res->u128 = src.u128 << (count * 8);
}
inline void vpslldq_256(YMM src, YMM *res, uint8_t count) {
    int i;
    XMM tmp;
    if (count > 15) count = 16;
    
    for (i = 0; i < 2; ++i) {
        tmp.u128 = src.u128[i];
        res->u128[i] = tmp.u128 << (count * 8);
    }
}

// Right Logical
inline void vpsrldq_128(XMM src, XMM *res, uint8_t count) {
    if (count > 15) count = 16;
    res->u128 = src.u128 >> (count * 8);
}
inline void vpsrldq_256(YMM src, YMM *res, uint8_t count) {
    int i;
    XMM tmp;
    if (count > 15) count = 16;
    
    for (i = 0; i < 2; ++i) {
        tmp.u128 = src.u128[i];
        res->u128[i] = tmp.u128 >> (count * 8);
    }
}

inline void vshufpd_128(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int imm0 = imm & 1;
    int imm1 = (imm >> 1) & 1;
    
    if (imm0 == 0)
        res->fa64[0] = vsrc.fa64[0];
    else
        res->fa64[0] = vsrc.fa64[1];
    
    if (imm1 == 0)
        res->fa64[1] = src.fa64[0];
    else
        res->fa64[1] = src.fa64[1];
}

inline void vshufpd_256(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int imm0 = imm & 1;
    int imm1 = (imm >> 1) & 1;
    int imm2 = (imm >> 2) & 1;
    int imm3 = (imm >> 3) & 1;

    if (imm0 == 0)
        res->fa64[0] = vsrc.fa64[0];
    else
        res->fa64[0] = vsrc.fa64[1];
    
    if (imm1 == 0)
        res->fa64[1] = src.fa64[0];
    else
        res->fa64[1] = src.fa64[1];

    if (imm2 == 0)
        res->fa64[2] = vsrc.fa64[2];
    else
        res->fa64[2] = vsrc.fa64[3];

    if (imm3 == 0)
        res->fa64[3] = src.fa64[2];
    else
        res->fa64[3] = src.fa64[3];
}

/************* Interleave *************/
inline void vpunpcklbw_128(XMM src, XMM vsrc, XMM *res) {
    res->u8[0] = vsrc.u8[0];
    res->u8[1] = src.u8[0];
    res->u8[2] = vsrc.u8[1];
    res->u8[3] = src.u8[1];
    res->u8[4] = vsrc.u8[2];
    res->u8[5] = src.u8[2];
    res->u8[6] = vsrc.u8[3];
    res->u8[7] = src.u8[3];
    res->u8[8] = vsrc.u8[4];
    res->u8[9] = src.u8[4];
    res->u8[10] = vsrc.u8[5];
    res->u8[11] = src.u8[5];
    res->u8[12] = vsrc.u8[6];
    res->u8[13] = src.u8[6];
    res->u8[14] = vsrc.u8[7];
    res->u8[15] = src.u8[7];
}
inline void vpunpcklbw_256(YMM src, YMM vsrc, YMM *res) {
    res->u8[0] = vsrc.u8[0];
    res->u8[1] = src.u8[0];
    res->u8[2] = vsrc.u8[1];
    res->u8[3] = src.u8[1];
    res->u8[4] = vsrc.u8[2];
    res->u8[5] = src.u8[2];
    res->u8[6] = vsrc.u8[3];
    res->u8[7] = src.u8[3];
    res->u8[8] = vsrc.u8[4];
    res->u8[9] = src.u8[4];
    res->u8[10] = vsrc.u8[5];
    res->u8[11] = src.u8[5];
    res->u8[12] = vsrc.u8[6];
    res->u8[13] = src.u8[6];
    res->u8[14] = vsrc.u8[7];
    res->u8[15] = src.u8[7];
    res->u8[16] = vsrc.u8[16];
    res->u8[17] = src.u8[16];
    res->u8[18] = vsrc.u8[17];
    res->u8[19] = src.u8[17];
    res->u8[20] = vsrc.u8[18];
    res->u8[21] = src.u8[18];
    res->u8[22] = vsrc.u8[19];
    res->u8[23] = src.u8[19];
    res->u8[24] = vsrc.u8[20];
    res->u8[25] = src.u8[20];
    res->u8[26] = vsrc.u8[21];
    res->u8[27] = src.u8[21];
    res->u8[28] = vsrc.u8[22];
    res->u8[29] = src.u8[22];
    res->u8[30] = vsrc.u8[23];
    res->u8[31] = src.u8[23];
}
inline void vpunpcklwd_128(XMM src, XMM vsrc, XMM *res) {
    res->u16[0] = vsrc.u16[0];
    res->u16[1] = src.u16[0];
    res->u16[2] = vsrc.u16[1];
    res->u16[3] = src.u16[1];
    res->u16[4] = vsrc.u16[2];
    res->u16[5] = src.u16[2];
    res->u16[6] = vsrc.u16[3];
    res->u16[7] = src.u16[3];
}

inline void vpunpcklwd_256(YMM src, YMM vsrc, YMM *res) {
    res->u16[0] = vsrc.u16[0];
    res->u16[1] = src.u16[0];
    res->u16[2] = vsrc.u16[1];
    res->u16[3] = src.u16[1];
    res->u16[4] = vsrc.u16[2];
    res->u16[5] = src.u16[2];
    res->u16[6] = vsrc.u16[3];
    res->u16[7] = src.u16[3];
    res->u16[8] = vsrc.u16[8];
    res->u16[9] = src.u16[8];
    res->u16[10] = vsrc.u16[9];
    res->u16[11] = src.u16[9];
    res->u16[12] = vsrc.u16[10];
    res->u16[13] = src.u16[10];
    res->u16[14] = vsrc.u16[11];
    res->u16[15] = src.u16[11];
}
inline void vpunpckldq_128(XMM src, XMM vsrc, XMM *res) {
    res->u32[0] = vsrc.u32[0];
    res->u32[1] = src.u32[0];
    res->u32[2] = vsrc.u32[1];
    res->u32[3] = src.u32[1];
}
inline void vpunpckldq_256(YMM src, YMM vsrc, YMM *res) {
    res->u32[0] = vsrc.u32[0];
    res->u32[1] = src.u32[0];
    res->u32[2] = vsrc.u32[1];
    res->u32[3] = src.u32[1];
    
    res->u32[4] = vsrc.u32[4];
    res->u32[5] = src.u32[4];
    res->u32[6] = vsrc.u32[5];
    res->u32[7] = src.u32[5];
}
inline void vpunpcklqdq_128(XMM src, XMM vsrc, XMM *res) {
    res->u64[0] = vsrc.u64[0];
    res->u64[1] = src.u64[0];
}
inline void vpunpcklqdq_256(YMM src, YMM vsrc, YMM *res) {
    res->u64[0] = vsrc.u64[0];
    res->u64[1] = src.u64[0];
    res->u64[2] = vsrc.u64[2];
    res->u64[3] = src.u64[2];
}

inline void vpunpckhbw_128(XMM src, XMM vsrc, XMM *res) {
    res->u8[0] = vsrc.u8[8];
    res->u8[1] = src.u8[8];
    res->u8[2] = vsrc.u8[9];
    res->u8[3] = src.u8[9];
    res->u8[4] = vsrc.u8[10];
    res->u8[5] = src.u8[10];
    res->u8[6] = vsrc.u8[11];
    res->u8[7] = src.u8[11];
    res->u8[8] = vsrc.u8[12];
    res->u8[9] = src.u8[12];
    res->u8[10] = vsrc.u8[13];
    res->u8[11] = src.u8[13];
    res->u8[12] = vsrc.u8[14];
    res->u8[13] = src.u8[14];
    res->u8[14] = vsrc.u8[15];
    res->u8[15] = src.u8[15];
}
inline void vpunpckhbw_256(YMM src, YMM vsrc, YMM *res) {
    res->u8[0] = vsrc.u8[8];
    res->u8[1] = src.u8[8];
    res->u8[2] = vsrc.u8[9];
    res->u8[3] = src.u8[9];
    res->u8[4] = vsrc.u8[10];
    res->u8[5] = src.u8[10];
    res->u8[6] = vsrc.u8[11];
    res->u8[7] = src.u8[11];
    res->u8[8] = vsrc.u8[12];
    res->u8[9] = src.u8[12];
    res->u8[10] = vsrc.u8[13];
    res->u8[11] = src.u8[13];
    res->u8[12] = vsrc.u8[14];
    res->u8[13] = src.u8[14];
    res->u8[14] = vsrc.u8[15];
    res->u8[15] = src.u8[15];
    res->u8[16] = vsrc.u8[24];
    res->u8[17] = src.u8[24];
    res->u8[18] = vsrc.u8[25];
    res->u8[19] = src.u8[25];
    res->u8[20] = vsrc.u8[26];
    res->u8[21] = src.u8[26];
    res->u8[22] = vsrc.u8[27];
    res->u8[23] = src.u8[27];
    res->u8[24] = vsrc.u8[28];
    res->u8[25] = src.u8[28];
    res->u8[26] = vsrc.u8[29];
    res->u8[27] = src.u8[29];
    res->u8[28] = vsrc.u8[30];
    res->u8[29] = src.u8[30];
    res->u8[30] = vsrc.u8[31];
    res->u8[31] = src.u8[31];
}
inline void vpunpckhwd_128(XMM src, XMM vsrc, XMM *res) {
    res->u16[0] = vsrc.u16[4];
    res->u16[1] = src.u16[4];
    res->u16[2] = vsrc.u16[5];
    res->u16[3] = src.u16[5];
    res->u16[4] = vsrc.u16[6];
    res->u16[5] = src.u16[6];
    res->u16[6] = vsrc.u16[7];
    res->u16[7] = src.u16[7];
}
inline void vpunpckhwd_256(YMM src, YMM vsrc, YMM *res) {
    res->u16[0] = vsrc.u16[4];
    res->u16[1] = src.u16[4];
    res->u16[2] = vsrc.u16[5];
    res->u16[3] = src.u16[5];
    res->u16[4] = vsrc.u16[6];
    res->u16[5] = src.u16[6];
    res->u16[6] = vsrc.u16[7];
    res->u16[7] = src.u16[7];
    res->u16[8] = vsrc.u16[12];
    res->u16[9] = src.u16[12];
    res->u16[10] = vsrc.u16[13];
    res->u16[11] = src.u16[13];
    res->u16[12] = vsrc.u16[14];
    res->u16[13] = src.u16[14];
    res->u16[14] = vsrc.u16[15];
    res->u16[15] = src.u16[15];
}
inline void vpunpckhdq_128(XMM src, XMM vsrc, XMM *res) {
    res->u32[0] = vsrc.u32[2];
    res->u32[1] = src.u32[2];
    res->u32[2] = vsrc.u32[3];
    res->u32[3] = src.u32[3];
}
inline void vpunpckhdq_256(YMM src, YMM vsrc, YMM *res) {
    res->u32[0] = vsrc.u32[2];
    res->u32[1] = src.u32[2];
    res->u32[2] = vsrc.u32[3];
    res->u32[3] = src.u32[3];
    res->u32[4] = vsrc.u32[6];
    res->u32[5] = src.u32[6];
    res->u32[6] = vsrc.u32[7];
    res->u32[7] = src.u32[7];
}
inline void vpunpckhqdq_128(XMM src, XMM vsrc, XMM *res) {
    res->u64[0] = vsrc.u64[1];
    res->u64[1] = src.u64[1];
}
inline void vpunpckhqdq_256(YMM src, YMM vsrc, YMM *res) {
    res->u64[0] = vsrc.u64[1];
    res->u64[1] = src.u64[1];
    res->u64[2] = vsrc.u64[3];
    res->u64[3] = src.u64[3];
}

inline void vunpcklpd_128(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = vsrc.fa64[0];
    res->fa64[1] = src.fa64[0];
}
inline void vunpcklpd_256(YMM src, YMM vsrc, YMM *res) {
    res->fa64[0] = vsrc.fa64[0];
    res->fa64[1] = src.fa64[0];
    res->fa64[2] = vsrc.fa64[2];
    res->fa64[3] = src.fa64[2];
}
inline void vunpckhpd_128(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = vsrc.fa64[1];
    res->fa64[1] = src.fa64[1];
}
inline void vunpckhpd_256(YMM src, YMM vsrc, YMM *res) {
    res->fa64[0] = vsrc.fa64[1];
    res->fa64[1] = src.fa64[1];
    res->fa64[2] = vsrc.fa64[3];
    res->fa64[3] = src.fa64[3];
}

/************* MAX/MIN Return *************/
inline void vminpd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int MIN = 0;
    double SRC1, SRC2;
    
    for (i = 0; i < 2; ++i) {
        SRC1 = vsrc.fa64[i];
        SRC2 = src.fa64[i];
        MIN = mindf(SRC1, SRC2);
        if (MIN)
            res->fa64[i] = vsrc.fa64[i];
        else
            res->fa64[i] = src.fa64[i];
    }
}
inline void vminpd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int MIN = 0;
    double SRC1, SRC2;
    
    for (i = 0; i < 4; ++i) {
        SRC1 = vsrc.fa64[i];
        SRC2 = src.fa64[i];
        MIN = mindf(SRC1, SRC2);
        if (MIN)
            res->fa64[i] = vsrc.fa64[i];
        else
            res->fa64[i] = src.fa64[i];
    }

}

inline void vmaxpd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int MAX = 0;
    double SRC1, SRC2;
    
    for (i = 0; i < 2; ++i) {
        SRC1 = vsrc.fa64[i];
        SRC2 = src.fa64[i];
        MAX = maxdf(SRC1, SRC2);
        if (MAX)
            res->fa64[i] = vsrc.fa64[i];
        else
            res->fa64[i] = src.fa64[i];
    }
}
inline void vmaxpd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int MAX = 0;
    double SRC1, SRC2;
    
    for (i = 0; i < 4; ++i) {
        SRC1 = vsrc.fa64[i];
        SRC2 = src.fa64[i];
        MAX = maxdf(SRC1, SRC2);
        if (MAX)
            res->fa64[i] = vsrc.fa64[i];
        else
            res->fa64[i] = src.fa64[i];
    }
}

inline void vminsd(XMM src, XMM vsrc, XMM *res) {
    int MIN = 0;
    double SRC1 = vsrc.fa64[0];
    double SRC2 = src.fa64[0];
    MIN = mindf(SRC1, SRC2);
    if (MIN)
        res->fa64[0] = vsrc.fa64[0];
    else
        res->fa64[0] = src.fa64[0];
    
    res->fa64[1] = vsrc.fa64[1];
}
inline void vmaxsd(XMM src, XMM vsrc, XMM *res) {
    int MAX = 0;
    double SRC1 = vsrc.fa64[0];
    double SRC2 = src.fa64[0];
    MAX = maxdf(SRC1, SRC2);
    if (MAX)
        res->fa64[0] = vsrc.fa64[0];
    else
        res->fa64[0] = src.fa64[0];
    
    res->fa64[1] = vsrc.fa64[1];
}

/************* Compare *************/
inline void vucomusd(XMM src, XMM dst, x86_saved_state_t *state) {
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
    
    double SRC1 = dst.fa64[0];
    double SRC2 = src.fa64[0];
    
    //UNORDERED
    if ((isNaN_f64(SRC1)) || (isNaN_f64(SRC1))) {
        FLAGS |= 0x00000040; //set ZF = 1
        FLAGS |= 0x00000004; //set PF = 1
        FLAGS |= 0x00000001; //set CF = 1
    }
    //GREATER_THAN
    if (SRC1 > SRC2) {
        FLAGS = (FLAGS | 0x00000040) ^ 0x00000040;  //set ZF = 0
        FLAGS = (FLAGS | 0x00000004) ^ 0x00000004;  //set PF = 0
        FLAGS = (FLAGS | 0x00000001) ^ 0x00000001;  //set CF = 0
    }
    //LESS_THAN
    if (SRC1 < SRC2) {
        FLAGS = (FLAGS | 0x00000040) ^ 0x00000040;  //set ZF = 0
        FLAGS = (FLAGS | 0x00000004) ^ 0x00000004;  //set PF = 0
        FLAGS |= 0x00000001; //set CF = 1
    }
    //EQUAL
    if (SRC1 == SRC2) {
        FLAGS |= 0x00000040; //set ZF = 1
        FLAGS = (FLAGS | 0x00000004) ^ 0x00000004;  //set PF = 0
        FLAGS = (FLAGS | 0x00000001) ^ 0x00000001;  //set CF = 0
    }
    
    FLAGS = (FLAGS | 0x00000800) ^ 0x00000800;  //set OF = 0
    FLAGS = (FLAGS | 0x00000010) ^ 0x00000010;  //set AF = 0
    FLAGS = (FLAGS | 0x00000080) ^ 0x00000080;  //set SF = 0
    
    if (is_saved_state64(state)) {
        saved_state64(state)->isf.rflags = FLAGS;
    } else {
        EFLAGS = FLAGS & 0xFFFFFFFF;
        saved_state32(state)->efl = EFLAGS;
    }
}

inline void vpcmpgtb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int8_t SRC1, SRC2;
    
    for (i = 0; i < 16; ++i) {
        SRC1 = vsrc.a8[i];
        SRC2 = src.a8[i];
        if (SRC1 > SRC2)
            res->a8[i] = 0xFF;
        else
            res->a8[i] = 0;
    }
}
inline void vpcmpgtb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int8_t SRC1, SRC2;
    
    for (i = 0; i < 32; ++i) {
        SRC1 = vsrc.a8[i];
        SRC2 = src.a8[i];
        if (SRC1 > SRC2)
            res->a8[i] = 0xFF;
        else
            res->a8[i] = 0;
    }
}

inline void vpcmpgtw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int16_t SRC1, SRC2;
    
    for (i = 0; i < 8; ++i) {
        SRC1 = vsrc.a16[i];
        SRC2 = src.a16[i];
        if (SRC1 > SRC2)
            res->a16[i] = 0xFFFF;
        else
            res->a16[i] = 0;
    }
}

inline void vpcmpgtw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int16_t SRC1, SRC2;
    
    for (i = 0; i < 16; ++i) {
        SRC1 = vsrc.a16[i];
        SRC2 = src.a16[i];
        if (SRC1 > SRC2)
            res->a16[i] = 0xFFFF;
        else
            res->a16[i] = 0;
    }
}

inline void vpcmpgtd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int32_t SRC1, SRC2;
    
    for (i = 0; i < 4; ++i) {
        SRC1 = vsrc.a32[i];
        SRC2 = src.a32[i];
        if (SRC1 > SRC2)
            res->a32[i] = 0xFFFFFFFF;
        else
            res->a32[i] = 0;
    }
}
inline void vpcmpgtd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int32_t SRC1, SRC2;
    
    for (i = 0; i < 8; ++i) {
        SRC1 = vsrc.a32[i];
        SRC2 = src.a32[i];
        if (SRC1 > SRC2)
            res->a32[i] = 0xFFFFFFFF;
        else
            res->a32[i] = 0;
    }
}

inline void vpcmpeqb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int8_t SRC1, SRC2;
    
    for (i = 0; i < 16; ++i) {
        SRC1 = vsrc.a8[i];
        SRC2 = src.a8[i];
        if (SRC1 == SRC2)
            res->a8[i] = 0xFF;
        else
            res->a8[i] = 0;
    }
}
inline void vpcmpeqb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int8_t SRC1, SRC2;
    
    for (i = 0; i < 32; ++i) {
        SRC1 = vsrc.a8[i];
        SRC2 = src.a8[i];
        if (SRC1 == SRC2)
            res->a8[i] = 0xFF;
        else
            res->a8[i] = 0;
    }
}

inline void vpcmpeqw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int16_t SRC1, SRC2;
    
    for (i = 0; i < 8; ++i) {
        SRC1 = vsrc.a16[i];
        SRC2 = src.a16[i];
        if (SRC1 == SRC2)
            res->a16[i] = 0xFFFF;
        else
            res->a16[i] = 0;
    }
}

inline void vpcmpeqw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int16_t SRC1, SRC2;
    
    for (i = 0; i < 16; ++i) {
        SRC1 = vsrc.a16[i];
        SRC2 = src.a16[i];
        if (SRC1 == SRC2)
            res->a16[i] = 0xFFFF;
        else
            res->a16[i] = 0;
    }
}

inline void vpcmpeqd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int32_t SRC1, SRC2;
    
    for (i = 0; i < 4; ++i) {
        SRC1 = vsrc.a32[i];
        SRC2 = src.a32[i];
        if (SRC1 == SRC2)
            res->a32[i] = 0xFFFFFFFF;
        else
            res->a32[i] = 0;
    }
}
inline void vpcmpeqd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int32_t SRC1, SRC2;
    
    for (i = 0; i < 8; ++i) {
        SRC1 = vsrc.a32[i];
        SRC2 = src.a32[i];
        if (SRC1 == SRC2)
            res->a32[i] = 0xFFFFFFFF;
        else
            res->a32[i] = 0;
    }
}

inline void vcmppd_128(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int i;
    int CMP = 0;
    double SRC1, SRC2;
    uint8_t immvar = imm & 31;

    for (i = 0; i < 2; ++i) {
        SRC1 = vsrc.fa64[i];
        SRC2 = src.fa64[i];
        CMP = cmpoptdf(SRC1, SRC2, immvar);
        if (CMP)
            res->u64[i] = 0xFFFFFFFFFFFFFFFF;
        else
            res->u64[i] = 0;
    }
}
inline void vcmppd_256(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    int i;
    int CMP = 0;
    double SRC1, SRC2;
    uint8_t immvar = imm & 31;
    
    for (i = 0; i < 4; ++i) {
        SRC1 = vsrc.fa64[i];
        SRC2 = src.fa64[i];
        CMP = cmpoptdf(SRC1, SRC2, immvar);
        if (CMP)
            res->u64[i] = 0xFFFFFFFFFFFFFFFF;
        else
            res->u64[i] = 0;
    }
}
inline void vcmpsd(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    int CMP = 0;
    uint8_t immvar = imm & 31;
    double SRC1 = vsrc.fa64[0];
    double SRC2 = src.fa64[0];
    CMP = cmpoptdf(SRC1, SRC2, immvar);
    if (CMP)
        res->u64[0] = 0xFFFFFFFFFFFFFFFF;
    else
        res->u64[0] = 0;
    
    res->u64[1] = vsrc.u64[1];
}

#endif /* vsse2_hpp */
