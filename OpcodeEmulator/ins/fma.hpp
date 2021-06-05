//
//  fma.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/5/1.
//  Copyright © 2019 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef fma_hpp
#define fma_hpp

#include "opemu.hpp"
#include "fpins.hpp"

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
                    int kernel_trap);

/**********************************************/
/**  FMA instructions implementation         **/
/**********************************************/

/************************** vfmadd pd/ps **************************/
inline void vfmadd132pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = src1.fa64[i] * src3.fa64[i] + src2.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfmadd132pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src1.fa64[i] * src3.fa64[i] + src2.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfmadd213pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src1.fa64[i] + src3.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfmadd213pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src1.fa64[i] + src3.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfmadd231pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src3.fa64[i] + src1.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfmadd231pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src3.fa64[i] + src1.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfmadd132ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src1.fa32[i] * src3.fa32[i] + src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfmadd132ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src1.fa32[i] * src3.fa32[i] + src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

inline void vfmadd213ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src1.fa32[i] + src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfmadd213ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src1.fa32[i] + src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

inline void vfmadd231ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src3.fa32[i] + src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfmadd231ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src3.fa32[i] + src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

/************************** vfmadd sd/ss **************************/
inline void vfmadd132sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = src1.fa64[0] * src3.fa64[0] + src2.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}
inline void vfmadd213sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = src2.fa64[0] * src1.fa64[0] + src3.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}
inline void vfmadd231sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = src2.fa64[0] * src3.fa64[0] + src1.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}

inline void vfmadd132ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = src1.fa32[0] * src3.fa32[0] + src2.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}
inline void vfmadd213ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = src2.fa32[0] * src1.fa32[0] + src3.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}
inline void vfmadd231ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = src2.fa32[0] * src3.fa32[0] + src1.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}

/************************** vfmaddsub pd/ps **************************/
inline void vfmaddsub132pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    
    TMP.fa64[0] = src1.fa64[0] * src3.fa64[0] - src2.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
    TMP.fa64[1] = src1.fa64[1] * src3.fa64[1] + src2.fa64[1];
    res->fa64[1] = round_fp64(TMP.fa64[1], rc);
}

inline void vfmaddsub132pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src1.fa64[i] * src3.fa64[i] - src2.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
        
        TMP.fa64[i+1] = src1.fa64[i+1] * src3.fa64[i+1] + src2.fa64[i+1];
        res->fa64[i+1] = round_fp64(TMP.fa64[i+1], rc);

        i++;
    }
}

inline void vfmaddsub213pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    
    TMP.fa64[0] = src2.fa64[0] * src1.fa64[0] - src3.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
    TMP.fa64[1] = src2.fa64[1] * src1.fa64[1] + src3.fa64[1];
    res->fa64[1] = round_fp64(TMP.fa64[1], rc);
}

inline void vfmaddsub213pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src1.fa64[i] - src3.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
        
        TMP.fa64[i+1] = src2.fa64[i+1] * src1.fa64[i+1] + src3.fa64[i+1];
        res->fa64[i+1] = round_fp64(TMP.fa64[i+1], rc);
        
        i++;
    }
}

inline void vfmaddsub231pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    
    TMP.fa64[0] = src2.fa64[0] * src3.fa64[0] - src1.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
    TMP.fa64[1] = src2.fa64[1] * src3.fa64[1] + src1.fa64[1];
    res->fa64[1] = round_fp64(TMP.fa64[1], rc);
}

inline void vfmaddsub231pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src3.fa64[i] - src1.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
        
        TMP.fa64[i+1] = src2.fa64[i+1] * src3.fa64[i+1] + src1.fa64[i+1];
        res->fa64[i+1] = round_fp64(TMP.fa64[i+1], rc);
        
        i++;
    }
}

inline void vfmaddsub132ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src1.fa32[i] * src3.fa32[i] - src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src1.fa32[i+1] * src3.fa32[i+1] + src2.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}
inline void vfmaddsub132ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src1.fa32[i] * src3.fa32[i] - src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src1.fa32[i+1] * src3.fa32[i+1] + src2.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}

inline void vfmaddsub213ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src1.fa32[i] - src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src2.fa32[i+1] * src1.fa32[i+1] + src3.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}
inline void vfmaddsub213ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src1.fa32[i] - src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src2.fa32[i+1] * src1.fa32[i+1] + src3.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}

inline void vfmaddsub231ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src3.fa32[i] - src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src2.fa32[i+1] * src3.fa32[i+1] + src1.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}
inline void vfmaddsub231ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src3.fa32[i] - src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src2.fa32[i+1] * src3.fa32[i+1] + src1.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}

/************************** vfmsub pd/ps **************************/
inline void vfmsub132pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = src1.fa64[i] * src3.fa64[i] - src2.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfmsub132pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src1.fa64[i] * src3.fa64[i] - src2.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfmsub213pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src1.fa64[i] - src3.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfmsub213pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src1.fa64[i] - src3.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfmsub231pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src3.fa64[i] - src1.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfmsub231pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src3.fa64[i] - src1.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfmsub132ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src1.fa32[i] * src3.fa32[i] - src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfmsub132ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src1.fa32[i] * src3.fa32[i] - src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

inline void vfmsub213ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src1.fa32[i] - src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfmsub213ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src1.fa32[i] - src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

inline void vfmsub231ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src3.fa32[i] - src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfmsub231ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src3.fa32[i] - src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

/************************** vfmsub sd/ss **************************/
inline void vfmsub132sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = src1.fa64[0] * src3.fa64[0] - src2.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}
inline void vfmsub213sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = src2.fa64[0] * src1.fa64[0] - src3.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}
inline void vfmsub231sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = src2.fa64[0] * src3.fa64[0] - src1.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}

inline void vfmsub132ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = src1.fa32[0] * src3.fa32[0] - src2.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}
inline void vfmsub213ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = src2.fa32[0] * src1.fa32[0] - src3.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}
inline void vfmsub231ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = src2.fa32[0] * src3.fa32[0] - src1.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}

/************************** vfmsubadd pd/ps **************************/
inline void vfmsubadd132pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    
    TMP.fa64[0] = src1.fa64[0] * src3.fa64[0] + src2.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
    TMP.fa64[1] = src1.fa64[1] * src3.fa64[1] - src2.fa64[1];
    res->fa64[1] = round_fp64(TMP.fa64[1], rc);
}

inline void vfmsubadd132pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src1.fa64[i] * src3.fa64[i] + src2.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
        
        TMP.fa64[i+1] = src1.fa64[i+1] * src3.fa64[i+1] - src2.fa64[i+1];
        res->fa64[i+1] = round_fp64(TMP.fa64[i+1], rc);
        
        i++;
    }
}

inline void vfmsubadd213pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    
    TMP.fa64[0] = src2.fa64[0] * src1.fa64[0] + src3.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
    TMP.fa64[1] = src2.fa64[1] * src1.fa64[1] - src3.fa64[1];
    res->fa64[1] = round_fp64(TMP.fa64[1], rc);
}

inline void vfmsubadd213pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src1.fa64[i] + src3.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
        
        TMP.fa64[i+1] = src2.fa64[i+1] * src1.fa64[i+1] - src3.fa64[i+1];
        res->fa64[i+1] = round_fp64(TMP.fa64[i+1], rc);
        
        i++;
    }
}

inline void vfmsubadd231pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    
    TMP.fa64[0] = src2.fa64[0] * src3.fa64[0] + src1.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
    TMP.fa64[1] = src2.fa64[1] * src3.fa64[1] - src1.fa64[1];
    res->fa64[1] = round_fp64(TMP.fa64[1], rc);
}

inline void vfmsubadd231pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = src2.fa64[i] * src3.fa64[i] + src1.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
        
        TMP.fa64[i+1] = src2.fa64[i+1] * src3.fa64[i+1] - src1.fa64[i+1];
        res->fa64[i+1] = round_fp64(TMP.fa64[i+1], rc);
        
        i++;
    }
}

inline void vfmsubadd132ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src1.fa32[i] * src3.fa32[i] + src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src1.fa32[i+1] * src3.fa32[i+1] - src2.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}
inline void vfmsubadd132ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src1.fa32[i] * src3.fa32[i] + src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src1.fa32[i+1] * src3.fa32[i+1] - src2.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}

inline void vfmsubadd213ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src1.fa32[i] + src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src2.fa32[i+1] * src1.fa32[i+1] - src3.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}
inline void vfmsubadd213ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src1.fa32[i] + src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src2.fa32[i+1] * src1.fa32[i+1] - src3.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}

inline void vfmsubadd231ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src3.fa32[i] + src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src2.fa32[i+1] * src3.fa32[i+1] - src1.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}
inline void vfmsubadd231ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = src2.fa32[i] * src3.fa32[i] + src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
        
        TMP.fa32[i+1] = src2.fa32[i+1] * src3.fa32[i+1] - src1.fa32[i+1];
        res->fa32[i+1] = round_fp32(TMP.fa32[i+1], rc);
        
        i++;
    }
}

/************************** vfnmadd pd/ps **************************/
inline void vfnmadd132pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = -(src1.fa64[i] * src3.fa64[i]) + src2.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfnmadd132pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = -(src1.fa64[i] * src3.fa64[i]) + src2.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfnmadd213pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = -(src2.fa64[i] * src1.fa64[i]) + src3.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfnmadd213pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = -(src2.fa64[i] * src1.fa64[i]) + src3.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfnmadd231pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = -(src2.fa64[i] * src3.fa64[i]) + src1.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfnmadd231pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = -(src2.fa64[i] * src3.fa64[i]) + src1.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfnmadd132ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = -(src1.fa32[i] * src3.fa32[i]) + src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfnmadd132ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = -(src1.fa32[i] * src3.fa32[i]) + src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

inline void vfnmadd213ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = -(src2.fa32[i] * src1.fa32[i]) + src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfnmadd213ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = -(src2.fa32[i] * src1.fa32[i]) + src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

inline void vfnmadd231ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = -(src2.fa32[i] * src3.fa32[i]) + src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfnmadd231ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = -(src2.fa32[i] * src3.fa32[i]) + src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

/************************** vfnmadd sd/ss **************************/
inline void vfnmadd132sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = -(src1.fa64[0] * src3.fa64[0]) + src2.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}
inline void vfnmadd213sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = -(src2.fa64[0] * src1.fa64[0]) + src3.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}
inline void vfnmadd231sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = -(src2.fa64[0] * src3.fa64[0]) + src1.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}

inline void vfnmadd132ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = -(src1.fa32[0] * src3.fa32[0]) + src2.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}
inline void vfnmadd213ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = -(src2.fa32[0] * src1.fa32[0]) + src3.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}
inline void vfnmadd231ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = -(src2.fa32[0] * src3.fa32[0]) + src1.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}

/************************** vfnmsub pd/ps **************************/
inline void vfnmsub132pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = -(src1.fa64[i] * src3.fa64[i]) - src2.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfnmsub132pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = -(src1.fa64[i] * src3.fa64[i]) - src2.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfnmsub213pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = -(src2.fa64[i] * src1.fa64[i]) - src3.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfnmsub213pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = -(src2.fa64[i] * src1.fa64[i]) - src3.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfnmsub231pd_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 2; ++i) {
        TMP.fa64[i] = -(src2.fa64[i] * src3.fa64[i]) - src1.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}
inline void vfnmsub231pd_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa64[i] = -(src2.fa64[i] * src3.fa64[i]) - src1.fa64[i];
        res->fa64[i] = round_fp64(TMP.fa64[i], rc);
    }
}

inline void vfnmsub132ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = -(src1.fa32[i] * src3.fa32[i]) - src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfnmsub132ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = -(src1.fa32[i] * src3.fa32[i]) - src2.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

inline void vfnmsub213ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = -(src2.fa32[i] * src1.fa32[i]) - src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfnmsub213ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = -(src2.fa32[i] * src1.fa32[i]) - src3.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

inline void vfnmsub231ps_128(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    int i;
    XMM TMP;
    
    for (i = 0; i < 4; ++i) {
        TMP.fa32[i] = -(src2.fa32[i] * src3.fa32[i]) - src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}
inline void vfnmsub231ps_256(YMM src3, YMM src2, YMM src1, YMM *res, int rc) {
    int i;
    YMM TMP;
    
    for (i = 0; i < 8; ++i) {
        TMP.fa32[i] = -(src2.fa32[i] * src3.fa32[i]) - src1.fa32[i];
        res->fa32[i] = round_fp32(TMP.fa32[i], rc);
    }
}

/************************** vfnmsub sd/ss **************************/
inline void vfnmsub132sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = -(src1.fa64[0] * src3.fa64[0]) - src2.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}
inline void vfnmsub213sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = -(src2.fa64[0] * src1.fa64[0]) - src3.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}
inline void vfnmsub231sd(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa64[0] = -(src2.fa64[0] * src3.fa64[0]) - src1.fa64[0];
    res->fa64[0] = round_fp64(TMP.fa64[0], rc);
}

inline void vfnmsub132ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = -(src1.fa32[0] * src3.fa32[0]) - src2.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}
inline void vfnmsub213ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = -(src2.fa32[0] * src1.fa32[0]) - src3.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}
inline void vfnmsub231ss(XMM src3, XMM src2, XMM src1, XMM *res, int rc) {
    XMM TMP;
    res->u128 = src1.u128;
    
    TMP.fa32[0] = -(src2.fa32[0] * src3.fa32[0]) - src1.fa32[0];
    res->fa32[0] = round_fp32(TMP.fa32[0], rc);
}

#endif /* fma_hpp */
