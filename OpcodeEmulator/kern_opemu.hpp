//
//  kern_opemu.hpp
//  OPEMU
//
//  Created by Meowthra on 2017/11/14.
//  Copyright © 2017年 Meowthra. All rights reserved.
//  Made in Taiwan.

#ifndef kern_opemu_hpp
#define kern_opemu_hpp

#include <Headers/kern_patcher.hpp>
#include <Headers/kern_nvram.hpp>

#define x86_SAVED_STATE32		THREAD_STATE_NONE + 1
#define x86_SAVED_STATE64		THREAD_STATE_NONE + 2

struct x86_64_intr_stack_frame {
    uint16_t	trapno;
    uint16_t	cpu;
    uint32_t 	_pad;
    uint64_t	trapfn;
    uint64_t	err;
    uint64_t	rip;
    uint64_t	cs;
    uint64_t	rflags;
    uint64_t	rsp;
    uint64_t	ss;
};
typedef struct x86_64_intr_stack_frame x86_64_intr_stack_frame_t;

struct x86_saved_state32 {
    uint32_t	gs;
    uint32_t	fs;
    uint32_t	es;
    uint32_t	ds;
    
    uint32_t	edi;
    uint32_t	esi;
    uint32_t	ebp;
    uint32_t	cr2;
    uint32_t	ebx;
    uint32_t	edx;
    uint32_t	ecx;
    uint32_t	eax;
    uint16_t	trapno;
    uint16_t	cpu;
    uint32_t	err;
    uint32_t	eip;
    uint32_t	cs;
    uint32_t	efl;
    uint32_t	uesp;
    uint32_t	ss;
    
};
typedef struct x86_saved_state32 x86_saved_state32_t;

struct x86_saved_state64 {
    uint64_t	rdi;
    uint64_t	rsi;
    uint64_t	rdx;
    uint64_t	r10;
    uint64_t	r8;
    uint64_t	r9;
    
    uint64_t	cr2;
    uint64_t	r15;
    uint64_t	r14;
    uint64_t	r13;
    uint64_t	r12;
    uint64_t	r11;
    uint64_t	rbp;
    uint64_t	rbx;
    uint64_t	rcx;
    uint64_t	rax;
    
    uint32_t	gs;
    uint32_t	fs;
    
    uint64_t 	_pad;
    
    struct	x86_64_intr_stack_frame	isf;
};
typedef struct x86_saved_state64 x86_saved_state64_t;

typedef struct {
    uint32_t			flavor;
    uint32_t			_pad_for_16byte_alignment[3];
    union {
        x86_saved_state32_t	ss_32;
        x86_saved_state64_t	ss_64;
    } uss;
} x86_saved_state_t;

#define	ss_32	uss.ss_32
#define	ss_64	uss.ss_64

static inline boolean_t
is_saved_state64(x86_saved_state_t *iss)
{
    return (iss->flavor == x86_SAVED_STATE64);
}

static inline boolean_t
is_saved_state32(x86_saved_state_t *iss)
{
    return (iss->flavor == x86_SAVED_STATE32);
}

static inline x86_saved_state32_t *
saved_state32(x86_saved_state_t *iss)
{
    return &iss->ss_32;
}

static inline x86_saved_state64_t *
saved_state64(x86_saved_state_t *iss)
{
    return &iss->ss_64;
}

class OPEMU {
public:
	bool init();
	void deinit();

private:
	/**
	 *  Patch kernel
	 *
	 *  @param patcher KernelPatcher instance
	 */
    void processKernel(KernelPatcher &patcher);

    static void kernel_trap(x86_saved_state_t *state, uintptr_t *lo_spp);
    static void user_trap(x86_saved_state_t *saved_state);
    
    void opemu_ktrap(x86_saved_state_t *state, uintptr_t *lo_spp);
    void opemu_utrap(x86_saved_state_t *state);
    
    mach_vm_address_t    orgkerneltrap {};
    mach_vm_address_t    orgusertrap {};
    
    using t_thread_exception_return = void (*) (void);
    t_thread_exception_return orgthreadexceptionreturn {nullptr};

    using t_mach_call_munger = void (*) (x86_saved_state_t *state);
    t_mach_call_munger orgmachcallmunger {nullptr};

    using t_unix_syscall = void (*) (x86_saved_state_t *state);
    t_unix_syscall orgunixsyscall {nullptr};
    
    struct ProcessingState {
        enum {
            NothingReady = 0,
            KernelRouted = 4,
            EverythingDone = KernelRouted,
        };
    };
    
    int progressState {ProcessingState::NothingReady};

};

#endif /* kern_opemu_hpp */
