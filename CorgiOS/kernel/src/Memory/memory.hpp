#pragma once
#include <stdint.h>
#include <stddef.h>
#include "efiMemory.hpp"

class Memory {
    public:
        static uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR *mMap, uint64_t mMapEntries, uint64_t mMapDescSize);
        static void memset(void *start, uint8_t value, uint64_t num);
        static void memset32(void *start, uint32_t value, uint64_t num);
        static void memset64(void *start, uint64_t value, uint64_t num);
        static int memcmp(const void *ptr1, const void *ptr2, uint64_t num);
        static void strcpy(char *dest, const char *src);
        static void strcat(char *dest, const char *src);
        class Bitmap {
            public:
                size_t Size;
                uint8_t *Buffer;
                bool operator[](uint64_t index);
                bool Set(uint64_t index, bool value);
                bool Get(uint64_t index);
        };
};