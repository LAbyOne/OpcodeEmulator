//
//  fpins.hpp
//  OpcodeEmulator
//
//  Created by Kali on 2019/5/15.
//  Copyright © 2019 lvs1974. All rights reserved.
//  Made in Taiwan.

#ifndef fpins_hpp
#define fpins_hpp

#include "opemu.hpp"
int getmxcsr();

float round_fp32(float fp32, int rc);
double round_fp64(double fp64, int rc);

float round_sf(float fp32);
float floor_sf(float fp32);
float ceil_sf(float fp32);
float trunc_sf(float fp32);
float sqrt_sf(float fp32);

double round_df(double fp64);
double floor_df(double fp64);
double ceil_df(double fp64);
double trunc_df(double fp64);
double sqrt_df(double fp64);

int isValidNumber_f32(float fp32);
int isValidNumber_f64(double fp64);

float SNanToQNaN_f32(float fp32);
double SNanToQNaN_f64(double fp64);

int isNaN_f64(double fp64);
int isNaN_f32(float fp32);


#endif /* fpins_hpp */
