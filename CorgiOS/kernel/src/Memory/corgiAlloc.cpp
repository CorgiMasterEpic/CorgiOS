#include "corgiAlloc.hpp"

uint64_t freeMemory = 0;
uint64_t reservedMemory = 0;
uint64_t usedMemory = 0;
bool initialized = false;
CorgiAlloc corgiAlloc;

void CorgiAlloc::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR *mMap, size_t mMapSize, size_t mMapDescSize) {
    if (initialized) {
        return;
    } else {
        initialized = true;
    }

    void *largestFreeMemorySegment = nullptr;
    size_t largestFreeMemorySegmentSize = 0;

    uint64_t mMapEntries = mMapSize / mMapDescSize;
    for (uint64_t i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + i * mMapDescSize);
        if (desc->Type == 7) {
            if (desc->NumberOfPages * 4096 > largestFreeMemorySegmentSize) {
                largestFreeMemorySegment = desc->PhysicalAddress;
                largestFreeMemorySegmentSize = desc->NumberOfPages * 4096;
            }
        }
    }

    uint64_t totalMemorySize = Memory::GetMemorySize(mMap, mMapEntries, mMapDescSize);
    freeMemory = totalMemorySize;
    uint64_t bitmapSize = totalMemorySize / 4096 / 8 + 1; // +1 for rounding up

    InitializeBitmap(bitmapSize, largestFreeMemorySegment);

    ReservePages(0, totalMemorySize / 4096 + 1);
    for (uint64_t i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + i * mMapDescSize);
        if (desc->Type == 7) {
            UnreservePages(desc->PhysicalAddress, desc->NumberOfPages);
        }
    }
    
    ReservePages(0, 0x100);
    LockPages(PageBitmap.Buffer, PageBitmap.Size / 4096 + 1); // Lock the bitmap pages
}

void CorgiAlloc::InitializeBitmap(size_t bitmapSize, void *bufferAddress) {
    PageBitmap.Size = bitmapSize;
    PageBitmap.Buffer = (uint8_t*)bufferAddress;

    for (uint64_t i = 0; i < bitmapSize; i++) {
        *(uint8_t*)(PageBitmap.Buffer + i) = 0;
    }
}

void CorgiAlloc::FreePage(void *address) {
    uint64_t pageIndex = (uint64_t)address / 4096;
    if (PageBitmap[pageIndex] == false) {
        return; // Page is already free
    } else {
        if (PageBitmap.Set(pageIndex, false)) {
            freeMemory += 4096;
            usedMemory -= 4096;
            if (pageBitmapIndex > pageIndex) {
                pageBitmapIndex = pageIndex;
            }
        }
    }
}

void CorgiAlloc::LockPage(void *address) {
    uint64_t pageIndex = (uint64_t)address / 4096;
    if (PageBitmap[pageIndex] == true) {
        return; // Page is already locked
    } else {
        if (PageBitmap.Set(pageIndex, true)) {
            freeMemory -= 4096;
            usedMemory += 4096;
        }
    }
}

void CorgiAlloc::UnreservePage(void *address) {
    uint64_t pageIndex = (uint64_t)address / 4096;
    if (PageBitmap[pageIndex] == false) {
        return; // Page is already unreserved
    } else {
        if (PageBitmap.Set(pageIndex, false)) {
            freeMemory += 4096;
            reservedMemory -= 4096;
            if (pageBitmapIndex > pageIndex) {
                pageBitmapIndex = pageIndex;
            }
        }
    }
}

void CorgiAlloc::ReservePage(void *address) {
    uint64_t pageIndex = (uint64_t)address / 4096;
    if (PageBitmap[pageIndex] == true) {
        return; // Page is already reserved
    } else {
        if (PageBitmap.Set(pageIndex, true)) {
            freeMemory -= 4096;
            reservedMemory += 4096;
        }
    }
}

void CorgiAlloc::FreePages(void *address, uint64_t pageCount) {
    for (uint64_t i = 0; i < pageCount; i++) {
        FreePage((void*)((uint64_t)address + i * 4096));
    }
}

void CorgiAlloc::LockPages(void *address, uint64_t pageCount) {
    for (uint64_t i = 0; i < pageCount; i++) {
        LockPage((void*)((uint64_t)address + i * 4096));
    }
}

void CorgiAlloc::UnreservePages(void *address, uint64_t pageCount) {
    for (uint64_t i = 0; i < pageCount; i++) {
        UnreservePage((void*)((uint64_t)address + i * 4096));
    }
}

void CorgiAlloc::ReservePages(void *address, uint64_t pageCount) {
    for (uint64_t i = 0; i < pageCount; i++) {
        ReservePage((void*)((uint64_t)address + i * 4096));
    }
}

void *CorgiAlloc::BorrowPage() { // Basiclly request page but i like using the term borrow
    for (; pageBitmapIndex < PageBitmap.Size * 8; pageBitmapIndex++) {
        if (PageBitmap[pageBitmapIndex] == false) {
            LockPage((void*)(pageBitmapIndex * 4096));
            return (void*)(pageBitmapIndex * 4096);
        } else {
            continue; // Page is already locked
        }
    }
    return nullptr; // No free pages available - TO BE IMPLEMENTED: Swap to disk or handle out of memory killer "Smarter then most mainstream OSes"

    /*
        OOM Killer Implementation:
        - Check if the system is running low on memory (e.g., freeMemory < threshold). like less then ~20 mb remaining, BUT ONLY IF WE NEED IT.
            Stage 1: Identify what processes we can kill to free up memory:
                - Check if the process is 100% idle (not using CPU) for atleast 10 secounds if so, we can kill it.
                - Check if the process is not being used like wifi drivers (not being used by any application) if so, we can kill it.
                - Check if the process is not more then like 90% crititcal to the system, like a service that reports errors or something like that if so, we can kill it.
                - If all else fails, we can kill the actually important processes like gpu drivers, but only as a last resort.
            Stage 2: Swap and absalutly last resorts.
                - Swap to disk yes its slow but its better then nothing.
                - If swap fails, we delete temporary files and caches to free up memory and other stuff.
                - If that fails, we can try to kill very important processes like certain features like like CorgiGL Backwords Compatiblity Services and other backwords compatibleity stuff. // Confused? CorgiGL Backwards Compatiblity Services has not been implemented yet, not until CorgiGL 2 is released.
                - Finally, time to kill the actual process that was causeing this issue, the process using the most memory, and then reopen all the other stuff we killed.
            Stage 3: The worst of the worst.
                - Go to console mode, and kill everything that it can without crashing the system.
                - If all else fails, we kernel panic with the error message - "OMM Killer is throwing a tantrum, your out of memory.\nConsider upgrading your pc?"

        Swap implementation:
            - Normal swap, but its dynamic, so it can grow and shrink as needed.
     */
}

uint64_t CorgiAlloc::GetFreeRAM() {
    return freeMemory;
}

uint64_t CorgiAlloc::GetUsedRAM() {
    return usedMemory;
}

uint64_t CorgiAlloc::GetReservedRAM() {
    return reservedMemory;
}