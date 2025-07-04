#pragma once
#include <stdint.h>

// Enum for page table flags
enum PT_Flag {
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63 // Only if supported
};

// Page directory entry structure
struct PageDirectoryEntry {
    uint64_t Value;

    void SetFlag(PT_Flag flag, bool enabled);
    bool GetFlag(PT_Flag flag);
    void SetAddress(uint64_t address);
    uint64_t GetAddress();
};

// Page table structure aligned to 4 KB
struct PageTable { 
    PageDirectoryEntry entries[512];
} __attribute__((aligned(0x1000)));