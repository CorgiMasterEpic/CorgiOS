#pragma once

#include <stdint.h>

class RDRand {
    public:
        static bool IsSupported();
        static bool GetRandomNumber(uint64_t &randomNumber);
};