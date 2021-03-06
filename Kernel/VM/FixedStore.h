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

#include "VM/Store.h"

namespace VM {

class FixedStore : public Store {
private:
	bool writeable;
	bool free;
	
	page_t startPage;
	page_t* pages;
	size_t numberOfPages;
protected:
public:
	///
	/// Constructor for a continigous pmem space
	///
	/// @param startPage the first page to use
	/// @param numberOfPages how many pages should be used after startpage
	///
	///
	FixedStore(page_t startPage, size_t numberOfPages, bool writeable = true, bool free = true);
	
	///
	/// Constructor for a non continigous pmem space
	///
	/// @param pages Pointer to an array of pages
	/// @param numberOfPages Number of entries in pages
	/// @param writeable Can the phy pages be written to
	/// @param free Mark pages free when this store deallocates
	///
	FixedStore(page_t* pages, size_t numberOfPages, bool writeable = true, bool free = true);
	
	// Desctructor
	virtual ~FixedStore();
	
	// Primitives of base class
	virtual bool isWriteable(uint32_t vaddr) const;
	virtual page_t getPageAddress(uint32_t vaddr);
	virtual void writeback(uint32_t vaddr, page_t page);
};

} // namespace VM
