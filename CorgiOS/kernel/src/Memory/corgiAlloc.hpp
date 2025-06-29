#pragma once
#include "efiMemory.hpp"
#include "memory.hpp"
#include <stdint.h>

class CorgiAlloc {
    public:
        Memory::Bitmap PageBitmap;
        void ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR *mMap, size_t mMapSize, size_t mMapDescSize);
        void FreePage(void *address);
        void FreePages(void *address, uint64_t pageCount);
        void LockPage(void *address);
        void LockPages(void *address, uint64_t pageCount);
        void *BorrowPage();

        // Getters
        uint64_t GetFreeRAM();
        uint64_t GetUsedRAM();
        uint64_t GetReservedRAM();

        // Aliases
        #define DropPage FreePage
        #define DropPages FreePages
        #define StealPage LockPage
        #define StealPages LockPages
        #define RequestPage BorrowPage // I don't use this one i prefer borrow page this is just here because.
    private:
        uint64_t pageBitmapIndex;
        void InitializeBitmap(size_t bitmapSize, void *bufferAddress);
        void ReservePage(void *address);
        void ReservePages(void *address, uint64_t pageCount);
        void UnreservePage(void *address);
        void UnreservePages(void *address, uint64_t pageCount);
};

extern CorgiAlloc corgiAlloc;