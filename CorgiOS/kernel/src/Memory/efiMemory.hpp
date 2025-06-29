#pragma once
#include <stdint.h>

struct EFI_MEMORY_DESCRIPTOR {
    uint32_t Type;
    void *PhysicalAddress;
    void *VirtualAddress;
    uint64_t NumberOfPages;
    uint64_t Attributes;
};

extern const char *EFI_MEMORY_TYPE_STRINGS[];