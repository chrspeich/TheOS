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

#include "Process/Thread.h"
#include "Process/Process.h"
#include "Utils/Memutils.h"

namespace Process {

Thread::Thread(uint32_t entryPoint, size_t stackSize, Ptr<Process> _process)
{
	#pragma unused(entryPoint, stackSize)
	this->process = _process;
	assert(this->process->thread != NULL);
	this->process->thread = this;
	memset(&this->cpuState, 0, sizeof(sizeof(Interrupts::CPUState)));

	// Start suspendes
	// This will also add us to the sheduler
	this->setState(ThreadState::Suspended);
}

Thread::~Thread()
{
}

Ptr<Process> Thread::getProcess() const
{
	return this->process;
}

void Thread::setCPUState(const Interrupts::CPUState* _state)
{
	memcpy(&this->cpuState, _state, sizeof(Interrupts::CPUState));
}

const Interrupts::CPUState* Thread::getCPUState() const
{
	return &this->cpuState;
}

void Thread::setState(ThreadState _state)
{
	this->state = _state;
}

ThreadState Thread::getState() const
{
	return this->state;
}

}
