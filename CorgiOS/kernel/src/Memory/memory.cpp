#include "memory.hpp"

uint64_t Memory::GetMemorySize(EFI_MEMORY_DESCRIPTOR *mMap, uint64_t mMapEntries, uint64_t mMapDescSize) {
    static uint64_t totalMemorySize = 0;

    if (totalMemorySize > 0) {
        return totalMemorySize;
    } else {
        for (uint64_t i = 0; i < mMapEntries; i++) {
            EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + i * mMapDescSize);
            totalMemorySize += desc->NumberOfPages * 4096; // 4096 bytes per page
        }
        return totalMemorySize;
    }
}

void Memory::memset(void *start, uint8_t value, uint64_t num) {
    uint8_t *ptr = (uint8_t *)start;
    for (uint64_t i = 0; i < num; i++) {
        ptr[i] = value;
    }
}

void Memory::memset32(void *start, uint32_t value, uint64_t num) {
    uint32_t *ptr = (uint32_t*)start;
    for (uint64_t i = 0; i < num; i++) {
        ptr[i] = value;
    }
}

void Memory::memset64(void *start, uint64_t value, uint64_t num) {
    uint64_t *ptr = (uint64_t *)start;
    for (uint64_t i = 0; i < num; i++) {
        ptr[i] = value;
    }
}

int Memory::memcmp(const void *ptr1, const void *ptr2, uint64_t num) {
    const uint8_t *bytePtr1 = (const uint8_t *)ptr1;
    const uint8_t *bytePtr2 = (const uint8_t *)ptr2;

    for (uint64_t i = 0; i < num; i++) {
        if (bytePtr1[i] != bytePtr2[i]) {
            return bytePtr1[i] - bytePtr2[i];
        }
    }

    return 0; // Memory blocks are equal
}

void Memory::strcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0'; // Null-terminate the destination string
}

void Memory::strcat(char *dest, const char *src) {
    while (*dest) {
        dest++; // Move to the end of the destination string
    }
    while (*src) {
        *dest++ = *src++; // Append the source string
    }
    *dest = '\0'; // Null-terminate the destination string
}

bool Memory::Bitmap::operator[](uint64_t index) {
    return Get(index);
}

bool Memory::Bitmap::Get(uint64_t index) {
    if (index >= Size * 8) {
        return false;
    } else {
        uint64_t byteIndex = index / 8;
        uint8_t bitIndex = index % 8;
        uint8_t bitIndexer = 0b10000000 >> bitIndex;
        if ((Buffer[byteIndex] & bitIndexer) > 0){
            return true;
        } else {
            return false;
        }
    }
}

bool Memory::Bitmap::Set(uint64_t index, bool value) {
    if (index >= Size * 8) {
        return false;
    } else {
        uint64_t byteIndex = index / 8;
        uint8_t bitIndex = index % 8;
        uint8_t bitIndexer = 0b10000000 >> bitIndex;
        Buffer[byteIndex] &= ~bitIndexer;
        if (value){
            Buffer[byteIndex] |= bitIndexer;
        }

        return true;
    }
}