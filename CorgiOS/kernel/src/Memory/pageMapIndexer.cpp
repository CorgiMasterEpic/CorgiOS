#include "pageMapIndexer.hpp"

PageMapIndexer::PageMapIndexer(uint64_t virtualAddress) {
    virtualAddress >>= 12;

    P_i = virtualAddress & 0x1ff; // Page Index
    virtualAddress >>= 9;

    PT_i = virtualAddress & 0x1ff; // Page Table Index
    virtualAddress >>= 9;

    PD_i = virtualAddress & 0x1ff; // Page Directory Index
    virtualAddress >>= 9;

    PDP_i = virtualAddress & 0x1ff; // Page Directory Pointer Index
    virtualAddress >>= 9;
}