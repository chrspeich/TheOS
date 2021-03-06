//
// Copyright (c) 2013, Christian Speich
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#pragma once

#include <CoreSystem/CommonTypes.h>

namespace Interrupts {
namespace X86 {

typedef struct {
	uint32_t edi;
  	uint32_t esi;
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	uint32_t interruptNumber;
	uint32_t errorCode;

	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
} CPUState;

//
// Handles an interrupt.
//
// It may return NULL to indicate that it want the cpu
// to be halted.
//
typedef const CPUState* (*Handler)(const CPUState* cpuState);

//
// Initialize the subsystem but does not enable interrupts
// (Exceptions may occour regardless)
//
void Initialize();

//
// Enables interrupts
//
void Enable();

//
// Disable interrupts
//
void Disable();

//
// Sets a handler for a given exception
//
void SetExceptionHandler(uint16_t exceptionNumber, Handler handler);

//
// Gets thee handler set for a given exception
//
Handler GetExceptionHandler(uint16_t exceptionNumber);

//
// Sets a handler for a given irq
//
void SetIRQHandler(uint16_t irqNumber, Handler handler);

//
// Masks a irq
//
void MaskIRQ(uint16_t irqNumber);

//
// Unmask a irq
//
void UnmaskIRQ(uint16_t irqNumber);

//
// Gets the IRQ Mask
//
uint32_t GetIRQMask();

//
// Sets the IRQ Mask
//
void SetIRQMask(uint32_t mask);

//
// Gets thee handler set for a given irq
//
Handler GetIRQHandler(uint16_t irqNumber);

//
// Sets the kernel stack to use when interrupts fires
//
void SetKernelStack(uint32_t stack);

//
// Gets kernel stack
//
uint32_t GetKernelStack();

}
}
