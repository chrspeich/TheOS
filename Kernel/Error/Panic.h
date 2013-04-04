//
// Copyright (c) 2012, Christian Speich
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
#include <CoreSystem/VariadicArguments.h>

#ifdef __cplusplus
#include "Interrupts/Interrupts.h"
#endif

//
// Causes the kernel to panic with a given message.
//
// Obviously this never returns
//
#ifdef __cplusplus
extern "C" 
#endif
void panic(const char* message, ...) __attribute__((noreturn));

#ifdef __cplusplus

//
// This causes the kernel to panic with a given computed message and
// cpu state. This will be called in some way be panic to capture the
// cpu state.
//
void panic_state(const char* message, Interrupts::CPUState* cpuState, va_list args) __attribute__((noreturn));

//
// Panic drivers
// =============
//

typedef void(*PanicDriver)(uint64_t timestamp, const char* message, Interrupts::CPUState* cpuState, va_list args);

//
// Use this macro on the top level to staticly register a panic driver at compile time
//
#define PanicRegisterDriver(driver) PanicDriver PanicDriver_##driver __attribute__ ((section (".PanicDrivers"))) = &driver

#endif