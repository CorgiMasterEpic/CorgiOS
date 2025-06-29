#include "RDRand.hpp"

bool RDRand::IsSupported() {
    uint32_t eax, ebx, ecx, edx;
    eax = 1; // CPUID function 1: Processor Info and Feature Bits

    asm volatile("cpuid"
                 : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                 : "a"(eax), "c"(0));

    // Check if the RDRAND bit (bit 30 of ECX) is set
    return (ecx & (1 << 30)) != 0;
}

bool RDRand::GetRandomNumber(uint64_t &randomNumber) {
    uint8_t success;
    asm volatile("rdrand %0; setc %1"
                 : "=r"(randomNumber), "=qm"(success)
                 :
                 : "cc");

    return success != 0; // Return true if RDRAND succeeded
}