//
//  kern_start.cpp
//  OPEMU
//
//  Created by Meowthra on 2017/11/14.
//  Copyright © 2017年 Meowthra. All rights reserved.
//  Made in Taiwan.

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>

#include "kern_config.hpp"
#include "kern_opemu.hpp"

static OPEMU opemu;

const char *Configuration::bootargOff[] {
    "-opemuoff"
};

const char *Configuration::bootargDebug[] {
    "-opemudbg"
};

const char *Configuration::bootargBeta[] {
    "-opemubeta"
};

PluginConfiguration ADDPR(config) {
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    LiluAPI::AllowNormal,
    config.bootargOff,
    arrsize(config.bootargOff),
    config.bootargDebug,
    arrsize(config.bootargDebug),
    config.bootargBeta,
    arrsize(config.bootargBeta),
    KernelVersion::MountainLion,
    KernelVersion::Mojave,
    []() {
        opemu.init();
    }
};
