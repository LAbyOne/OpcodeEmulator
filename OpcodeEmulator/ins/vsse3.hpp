//
//  vsse3.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/4/8.
//  Copyright © 2019年 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef vsse3_hpp
#define vsse3_hpp

#include "opemu.hpp"

int vsse3_instruction(x86_saved_state_t *state,
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
/**  VSSE3  instructions implementation       **/
/**********************************************/
inline void vmovsldup_128(XMM src, XMM *res) {
    res->fa32[0] = src.fa32[0];
    res->fa32[1] = src.fa32[0];
    res->fa32[2] = src.fa32[2];
    res->fa32[3] = src.fa32[2];
}
inline void vmovsldup_256(YMM src, YMM *res) {
    res->fa32[0] = src.fa32[0];
    res->fa32[1] = src.fa32[0];
    res->fa32[2] = src.fa32[2];
    res->fa32[3] = src.fa32[2];
    res->fa32[4] = src.fa32[4];
    res->fa32[5] = src.fa32[4];
    res->fa32[6] = src.fa32[6];
    res->fa32[7] = src.fa32[6];
}

inline void vmovddup_128(XMM src, XMM *res) {
    res->fa64[0] = src.fa64[0];
    res->fa64[1] = src.fa64[0];
}
inline void vmovddup_256(YMM src, YMM *res) {
    res->fa64[0] = src.fa64[0];
    res->fa64[1] = src.fa64[0];
    res->fa64[2] = src.fa64[2];
    res->fa64[3] = src.fa64[2];
}

inline void vmovshdup_128(XMM src, XMM *res) {
    res->fa32[0] = src.fa32[1];
    res->fa32[1] = src.fa32[1];
    res->fa32[2] = src.fa32[3];
    res->fa32[3] = src.fa32[3];
}
inline void vmovshdup_256(YMM src, YMM *res) {
    res->fa32[0] = src.fa32[1];
    res->fa32[1] = src.fa32[1];
    res->fa32[2] = src.fa32[3];
    res->fa32[3] = src.fa32[3];
    res->fa32[4] = src.fa32[5];
    res->fa32[5] = src.fa32[5];
    res->fa32[6] = src.fa32[7];
    res->fa32[7] = src.fa32[7];
}

inline void vlddqu_128(XMM src, XMM *res) {
    res->u128 = src.u128;
}
inline void vlddqu_256(YMM src, YMM *res) {
    res->u256 = src.u256;
}

inline void vhaddpd_128(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = vsrc.fa64[0] + vsrc.fa64[1];
    res->fa64[1] = src.fa64[0] + src.fa64[1];
}
inline void vhaddpd_256(YMM src, YMM vsrc, YMM *res) {
    res->fa64[0] = vsrc.fa64[0] + vsrc.fa64[1];
    res->fa64[1] = src.fa64[0] + src.fa64[1];
    res->fa64[2] = vsrc.fa64[2] + vsrc.fa64[3];
    res->fa64[3] = src.fa64[2] + src.fa64[3];
}

inline void vhaddps_128(XMM src, XMM vsrc, XMM *res) {
    res->fa32[0] = vsrc.fa32[0] + vsrc.fa32[1];
    res->fa32[1] = vsrc.fa32[2] + vsrc.fa32[3];
    res->fa32[2] = src.fa32[0] + src.fa32[1];
    res->fa32[3] = src.fa32[2] + src.fa32[3];
}
inline void vhaddps_256(YMM src, YMM vsrc, YMM *res) {
    res->fa32[0] = vsrc.fa32[0] + vsrc.fa32[1];
    res->fa32[1] = vsrc.fa32[2] + vsrc.fa32[3];
    res->fa32[2] = src.fa32[0] + src.fa32[1];
    res->fa32[3] = src.fa32[2] + src.fa32[3];
    res->fa32[4] = vsrc.fa32[4] + vsrc.fa32[5];
    res->fa32[5] = vsrc.fa32[6] + vsrc.fa32[7];
    res->fa32[6] = src.fa32[4] + src.fa32[5];
    res->fa32[7] = src.fa32[6] + src.fa32[7];
}

inline void vhsubpd_128(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = vsrc.fa64[0] - vsrc.fa64[1];
    res->fa64[1] = src.fa64[0] - src.fa64[1];
}
inline void vhsubpd_256(YMM src, YMM vsrc, YMM *res) {
    res->fa64[0] = vsrc.fa64[0] - vsrc.fa64[1];
    res->fa64[1] = src.fa64[0] - src.fa64[1];
    res->fa64[2] = vsrc.fa64[2] - vsrc.fa64[3];
    res->fa64[3] = src.fa64[2] - src.fa64[3];
}

inline void vhsubps_128(XMM src, XMM vsrc, XMM *res) {
    res->fa32[0] = vsrc.fa32[0] - vsrc.fa32[1];
    res->fa32[1] = vsrc.fa32[2] - vsrc.fa32[3];
    res->fa32[2] = src.fa32[0] - src.fa32[1];
    res->fa32[3] = src.fa32[2] - src.fa32[3];
}
inline void vhsubps_256(YMM src, YMM vsrc, YMM *res) {
    res->fa32[0] = vsrc.fa32[0] - vsrc.fa32[1];
    res->fa32[1] = vsrc.fa32[2] - vsrc.fa32[3];
    res->fa32[2] = src.fa32[0] - src.fa32[1];
    res->fa32[3] = src.fa32[2] - src.fa32[3];
    res->fa32[4] = vsrc.fa32[4] - vsrc.fa32[5];
    res->fa32[5] = vsrc.fa32[6] - vsrc.fa32[7];
    res->fa32[6] = src.fa32[4] - src.fa32[5];
    res->fa32[7] = src.fa32[6] - src.fa32[7];
}

inline void vaddsubpd_128(XMM src, XMM vsrc, XMM *res) {
    res->fa64[0] = vsrc.fa64[0] - src.fa64[0];
    res->fa64[1] = vsrc.fa64[1] + src.fa64[1];
}
inline void vaddsubpd_256(YMM src, YMM vsrc, YMM *res) {
    res->fa64[0] = vsrc.fa64[0] - src.fa64[0];
    res->fa64[1] = vsrc.fa64[1] + src.fa64[1];
    res->fa64[2] = vsrc.fa64[2] - src.fa64[2];
    res->fa64[3] = vsrc.fa64[3] + src.fa64[3];
}

inline void vaddsubps_128(XMM src, XMM vsrc, XMM *res) {
    res->fa32[0] = vsrc.fa32[0] - src.fa32[0];
    res->fa32[1] = vsrc.fa32[1] + src.fa32[1];
    res->fa32[2] = vsrc.fa32[2] - src.fa32[2];
    res->fa32[3] = vsrc.fa32[3] + src.fa32[3];
}
inline void vaddsubps_256(YMM src, YMM vsrc, YMM *res) {
    res->fa32[0] = vsrc.fa32[0] - src.fa32[0];
    res->fa32[1] = vsrc.fa32[1] + src.fa32[1];
    res->fa32[2] = vsrc.fa32[2] - src.fa32[2];
    res->fa32[3] = vsrc.fa32[3] + src.fa32[3];
    res->fa32[4] = vsrc.fa32[4] - src.fa32[4];
    res->fa32[5] = vsrc.fa32[5] + src.fa32[5];
    res->fa32[6] = vsrc.fa32[6] - src.fa32[6];
    res->fa32[7] = vsrc.fa32[7] + src.fa32[7];
}

#endif /* vsse3_hpp */
