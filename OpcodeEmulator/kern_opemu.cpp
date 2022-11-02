//
//  kern_opemu.cpp
//  OPEMU
//
//  Created by Meowthra on 2017/11/14.
//  Copyright © 2017年 Meowthra. All rights reserved.
//  Made in Taiwan.

#include <IOKit/IOService.h>

#include <Headers/kern_api.hpp>
#include <Headers/kern_file.hpp>
#include <Headers/kern_compat.hpp>
#include <Headers/kern_compression.hpp>

#include "kern_opemu.hpp"
#include "opemu.hpp"

static OPEMU *callbackOPEMU = nullptr;
static KernelPatcher *callbackPatcher = nullptr;

//=======================================================================================//
void OPEMU::kernel_trap(x86_saved_state_t *state, uintptr_t *lo_spp) {
    x86_saved_state64_t *regs;
    regs = saved_state64(state);
    int         type;
    type  = regs->isf.trapno;

    if (type == 6) {
       callbackOPEMU->opemu_ktrap(state, lo_spp);
    } else {
        return FunctionCast(kernel_trap, callbackOPEMU->orgkerneltrap)(state, lo_spp);
    }
}

//=======================================================================================//
void OPEMU::user_trap(x86_saved_state_t *saved_state) {
    int         type;
    int         exc;
    int         err;
    mach_exception_code_t   code;
    mach_exception_subcode_t subcode;
    if (is_saved_state64(saved_state)){
        x86_saved_state64_t *regs;
        regs = saved_state64(saved_state);
        err  = (int)regs->isf.err & 0xffff;
        type = regs->isf.trapno;
        
    } else {
        x86_saved_state32_t *regs;
        regs = saved_state32(saved_state);
        err   = regs->err & 0xffff;
        type  = regs->trapno;
    }
    code = 0;
    subcode = 0;
    exc = 0;

    if (type == 6) {
        callbackOPEMU->opemu_utrap(saved_state);
        callbackOPEMU->orgthreadexceptionreturn();
        //opemu_utrap(saved_state);
        //exc = 2; //EXC_BAD_INSTRUCTION
        //code = 1; //EXC_I386_INVOP
    } else {
        return FunctionCast(user_trap, callbackOPEMU->orgusertrap)(saved_state);
    }
}

//=======================================================================================//
void OPEMU::processKernel(KernelPatcher &patcher) {

	if (!(progressState & ProcessingState::KernelRouted)) {
		//hook kernel_trap
        orgkerneltrap = patcher.solveSymbol(KernelPatcher::KernelID, "_kernel_trap");
        if (orgkerneltrap) {
            DBGLOG("OPEMU", "obtained _kernel_trap");
            
            KernelPatcher::RouteRequest request("_kernel_trap", kernel_trap, orgkerneltrap);
             patcher.routeMultipleLong(KernelPatcher::KernelID, &request, 1);
            DBGLOG("OPEMU", "routed _kernel_trap");
            
        } else {
            SYSLOG("OPEMU", "failed to route kernel_trap");
        }
    
             

        //hook user_trap
        orgusertrap = patcher.solveSymbol(KernelPatcher::KernelID, "_user_trap");
        if (orgusertrap) {
            DBGLOG("OPEMU", "obtained _user_trap");

            KernelPatcher::RouteRequest request("_user_trap", user_trap, orgusertrap);
             patcher.routeMultipleLong(KernelPatcher::KernelID, &request, 1);
            DBGLOG("OPEMU", "routed _user_trap");
        } else {
            SYSLOG("OPEMU", "failed to route user_trap");
        }
        //hook thread_exception_return
        orgthreadexceptionreturn = reinterpret_cast<t_thread_exception_return>(patcher.solveSymbol(KernelPatcher::KernelID, "_thread_exception_return"));
        if (orgthreadexceptionreturn) {
            DBGLOG("OPEMU", "obtained _thread_exception_return");
        } else {
            DBGLOG("OPEMU", "failed to resolve _thread_exception_return");
        }
        
        //hook unix_syscall
        orgunixsyscall = reinterpret_cast<t_unix_syscall>(patcher.solveSymbol(KernelPatcher::KernelID, "_unix_syscall"));
        if (orgunixsyscall) {
            DBGLOG("OPEMU", "obtained _unix_syscall");
        } else {
            DBGLOG("OPEMU", "failed to resolve _unix_syscall");
        }
        
        //hook mach_call_munger
        orgmachcallmunger = reinterpret_cast<t_mach_call_munger>(patcher.solveSymbol(KernelPatcher::KernelID, "_mach_call_munger"));
        if (orgmachcallmunger) {
            DBGLOG("OPEMU", "obtained _mach_call_munger");
        } else {
            DBGLOG("OPEMU", "failed to resolve _mach_call_munger");
        }
        
        progressState |= ProcessingState::KernelRouted;
	}
    patcher.clearError();
}

//=======================================================================================//
bool OPEMU::init() {
    // Kernel
    LiluAPI::Error error = lilu.onPatcherLoad([](void *user, KernelPatcher &patcher) {
        callbackOPEMU = static_cast<OPEMU *>(user);
        callbackPatcher = &patcher;
        callbackOPEMU->processKernel(patcher);
    }, this);
    
    if (error != LiluAPI::Error::NoError) {
        SYSLOG("OPEMU", "failed to register onPatcherLoad method %d", error);
        return false;
    }
    return true;
}

//=======================================================================================//
void OPEMU::deinit() {}

//=======================================================================================//
void OPEMU::opemu_ktrap(x86_saved_state_t *state, uintptr_t *lo_spp)
{
    vm_offset_t addr;
    x86_saved_state64_t *regs;
    regs = saved_state64(state);
    addr = regs->isf.rip;
    uint8_t *code_buffer = (uint8_t*) addr;
    unsigned int bytes_skip = 0;
    
    uint64_t recovery_ip = 0;
    
    // "lock" instruction return
    if (code_buffer[0]==0xF0) {
        return FunctionCast(kernel_trap, callbackOPEMU->orgkerneltrap)(state, lo_spp);
    }
    // "ud2" instruction return
    if ( (code_buffer[0]==0x0F) && (code_buffer[1]==0x0B) ) {
        return FunctionCast(kernel_trap, callbackOPEMU->orgkerneltrap)(state, lo_spp);
    }

    //Enable REX Opcode Emulation
    bytes_skip = rex_ins(code_buffer, state, 1);
    
    //Enable VEX Opcode Emulation
    if (bytes_skip == 0) {
        bytes_skip = vex_ins(code_buffer, state, 1);
    }
    
    recovery_ip = (regs->isf.rip + bytes_skip);
    regs->isf.rip = recovery_ip;

    if (!bytes_skip)
    {
        printf("OpcodeEmulator invalid kernel opcode (64-bit): %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", code_buffer[0], code_buffer[1], code_buffer[2], code_buffer[3], code_buffer[4], code_buffer[5], code_buffer[6], code_buffer[7], code_buffer[8], code_buffer[9]);
        return FunctionCast(kernel_trap, callbackOPEMU->orgkerneltrap)(state, lo_spp);
    }
}

void OPEMU::opemu_utrap(x86_saved_state_t *state)
{
    unsigned int bytes_skip = 0;
    
    if (is_saved_state64(state))
    {
        vm_offset_t addr;
        x86_saved_state64_t *regs;
        regs = saved_state64(state);
        addr = regs->isf.rip;
        uint8_t *code_buffer = (uint8_t*) addr;
        
        uint64_t recovery_ip = 0;
        
        // "lock" instruction return
        if (code_buffer[0]==0xF0) {
            return FunctionCast(user_trap, callbackOPEMU->orgusertrap)(state);
        }
        // "ud2" instruction return
        if ( (code_buffer[0]==0x0F) && (code_buffer[1]==0x0B) ) {
            return FunctionCast(user_trap, callbackOPEMU->orgusertrap)(state);
        }

        //Enable REX Opcode Emulation
        bytes_skip = rex_ins(code_buffer, state, 0);
        
        //Enable VEX Opcode Emulation
        if (bytes_skip == 0) {
            bytes_skip = vex_ins(code_buffer, state, 0);
        }
            
        recovery_ip = (regs->isf.rip + bytes_skip);
        regs->isf.rip = recovery_ip;
        
        if (!bytes_skip)
        {
            printf("OpcodeEmulator invalid user opcode (64-bit): %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", code_buffer[0], code_buffer[1], code_buffer[2], code_buffer[3], code_buffer[4], code_buffer[5], code_buffer[6], code_buffer[7], code_buffer[8], code_buffer[9]);
            return FunctionCast(user_trap, callbackOPEMU->orgusertrap)(state);
        }
    }
    else
    {
        vm_offset_t addr;
        x86_saved_state32_t *regs;
        regs = saved_state32(state);
        addr = regs->eip;
        uint8_t *code_buffer = (uint8_t*) addr;
        
        uint32_t recovery_ip = 0;
        
        // "lock" instruction return
        if (code_buffer[0]==0xF0) {
            return FunctionCast(user_trap, callbackOPEMU->orgusertrap)(state);
        }
        // "ud2" instruction return
        if ( (code_buffer[0]==0x0F) && (code_buffer[1]==0x0B) ) {
            return FunctionCast(user_trap, callbackOPEMU->orgusertrap)(state);
        }

        // sysenter/sysexit for AMD Machines 32-bit Mode
        // sysenter
        if ( (code_buffer[0]==0x0F) && (code_buffer[1]==0x34) ) {
            regs->eip = regs->edx;
            regs->uesp = regs->ecx;
            
            if ((signed int)regs->eax < 0) {
                callbackOPEMU->orgmachcallmunger(state);
            } else {
                callbackOPEMU->orgunixsyscall(state);
            }
            return;
        }
        // sysexit
        if ( (code_buffer[0]==0x0F) && (code_buffer[1]==0x35) ) {
            regs->eip = regs->edx;
            regs->uesp = regs->ecx;
            return;
        }
        
        //Enable REX Opcode Emulation
        bytes_skip = rex_ins(code_buffer, state, 0);
        
        //Enable VEX Opcode Emulation
        if (bytes_skip == 0) {
            bytes_skip = vex_ins(code_buffer, state, 0);
        }
        
        recovery_ip = (regs->eip + bytes_skip);
        regs->eip = recovery_ip;
        
        if (!bytes_skip)
        {
            printf("OpcodeEmulator invalid user opcode (32-bit): %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", code_buffer[0], code_buffer[1], code_buffer[2], code_buffer[3], code_buffer[4], code_buffer[5], code_buffer[6], code_buffer[7], code_buffer[8], code_buffer[9]);
            return FunctionCast(user_trap, callbackOPEMU->orgusertrap)(state);
        }
    }
}
