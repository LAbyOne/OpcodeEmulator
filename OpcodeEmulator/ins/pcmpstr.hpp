//
//  pcmpstr.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/5/18.
//  Copyright © 2019 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef pcmpstr_hpp
#define pcmpstr_hpp

#include "opemu.hpp"

int calc_str_len(__int128_t val, const int mode);
void override_invalid(unsigned char res[16][16], int la, int lb, const int mode, int dim);
void calc_matrix(__int128_t a, int la, __int128_t b, int lb, const int mode, unsigned char res[16][16]);
int pcmpstr_calc_res(__int128_t a, int la, __int128_t b, int lb, const int mode);
int cmp_indexed(__int128_t a, int la, __int128_t b, int lb, const int mode, int *res2);
int cmp_flags(__int128_t a, int la, __int128_t b, int lb, int mode, int res2, int is_implicit);
__int128_t cmp_masked(__int128_t a, int la, __int128_t b, int lb, const int mode, int *res2);
int cmp_ei(__int128_t *a, int la, __int128_t *b, int lb, const int mode, int *flags);
int cmp_ii(__int128_t *a, __int128_t *b, const int mode, int *flags);
__int128_t cmp_em(__int128_t *a, int la, __int128_t *b, int lb, const int mode, int *flags );
__int128_t cmp_im(__int128_t *a, __int128_t *b, const int mode, int *flags);

#endif /* pcmpstr_hpp */
