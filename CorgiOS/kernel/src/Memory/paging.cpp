#include "paging.hpp"

// Set or clear a flag in the page directory entry
void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled) {
    uint64_t bitSelector = (uint64_t)1 << flag;
    Value &= ~bitSelector; // Clear the flag
    if (enabled) {
        Value |= bitSelector; // Set the flag
    }
}

// Get the status of a flag in the page directory entry
bool PageDirectoryEntry::GetFlag(PT_Flag flag) {
    uint64_t bitSelector = (uint64_t)1 << flag;
    return (Value & bitSelector) > 0 ? true : false; // Explicit parentheses for clarity
}

// Get the physical address stored in the page directory entry
uint64_t PageDirectoryEntry::GetAddress() {
    return (Value & 0x000ffffffffff000) >> 12; // Mask and shift to get the address
}

// Set the physical address in the page directory entry
void PageDirectoryEntry::SetAddress(uint64_t address) {
    address &= 0x000000ffffffffff; // Mask to ensure only the lower 48 bits are used
    Value &= 0xfff0000000000fff;   // Clear the address bits
    Value |= (address << 12);      // Set the address bits
}