//
//  kern_config.h
//  OPEMU
//
//  Created by Meowthra on 2017/11/16.
//  Copyright © 2017年 Meowthra. All rights reserved.
//  Made in Taiwan.

#ifndef kern_config_h
#define kern_config_h

class Configuration {
public:
	/**
	 *  Possible boot arguments
	 */
    static const char *bootargOff[];
    static const char *bootargDebug[];
    static const char *bootargBeta[];
    
public:

    Configuration() = default;
};

extern Configuration config;

#endif /* kern_config_h */
