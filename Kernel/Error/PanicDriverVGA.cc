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

#include "Panic.h"

extern "C" {
#include <CoreSystem/String.h>
#include <CoreSystem/MachineInstructions.h>
}

static uint16_t *videoBuffer = (unsigned short *)0xC00B8000;
static uint8_t width = 80;
static uint8_t height = 25;

struct {
	uint8_t x;
	uint8_t y;
	uint8_t attr;
} cursorPosition;

static void VGAPutChar(char c)
{
	if (cursorPosition.y >= height) {
		return;
	}
	
	switch (c) {
		case '\t':
			cursorPosition.x += 4;
			break;
		case '\n':
			cursorPosition.y += 1;
			[[clang::fallthrough]];
		case '\r':
			cursorPosition.x = 1;
			break;
		default:
			if (cursorPosition.x >= width - 1) {
				cursorPosition.x  = 1;
				cursorPosition.y += 1;
			}
			
			videoBuffer[cursorPosition.y * width + cursorPosition.x] = (uint16_t)(cursorPosition.attr << 8) | (uint16_t)c;
			cursorPosition.x += 1;
			break;
	}
}

static void VGAClear()
{
	cursorPosition.x = 1;
	cursorPosition.y = 1;
	cursorPosition.attr = 0x74;
	
	for (uint16_t i = 0; i < width*height; i++) {
		videoBuffer[i] = (uint16_t)(cursorPosition.attr << 8) | 0;
	}
	
	// Disable cursor
	outb(0x3D4, 0x0A);
	uint8_t creg = inb(0x3D5);
	creg |= (1<<5);
	outb(0x3D4, 0x0A);
	outb(0x3D5, creg);
}

void PanicDriverVGA(uint64_t timestamp, const char* message, Interrupts::CPUState* cpuState, va_list args)
{
	VGAClear();
	
	pprintf(VGAPutChar, "Panic\n");
	pprintf(VGAPutChar, "======\n");
	pprintf(VGAPutChar, "Time: %d\n", timestamp);
	pprintf(VGAPutChar, "Message:");
	vpprintf(VGAPutChar, message, args);
	pprintf(VGAPutChar, "\n\n");
	
	if (cpuState) {
		pprintf(VGAPutChar, "CPU State:\n");
		pprintf(VGAPutChar, "  eax = %08x      ebx = %08x ecx = %08x   edx =  %08x\n", cpuState->eax, cpuState->ebx, cpuState->ecx, cpuState->edx);
		pprintf(VGAPutChar, "  ebp = %08x      esi = %08x edi = %08x   eip = %p\n", cpuState->ebp, cpuState->esi, cpuState->edi, cpuState->eip);
		pprintf(VGAPutChar, "   cs = %08x   eflags = %08x esp = %08x    ss =  %08x\n\n", cpuState->cs, cpuState->eflags, cpuState->esp, cpuState->ss);
		
		pprintf(VGAPutChar, "Backtrace:\n");
	}
	else {
		pprintf(VGAPutChar, "No cpu state was supplied!\n\n");
	}
}

PanicRegisterDriver(PanicDriverVGA);
