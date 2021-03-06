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

#include "VM/Backend.h"

#include "Boot/Bootstrap.h"

#include "Logging/Logging.h"
#include "Memory/PhyMem.h"
#include "Memory/kalloc.h"
#include "Error/Assert.h"
#include <CoreSystem/CommonTypes.h>

using namespace VM::Backend;

namespace VM { 
namespace Backend {

//
// The Kernel Context
// ==================
//

//
// Initialization
// ==============
//

void Initialize()
{
	Native::Initialize();
	
	LogVerbose("VM subsystem initialized.");
}

//
// The Kernel Context
// ==================
//
Ptr<Context> GetKernelContext()
{
	// Casting is a bit tricky here, so
	// just pass down the raw pointer
	return Native::KernelContext;
}

//
// Context base implementation
// ===========================
//

Ptr<Context> Context::create(VMBackendMapOptions defaultOptions)
{
	Ptr<Context> context = new Native::Context(defaultOptions);
	
	return context;
}

VM::Backend::Context::Context(VMBackendMapOptions opts)
{
	this->defaultOptions = opts;
}

VM::Backend::Context::~Context()
{
}

void VM::Backend::Context::beginAccess()
{
	int32_t ac;
		
	ac = __sync_add_and_fetch(&this->accessCounter, 1);
	
	// It had to be >=0 before, so now it must be >= 1
	assert(ac >= 1);
	
	// We're the first, so make it accible
	if (ac == 1) {
		this->makeAccessible();
	}
}

void VM::Backend::Context::endAccess()
{
	int32_t ac;
		
	ac = __sync_sub_and_fetch(&this->accessCounter, 1);
	
	// It had to be >=1 before, so now it must be >=0
	assert(ac >= 0);
	
	// We're the first, so make it accible
	if (ac == 0) {
		this->endAccessible();
	}
}

} // namespace Backend
} // namespace VM
