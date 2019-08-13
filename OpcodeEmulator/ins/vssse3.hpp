//
//  vssse3.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/8.
//  Copyright © 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef vssse3_hpp
#define vssse3_hpp

#include "opemu.hpp"

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
                       int kernel_trap);

/**********************************************/
/**  VSSSE3  instructions implementation       **/
/**********************************************/
inline void vpshufb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int index;
    int xbit;
    uint8_t mask;
    for (i = 0; i < 16; ++i) {
        mask = src.u8[i];
        xbit = (mask >> 7) & 1;
        if (xbit) {
            res->u8[i] = 0;
        } else {
            index = src.u8[i] & 15;
            res->u8[i] = vsrc.u8[index];

        }
    }
}
inline void vpshufb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int index;
    int xbit;
    uint8_t mask;
    for (i = 0; i < 32; ++i) {
        mask = src.u8[i];
        xbit = (mask >> 7) & 1;
        if (xbit) {
            res->u8[i] = 0;
        } else {
            index = src.u8[i] & 15;
            res->u8[i] = vsrc.u8[index];
        }
    }

}

inline void vphaddw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int j;
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        res->u16[i] = vsrc.u16[j+1] + vsrc.u16[j];
        res->u16[i+4] = src.u16[j+1] + src.u16[j];
    }
}
inline void vphaddw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int j;
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        res->u16[i] = vsrc.u16[j+1] + vsrc.u16[j];
        res->u16[i+4] = src.u16[j+1] + src.u16[j];
    }
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        res->u16[i+8] = vsrc.u16[j+9] + vsrc.u16[j+8];
        res->u16[i+12] = src.u16[j+9] + src.u16[j+8];
    }
}

inline void vphaddd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int j;
    for (i = 0; i < 2; ++i) {
        j = i < 1;
        res->u32[i] = vsrc.u32[j+1] + vsrc.u32[j];
        res->u32[i+2] = src.u32[j+1] + src.u32[j];
    }
}
inline void vphaddd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int j;
    for (i = 0; i < 2; ++i) {
        j = i < 1;
        res->u32[i] = vsrc.u32[j+1] + vsrc.u32[j];
        res->u32[i+2] = src.u32[j+1] + src.u32[j];
    }
    for (i = 0; i < 2; ++i) {
        j = i < 1;
        res->u32[i+4] = vsrc.u32[j+5] + vsrc.u32[j+4];
        res->u32[i+6] = src.u32[j+5] + src.u32[j+4];
    }
}

inline void vphaddsw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int j;
    int16_t X;
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        X = vsrc.a16[j+1] + vsrc.a16[j];
        res->a16[i] = STSW(X);
        X = src.a16[j+1] + src.a16[j];
        res->a16[i+4] = STSW(X);
    }
}
inline void vphaddsw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int j;
    int16_t X;
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        X = vsrc.a16[j+1] + vsrc.a16[j];
        res->a16[i] = STSW(X);
        X = src.a16[j+1] + src.a16[j];
        res->a16[i+4] = STSW(X);
    }
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        X = vsrc.a16[j+9] + vsrc.a16[j+8];
        res->a16[i+8] = STSW(X);
        X = src.a16[j+9] + src.a16[j+8];
        res->a16[i+12] = STSW(X);
    }
}

inline void vpmaddubsw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int j;
    int16_t X;
    for (i = 0; i < 8; ++i) {
        j = i < 1;
        X = (src.a8[j+1] * vsrc.a8[j+1]) + (src.a8[j] * vsrc.a8[j]);
        res->a16[i] = STSW(X);
    }
}
inline void vpmaddubsw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int j;
    int16_t X;
    for (i = 0; i < 16; ++i) {
        j = i < 1;
        X = (src.a8[j+1] * vsrc.a8[j+1]) + (src.a8[j] * vsrc.a8[j]);
        res->a16[i] = STSW(X);
    }
}
inline void vphsubw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int j;
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        res->a16[i] = vsrc.a16[j] - vsrc.a16[j+1];
        res->a16[i+4] = src.a16[j] - src.a16[j+1];
    }
}
inline void vphsubw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int j;
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        res->a16[i] = vsrc.a16[j] - vsrc.a16[j+1];
        res->a16[i+4] = src.a16[j] - src.a16[j+1];
    }
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        res->a16[i+8] = vsrc.a16[j+8] - vsrc.a16[j+9];
        res->a16[i+12] = src.a16[j+8] - src.a16[j+9];
    }
}

inline void vphsubd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int j;
    for (i = 0; i < 2; ++i) {
        j = i < 1;
        res->a32[i] = vsrc.a32[j] - vsrc.a32[j+1];
        res->a32[i+2] = src.a32[j] - src.a32[j+1];
    }
}
inline void vphsubd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int j;
    for (i = 0; i < 2; ++i) {
        j = i < 1;
        res->a32[i] = vsrc.a32[j] - vsrc.a32[j+1];
        res->a32[i+2] = src.a32[j] - src.a32[j+1];
    }
    for (i = 0; i < 2; ++i) {
        j = i < 1;
        res->a32[i+4] = vsrc.a32[j+4] - vsrc.a32[j+5];
        res->a32[i+6] = src.a32[j+4] - src.a32[j+5];
    }
}
inline void vphsubsw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int j;
    int16_t X;
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        X = vsrc.a16[j] - vsrc.a16[j+1];
        res->a16[i] = STSW(X);
        X = src.a16[j] - src.a16[j+1];
        res->a16[i+4] = STSW(X);
    }
}
inline void vphsubsw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int j;
    int16_t X;
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        X = vsrc.a16[j] - vsrc.a16[j+1];
        res->a16[i] = STSW(X);
        X = src.a16[j] - src.a16[j+1];
        res->a16[i+4] = STSW(X);
    }
    for (i = 0; i < 4; ++i) {
        j = i < 1;
        X = vsrc.a16[j+8] - vsrc.a16[j+9];
        res->a16[i+8] = STSW(X);
        X = src.a16[j+8] - src.a16[j+9];
        res->a16[i+12] = STSW(X);
    }
}

inline void vpsignb_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int8_t mask;
    for (i = 0; i < 16; ++i) {
        mask = src.a8[i];
        if (mask < 0 )
            res->u8[i] = -vsrc.u8[i];
        else if (mask == 0 )
            res->u8[i] = 0;
        else if (mask > 0 )
            res->u8[i] = res->u8[i];
    }
}
inline void vpsignb_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int8_t mask;
    for (i = 0; i < 32; ++i) {
        mask = src.a8[i];
        if (mask < 0 )
            res->u8[i] = -vsrc.u8[i];
        else if (mask == 0 )
            res->u8[i] = 0;
        else if (mask > 0 )
            res->u8[i] = res->u8[i];
    }
}

inline void vpsignw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int16_t mask;
    for (i = 0; i < 8; ++i) {
        mask = src.a16[i];
        if (mask < 0 )
            res->u16[i] = -vsrc.u16[i];
        else if (mask == 0 )
            res->u16[i] = 0;
        else if (mask > 0 )
            res->u16[i] = res->u16[i];
    }
}
inline void vpsignw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int16_t mask;
    for (i = 0; i < 16; ++i) {
        mask = src.a16[i];
        if (mask < 0 )
            res->u16[i] = -vsrc.u16[i];
        else if (mask == 0 )
            res->u16[i] = 0;
        else if (mask > 0 )
            res->u16[i] = res->u16[i];
    }
}

inline void vpsignd_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    int32_t mask;
    for (i = 0; i < 4; ++i) {
        mask = src.a32[i];
        if (mask < 0 )
            res->u32[i] = -vsrc.u32[i];
        else if (mask == 0 )
            res->u32[i] = 0;
        else if (mask > 0 )
            res->u32[i] = res->u32[i];
    }
}
inline void vpsignd_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    int32_t mask;
    for (i = 0; i < 8; ++i) {
        mask = src.a32[i];
        if (mask < 0 )
            res->u32[i] = -vsrc.u32[i];
        else if (mask == 0 )
            res->u32[i] = 0;
        else if (mask > 0 )
            res->u32[i] = res->u32[i];
    }
}

inline void vpmulhrsw_128(XMM src, XMM vsrc, XMM *res) {
    int i;
    XMM temp;
    for (i = 0; i < 8; ++i) {
        temp.a32[0] = ((vsrc.a16[i] * src.a16[i] + 0x4000) >> 15);
        res->a16[i] = temp.a16[0];
    }
}
inline void vpmulhrsw_256(YMM src, YMM vsrc, YMM *res) {
    int i;
    XMM temp;
    for (i = 0; i < 16; ++i) {
        temp.a32[0] = ((vsrc.a16[i] * src.a16[i] + 0x4000) >> 15);
        res->a16[i] = temp.a16[0];
    }
}

inline void vpabsb_128(XMM src, XMM *res) {
    int i;
    int8_t x;
    for (i = 0; i < 16; ++i) {
        x = src.a8[i];
        res->u8[i] = ABS(x);
    }
}
inline void vpabsb_256(YMM src, YMM *res) {
    int i;
    int8_t x;
    for (i = 0; i < 32; ++i) {
        x = src.a8[i];
        res->u8[i] = ABS(x);
    }

}
inline void vpabsw_128(XMM src, XMM *res) {
    int i;
    int16_t x;
    for (i = 0; i < 8; ++i) {
        x = src.a16[i];
        res->u16[i] = ABS(x);
    }
}
inline void vpabsw_256(YMM src, YMM *res) {
    int i;
    int16_t x;
    for (i = 0; i < 16; ++i) {
        x = src.a16[i];
        res->u16[i] = ABS(x);
    }
}
inline void vpabsd_128(XMM src, XMM *res) {
    int i;
    int32_t x;
    for (i = 0; i < 4; ++i) {
        x = src.a32[i];
        res->u32[i] = ABS(x);
    }
}
inline void vpabsd_256(YMM src, YMM *res) {
    int i;
    int32_t x;
    for (i = 0; i < 8; ++i) {
        x = src.a32[i];
        res->u32[i] = ABS(x);
    }
}

inline void vpalignr_128(XMM src, XMM vsrc, XMM *res, uint8_t imm) {
    XMM src1, src2;
    src1.u128 = vsrc.u128;
    src2.u128 = src.u128;
    
    if (imm == 0) {
        res->u128 = src2.u128;
    } else if (imm < 16) {
        res->u128 = (src1.u128 << (128 - (imm * 8))) | (src2.u128 >> (imm * 8));
    } else if (imm == 16) {
        res->u128 = src1.u128;
    } else if (imm < 32) {
        res->u128 = src1.u128 >> ((imm - 16) * 8);
    } else if (imm > 31) {
        res->u128 = 0;
    }
}
inline void vpalignr_256(YMM src, YMM vsrc, YMM *res, uint8_t imm) {
    XMM src1, src2;

    //127:0 bit
    src1.u128 = vsrc.u128[0];
    src2.u128 = src.u128[0];

    if (imm == 0) {
        res->u128[0] = src2.u128;
    } else if (imm < 16) {
        res->u128[0] = (src1.u128 << (128 - (imm * 8))) | (src2.u128 >> (imm * 8));
    } else if (imm == 16) {
        res->u128[0] = src1.u128;
    } else if (imm < 32) {
        res->u128[0] = src1.u128 >> ((imm - 16) * 8);
    } else if (imm > 31) {
        res->u128[0] = 0;
    }
    
    //255:128 bit
    src1.u128 = vsrc.u128[1];
    src2.u128 = src.u128[1];

    if (imm == 0) {
        res->u128[1] = src2.u128;
    } else if (imm < 16) {
        res->u128[1] = (src1.u128 << (128 - (imm * 8))) | (src2.u128 >> (imm * 8));
    } else if (imm == 16) {
        res->u128[1] = src1.u128;
    } else if (imm < 32) {
        res->u128[1] = src1.u128 >> ((imm - 16) * 8);
    } else if (imm > 31) {
        res->u128[1] = 0;
    }
}

#endif /* vssse3_hpp */
