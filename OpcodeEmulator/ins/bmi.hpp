//
//  bmi.hpp
//  OpcodeEmulator
//
//  Created by Meowthra on 2019/1/29.
//  Copyright © 2019年 Meowthra. All rights reserved.
//  Made in Taiwan.

#ifndef bmi_hpp
#define bmi_hpp

#include "opemu.hpp"

int bmi_instruction(x86_saved_state_t *state,
                    uint8_t vexreg,
                    uint8_t opcode,
                    uint8_t *modrm,
                    uint8_t high_reg,
                    uint8_t high_index,
                    uint8_t high_base,
                    uint8_t operand_size,
                    uint8_t leading_opcode,
                    uint8_t simd_prefix,
                    uint8_t *bytep,
                    uint8_t ins_size,
                    uint8_t modbyte,
                    int kernel_trap);

/**********************************************/
/**  BMI1  instructions implementation       **/
/**********************************************/
inline void andn64(M64 src, M64 vsrc, M64 *res, uint8_t operand_size, x86_saved_state_t *state) {
    uint64_t FLAGS = saved_state64(state)->isf.rflags;
    int SF;

    if (operand_size == 64) {
        int64_t SRC1 = vsrc.a64;
        int64_t SRC2 = src.a64;
        res->a64 = (~SRC1) & SRC2;
        
        //set flag
        uint64_t DEST = res->u64;
        //SF = bit[63]
        SF = (DEST >> 63) & 1;
        if (SF == 1) {
            //set SF = 1
            FLAGS = FLAGS | 0x0080;
        } else {
            //set SF = 0
            FLAGS = (FLAGS | 0x0080) ^ 0x0080;
        }
        if (DEST == 0) {
            //set ZF = 1
            FLAGS = FLAGS | 0x0040;
        } else {
            //set ZF = 0
            FLAGS = (FLAGS | 0x0040) ^ 0x0040;
        }

    } else {
        int32_t SRC1 = vsrc.a32[0];
        int32_t SRC2 = src.a32[0];
        res->a32[0] = (~SRC1) & SRC2;
        
        //set flag
        uint32_t DEST = res->u32[0];
        //SF = bit[31]
        SF = (DEST >> 31) & 1;
        if (SF == 1) {
            //set SF = 1
            FLAGS = FLAGS | 0x0080;
        } else {
            //set SF = 0
            FLAGS = (FLAGS | 0x0080) ^ 0x0080;
        }
        if (DEST == 0) {
            //set ZF = 1
            FLAGS = FLAGS | 0x0040;
        } else {
            //set ZF = 0
            FLAGS = (FLAGS | 0x0040) ^ 0x0040;
        }
    }
    
    saved_state64(state)->isf.rflags = FLAGS;
}

inline void andn32(M32 src, M32 vsrc, M32 *res, x86_saved_state_t *state) {
    uint32_t FLAGS = saved_state32(state)->efl;
    int SF;
    
    int32_t SRC1 = vsrc.a32;
    int32_t SRC2 = src.a32;
    res->a32 = (~SRC1) & SRC2;
    
    //set flag
    uint32_t DEST = res->u32;
    //SF = bit[31]
    SF = (DEST >> 31) & 1;
    if (SF == 1) {
        //set SF = 1
        FLAGS = FLAGS | 0x0080;
    } else {
        //set SF = 0
        FLAGS = (FLAGS | 0x0080) ^ 0x0080;
    }
    if (DEST == 0) {
        //set ZF = 1
        FLAGS = FLAGS | 0x0040;
    } else {
        //set ZF = 0
        FLAGS = (FLAGS | 0x0040) ^ 0x0040;
    }

    saved_state32(state)->efl = FLAGS;
}

inline void bextr64(M64 src, M64 vsrc, M64 *res, uint8_t operand_size, x86_saved_state_t *state) {
    uint64_t FLAGS = saved_state64(state)->isf.rflags;
    
    uint8_t start = vsrc.u8[0];
    uint8_t len = vsrc.u8[1];

    if (operand_size == 64) {
        uint64_t DEST = 0;
        uint64_t SRC = src.u64;
        if (start < 64) {
            DEST = (SRC >> start) & ((1LL << (len + 1)) - 1);
            res->u64 = DEST;
        }
        
        //set flag
        if (DEST == 0) {
            //set ZF = 1
            FLAGS = FLAGS | 0x0040;
        } else {
            //set ZF = 0
            FLAGS = (FLAGS | 0x0040) ^ 0x0040;
        }
        
    } else {
        uint32_t DEST = 0;
        uint32_t SRC = src.u32[0];
        if (start < 32) {
            DEST = (SRC >> start) & ((1 << (len + 1)) - 1);
            res->u32[0] = DEST;
        }

        //set flag
        if (DEST == 0) {
            //set ZF = 1
            FLAGS = FLAGS | 0x0040;
        } else {
            //set ZF = 0
            FLAGS = (FLAGS | 0x0040) ^ 0x0040;
        }
    }
    saved_state64(state)->isf.rflags = FLAGS;
}

inline void bextr32(M32 src, M32 vsrc, M32 *res, x86_saved_state_t *state) {
    uint32_t FLAGS = saved_state32(state)->efl;
    
    uint8_t start = vsrc.u8[0];
    uint8_t len = vsrc.u8[1];
    
    uint32_t DEST = 0;
    uint32_t SRC = src.u32;

    if (start < 32) {
        DEST = (SRC >> start) & ((1 << (len + 1)) - 1);
        res->u32 = DEST;
    }

    //set flag
    if (DEST == 0) {
        //set ZF = 1
        FLAGS = FLAGS | 0x0040;
    } else {
        //set ZF = 0
        FLAGS = (FLAGS | 0x0040) ^ 0x0040;
    }

    saved_state32(state)->efl = FLAGS;
}

inline void blsr64(M64 src, M64 *res, uint8_t operand_size, x86_saved_state_t *state) {
    uint64_t FLAGS = saved_state64(state)->isf.rflags;
    int SF;

    if (operand_size == 64) {
        int64_t SRC1 = src.a64;
        int64_t SRC2 = src.a64;
        res->a64 = (SRC1 - 1) & (SRC2);
        
        //set flag
        uint64_t DEST = res->u64;
        //SF = bit[63]
        SF = (DEST >> 63) & 1;
        if (SF == 1) {
            //set SF = 1
            FLAGS = FLAGS | 0x0080;
        } else {
            //set SF = 0
            FLAGS = (FLAGS | 0x0080) ^ 0x0080;
        }
        if (DEST == 0) {
            //set ZF = 1
            FLAGS = FLAGS | 0x0040;
        } else {
            //set ZF = 0
            FLAGS = (FLAGS | 0x0040) ^ 0x0040;
        }
        if (SRC2 == 0) {
            //set CF = 1
            FLAGS = FLAGS | 0x0001;
        } else {
            //set CF = 0
            FLAGS = (FLAGS | 0x0001) ^ 0x0001;
        }

    } else {
        int32_t SRC1 = src.a32[0];
        int32_t SRC2 = src.a32[0];
        res->a32[0] = (SRC1 - 1) & (SRC2);
        
        //set flag
        uint32_t DEST = res->u32[0];
        //SF = bit[31]
        SF = (DEST >> 31) & 1;
        if (SF == 1) {
            //set SF = 1
            FLAGS = FLAGS | 0x0080;
        } else {
            //set SF = 0
            FLAGS = (FLAGS | 0x0080) ^ 0x0080;
        }
        if (DEST == 0) {
            //set ZF = 1
            FLAGS = FLAGS | 0x0040;
        } else {
            //set ZF = 0
            FLAGS = (FLAGS | 0x0040) ^ 0x0040;
        }
        if (SRC2 == 0) {
            //set CF = 1
            FLAGS = FLAGS | 0x0001;
        } else {
            //set CF = 0
            FLAGS = (FLAGS | 0x0001) ^ 0x0001;
        }
    }
    saved_state64(state)->isf.rflags = FLAGS;
}

inline void blsr32(M32 src, M32 *res, x86_saved_state_t *state) {
    uint32_t FLAGS = saved_state32(state)->efl;
    int SF;

    int32_t SRC1 = src.a32;
    int32_t SRC2 = src.a32;
    res->a32 = (SRC1 - 1) & (SRC2);

    //set flag
    uint32_t DEST = res->u32;
    //SF = bit[31]
    SF = (DEST >> 31) & 1;
    if (SF == 1) {
        //set SF = 1
        FLAGS = FLAGS | 0x0080;
    } else {
        //set SF = 0
        FLAGS = (FLAGS | 0x0080) ^ 0x0080;
    }
    if (DEST == 0) {
        //set ZF = 1
        FLAGS = FLAGS | 0x0040;
    } else {
        //set ZF = 0
        FLAGS = (FLAGS | 0x0040) ^ 0x0040;
    }
    if (SRC2 == 0) {
        //set CF = 1
        FLAGS = FLAGS | 0x0001;
    } else {
        //set CF = 0
        FLAGS = (FLAGS | 0x0001) ^ 0x0001;
    }
    saved_state32(state)->efl = FLAGS;
}

inline void blsmsk64(M64 src, M64 *res, uint8_t operand_size, x86_saved_state_t *state) {
    uint64_t FLAGS = saved_state64(state)->isf.rflags;
    int SF;

    if (operand_size == 64) {
        int64_t SRC1 = src.a64;
        int64_t SRC2 = src.a64;
        res->a64 = (SRC1 - 1) ^ (SRC2);

        //set flag
        uint64_t DEST = res->u64;
        //SF = bit[63]
        SF = (DEST >> 63) & 1;
        if (SF == 1) {
            //set SF = 1
            FLAGS = FLAGS | 0x0080;
        } else {
            //set SF = 0
            FLAGS = (FLAGS | 0x0080) ^ 0x0080;
        }
        
        //set ZF = 0
        FLAGS = (FLAGS | 0x0040) ^ 0x0040;

        if (SRC2 == 0) {
            //set CF = 1
            FLAGS = FLAGS | 0x0001;
        } else {
            //set CF = 0
            FLAGS = (FLAGS | 0x0001) ^ 0x0001;
        }
    } else {
        int32_t SRC1 = src.a32[0];
        int32_t SRC2 = src.a32[0];
        res->a32[0] = (SRC1 - 1) ^ (SRC2);

        //set flag
        uint32_t DEST = res->u32[0];
        //SF = bit[31]
        SF = (DEST >> 31) & 1;
        if (SF == 1) {
            //set SF = 1
            FLAGS = FLAGS | 0x0080;
        } else {
            //set SF = 0
            FLAGS = (FLAGS | 0x0080) ^ 0x0080;
        }
        
        //set ZF = 0
        FLAGS = (FLAGS | 0x0040) ^ 0x0040;

        if (SRC2 == 0) {
            //set CF = 1
            FLAGS = FLAGS | 0x0001;
        } else {
            //set CF = 0
            FLAGS = (FLAGS | 0x0001) ^ 0x0001;
        }

    }
    saved_state64(state)->isf.rflags = FLAGS;
}

inline void blsmsk32(M32 src, M32 *res, x86_saved_state_t *state) {
    uint32_t FLAGS = saved_state32(state)->efl;
    int SF;

    int32_t SRC = src.a32;
    res->a32 = (SRC - 1) ^ (SRC);
    
    //set flag
    uint32_t DEST = res->u32;
    //SF = bit[31]
    SF = (DEST >> 31) & 1;
    if (SF == 1) {
        //set SF = 1
        FLAGS = FLAGS | 0x0080;
    } else {
        //set SF = 0
        FLAGS = (FLAGS | 0x0080) ^ 0x0080;
    }

    //set ZF = 0
    FLAGS = (FLAGS | 0x0040) ^ 0x0040;

    if (SRC == 0) {
        //set CF = 1
        FLAGS = FLAGS | 0x0001;
    } else {
        //set CF = 0
        FLAGS = (FLAGS | 0x0001) ^ 0x0001;
    }

    saved_state32(state)->efl = FLAGS;

}

inline void blsi64(M64 src, M64 *res, uint8_t operand_size, x86_saved_state_t *state) {
    uint64_t FLAGS = saved_state64(state)->isf.rflags;
    int SF;

    if (operand_size == 64) {
        int64_t SRC1 = src.a64;
        int64_t SRC2 = src.a64;
        res->a64 = (-SRC1) & (SRC2);
        
        //set flag
        uint64_t DEST = res->u64;
        //SF = bit[63]
        SF = (DEST >> 63) & 1;
        if (SF == 1) {
            //set SF = 1
            FLAGS = FLAGS | 0x0080;
        } else {
            //set SF = 0
            FLAGS = (FLAGS | 0x0080) ^ 0x0080;
        }
        if (DEST == 0) {
            //set ZF = 1
            FLAGS = FLAGS | 0x0040;
        } else {
            //set ZF = 0
            FLAGS = (FLAGS | 0x0040) ^ 0x0040;
        }
        if (SRC2 == 0) {
            //set CF = 0
            FLAGS = (FLAGS | 0x0001) ^ 0x0001;
        } else {
            //set CF = 1
            FLAGS = FLAGS | 0x0001;
        }

    } else {
        int32_t SRC1 = src.a32[0];
        int32_t SRC2 = src.a32[0];
        res->a32[0] = (-SRC1 ) & (SRC2);
        
        //set flag
        uint32_t DEST = res->u32[0];
        //SF = bit[31]
        SF = (DEST >> 31) & 1;
        if (SF == 1) {
            //set SF = 1
            FLAGS = FLAGS | 0x0080;
        } else {
            //set SF = 0
            FLAGS = (FLAGS | 0x0080) ^ 0x0080;
        }
        if (DEST == 0) {
            //set ZF = 1
            FLAGS = FLAGS | 0x0040;
        } else {
            //set ZF = 0
            FLAGS = (FLAGS | 0x0040) ^ 0x0040;
        }
        if (SRC2 == 0) {
            //set CF = 0
            FLAGS = (FLAGS | 0x0001) ^ 0x0001;
        } else {
            //set CF = 1
            FLAGS = FLAGS | 0x0001;
        }
    }
    saved_state64(state)->isf.rflags = FLAGS;
}

inline void blsi32(M32 src, M32 *res, x86_saved_state_t *state) {
    uint32_t FLAGS = saved_state32(state)->efl;
    int SF;

    int32_t SRC1 = src.a32;
    int32_t SRC2 = src.a32;
    res->a32 = (-SRC1 ) & (SRC2);

    //set flag
    uint32_t DEST = res->u32;
    //SF = bit[31]
    SF = (DEST >> 31) & 1;
    if (SF == 1) {
        //set SF = 1
        FLAGS = FLAGS | 0x0080;
    } else {
        //set SF = 0
        FLAGS = (FLAGS | 0x0080) ^ 0x0080;
    }
    if (DEST == 0) {
        //set ZF = 1
        FLAGS = FLAGS | 0x0040;
    } else {
        //set ZF = 0
        FLAGS = (FLAGS | 0x0040) ^ 0x0040;
    }
    if (SRC2 == 0) {
        //set CF = 0
        FLAGS = (FLAGS | 0x0001) ^ 0x0001;
    } else {
        //set CF = 1
        FLAGS = FLAGS | 0x0001;
    }
    saved_state32(state)->efl = FLAGS;
}

/**********************************************/
/**  BMI2  instructions implementation       **/
/**********************************************/
inline void bzhi64(M64 src, M64 vsrc, M64 *res, uint8_t operand_size, x86_saved_state_t *state) {
    uint64_t FLAGS = saved_state64(state)->isf.rflags;
    
    uint8_t INDEX = vsrc.u8[0];

    if (operand_size == 64) {
        uint64_t SRC = src.u64;
        if (INDEX < 64) {
            SRC &= ((1LL << INDEX) -1);
        }
        res->u64 = SRC;

        if (INDEX > 63) {
            //CF = 1
            FLAGS = FLAGS | 0x0001;
        } else {
            //CF = 0
            FLAGS = (FLAGS | 0x0001) ^ 0x0001;
        }

    } else {
        uint32_t SRC = src.u32[0];
        if (INDEX < 32) {
            SRC &= (1 << INDEX) -1;
        }
        res->u32[0] = SRC;
        
        if (INDEX > 31) {
            //CF = 1
            FLAGS = FLAGS | 0x0001;
        } else {
            //CF = 0
            FLAGS = (FLAGS | 0x0001) ^ 0x0001;
        }
    }
    
    saved_state64(state)->isf.rflags = FLAGS;
}

inline void bzhi32(M32 src, M32 vsrc, M32 *res, x86_saved_state_t *state) {
    uint32_t FLAGS = saved_state32(state)->efl;
    
    uint8_t INDEX = vsrc.u8[0];
    uint32_t SRC = src.u32;

    if (INDEX < 32) {
        SRC &= (1 << INDEX) -1;
    }
    res->u32 = SRC;
    
    if (INDEX > 31) {
        //CF = 1
        FLAGS = FLAGS | 0x0001;
    } else {
        //CF = 0
        FLAGS = (FLAGS | 0x0001) ^ 0x0001;
    }

    saved_state32(state)->efl = FLAGS;
}

inline void mulx64(M64 src, M64 *res, M64 *dres, x86_saved_state_t *state, uint8_t operand_size) {
    uint64_t RDX = saved_state64(state)->rdx;
    
    if (operand_size == 64) {
        __uint128_t SRC1 = RDX;
        __uint128_t SRC2 = src.u64;
        __uint128_t TMP = SRC1 * SRC2;

        uint64_t DEST2 = TMP & 0xffffffffffffffff;
        dres->u64 = DEST2;

        uint64_t DEST1 = TMP >> 64;
        res->u64 = DEST1;
        
    } else {
        uint64_t SRC1 = RDX & 0xffffffff;
        uint64_t SRC2 = src.u64 & 0xffffffff;
        uint64_t TMP = SRC1 * SRC2;

        uint32_t DEST2 = TMP & 0xffffffff;
        dres->u32[0] = DEST2;

        uint32_t DEST1 = TMP >> 32;
        res->u32[0] = DEST1;
    }
}

inline void mulx32(M32 src, M32 *res, M32 *dres, x86_saved_state_t *state) {
    uint32_t EDX = saved_state32(state)->edx;

    uint64_t SRC1 = EDX;
    uint64_t SRC2 = src.u32;
    uint64_t TMP = SRC1 * SRC2;

    uint32_t DEST2 = TMP & 0xffffffff;
    dres->u32 = DEST2;

    uint32_t DEST1 = TMP >> 32;
    res->u32 = DEST1;
}

inline void pdep64(M64 src, M64 vsrc, M64 *res, uint8_t operand_size) {
    int i = 0;
    int k = 0;

    if (operand_size == 64) {
        uint64_t DSET = 0;
        uint64_t MASK = src.u64;
        uint64_t TEMP = vsrc.u64;
        for (i = 0; i < 64; ++i)
            if (MASK & (1LL << i)) {
                DSET |= ((TEMP & (1LL << k)) >> k) << i;
                ++k;
            }
        
        res->u64 = DSET;
    } else {
        uint32_t DSET = 0;
        uint32_t MASK = src.u32[0];
        uint32_t TEMP = vsrc.u32[0];
        for (i = 0; i < 32; ++i)
            if (MASK & (1 << i)) {
                DSET |= ((TEMP & (1 << k)) >> k) << i;
                ++k;
            }
        
        res->u32[0] = DSET;
    }
}

inline void pdep32(M32 src, M32 vsrc, M32 *res) {
    int i = 0;
    int k = 0;

    uint32_t DSET = 0;
    uint32_t MASK = src.u32;
    uint32_t TEMP = vsrc.u32;
    for (i = 0; i < 32; ++i)
        if (MASK & (1 << i)) {
            DSET |= ((TEMP & (1 << k)) >> k) << i;
            ++k;
        }

    res->u32 = DSET;
}

inline void pext64(M64 src, M64 vsrc, M64 *res, uint8_t operand_size) {
    int i = 0;
    int k = 0;
    
    if (operand_size == 64) {
        uint64_t DSET = 0;
        uint64_t MASK = src.u64;
        uint64_t TEMP = vsrc.u64;
        for (i = 0; i < 64; ++i)
            if (MASK & (1LL << i)) {
                DSET |= ((TEMP & (1LL << i)) >> i) << k;
                ++k;
            }
            
        res->u64 = DSET;
    } else {
        uint32_t DSET = 0;
        uint32_t MASK = src.u32[0];
        uint32_t TEMP = vsrc.u32[0];
        for (i = 0; i < 32; ++i)
            if (MASK & (1 << i)) {
                DSET |= ((TEMP & (1 << i)) >> i) << k;
                ++k;
            }
        
        res->u32[0] = DSET;
    }
}

inline void pext32(M32 src, M32 vsrc, M32 *res) {
    int i = 0;
    int k = 0;

    uint32_t DSET = 0;
    uint32_t MASK = src.u32;
    uint32_t TEMP = vsrc.u32;
    for (i = 0; i < 32; ++i)
        if (MASK & (1 << i)) {
            DSET |= ((TEMP & (1 << i)) >> i) << k;
            ++k;
        }

    res->u32 = DSET;
}

inline void rorx64(M64 src, M64 *res, uint8_t imm, uint8_t operand_size) {
    if (operand_size == 64) {
        int y = imm & 0x3F;
        uint64_t SRC = src.u64;
        res->u64 = (SRC >> y) | (SRC << (64 - y));
        res->u64 = SRC;
    } else {
        int y = imm & 0x1F;
        uint32_t SRC = src.u32[0];
        res->u32[0] = (SRC >> y) | (SRC << (32 - y));
        res->u32[0] = SRC;
    }
}

inline void rorx32(M32 src, M32 *res, uint8_t imm) {
    int y = imm & 0x1F;
    uint32_t SRC = src.u32;
    res->u32 = (SRC >> y) | (SRC << (32 - y));
    res->u32 = SRC;

}

inline void shlx64(M64 src, M64 vsrc, M64 *res, uint8_t operand_size) {
    if (operand_size == 64) {
        int count = vsrc.u8[0] & 63;
        uint64_t temp = src.u64;
        res->u64 = temp << count;
    } else {
        int count = vsrc.u8[0] & 31;
        uint32_t temp = src.u32[0];
        res->u32[0] = temp << count;
    }
}

inline void shlx32(M32 src, M32 vsrc, M32 *res) {
    int count = vsrc.u8[0] & 31;
    uint32_t temp = src.u32;
    res->u32 = temp << count;
}

inline void sarx64(M64 src, M64 vsrc, M64 *res, uint8_t operand_size) {
    if (operand_size == 64) {
        int count = vsrc.u8[0] & 63;
        int64_t temp = src.a64;
        res->a64 = temp >> count;
    } else {
        int count = vsrc.u8[0] & 31;
        int32_t temp =  src.a32[0];
        res->a32[0] = temp >> count;
    }
}

inline void sarx32(M32 src, M32 vsrc, M32 *res) {
    int count = vsrc.u8[0] & 31;
    int32_t temp =  src.a32;
    res->a32 = temp >> count;
}

inline void shrx64(M64 src, M64 vsrc, M64 *res, uint8_t operand_size) {
    if (operand_size == 64) {
        int count = vsrc.u8[0] & 63;
        uint64_t temp = src.u64;
        res->u64 = temp >> count;
    } else {
        int count = vsrc.u8[0] & 31;
        uint32_t temp =  src.u32[0];
        res->u32[0] = temp >> count;
    }
}

inline void shrx32(M32 src, M32 vsrc, M32 *res) {
    int count = vsrc.u8[0] & 31;
    uint32_t temp =  src.u32;
    res->u32 = temp >> count;
}

#endif /* bmi_hpp */
