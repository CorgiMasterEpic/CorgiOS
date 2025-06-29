#pragma once
#include <stdint.h>

class Strings {
    public:
        static const char *toString(uint64_t value);
        static const char *toString(int64_t value);
        static const char *toString(double value, uint8_t decimalPlaces = 2);
        static const char *toString(bool value);
        static const char *toHexString(uint64_t value, bool uppercase = false);
        static uint64_t getStringLength(const char *str);
        static bool isStringEqual(const char *str1, const char *str2);
};