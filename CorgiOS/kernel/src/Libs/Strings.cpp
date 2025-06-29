#include "Strings.hpp"

const char *Strings::toString(uint64_t value) {
    static char toStringBuffer[128];

    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0){
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while(value / 10 > 0){
        uint8_t remainder = value % 10;
        value /= 10;
        toStringBuffer[size - index] = remainder + '0';
        index++;
    }

    uint8_t remainder = value % 10;
    toStringBuffer[size - index] = remainder + '0';
    toStringBuffer[size + 1] = 0; 
    return toStringBuffer;
}

const char *Strings::toString(int64_t value) {
    static char toStringBuffer[128];

    uint8_t isNegative = 0;

    if (value < 0){
        isNegative = 1;
        value *= -1;
        toStringBuffer[0] = '-';
    }

    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0){
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while(value / 10 > 0){
        uint8_t remainder = value % 10;
        value /= 10;
        toStringBuffer[isNegative + size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    toStringBuffer[isNegative + size - index] = remainder + '0';
    toStringBuffer[isNegative + size + 1] = 0; 
    return toStringBuffer;
}

const char *Strings::toString(double value, uint8_t decimalPlaces) {
    static char toStringBuffer[128];

    if (decimalPlaces > 20) decimalPlaces = 20;

    char* intPointer = (char*)toString((int64_t)value);
    char* doublePointer = toStringBuffer;

    if (value < 0){
        value *= -1;
    }

    while(*intPointer != 0){
        *doublePointer = *intPointer;
        intPointer++;
        doublePointer++;
    }

    *doublePointer = '.';
    doublePointer++;

    double newValue = value - (int)value;

    for (uint8_t i = 0; i < decimalPlaces; i++){
        newValue *= 10;
        *doublePointer = (int)newValue + '0';
        newValue -= (int)newValue;
        doublePointer++;
    }

    *doublePointer = 0;
    return toStringBuffer;
}

const char *Strings::toHexString(uint64_t value, bool uppercase) {
    static char hexBuffer[20 + 1] = {0}; // 2 for "0x", 16 for hex, 1 for null
    const char *hexChars = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

    hexBuffer[0] = '0';
    hexBuffer[1] = 'x';

    // Start inserting from the end (backward)
    int index = 18; // last hex digit goes here (before null terminator)
    if (value == 0) {
        hexBuffer[2] = '0';
        hexBuffer[3] = '\0';
        return hexBuffer;
    }

    while (value > 0 && index >= 2) {
        hexBuffer[index--] = hexChars[value % 16];
        value /= 16;
    }

    // Shift hex digits to position after "0x"
    int start = 2;
    int copyFrom = index + 1;
    while (copyFrom <= 18) {
        hexBuffer[start++] = hexBuffer[copyFrom++];
    }

    hexBuffer[start] = '\0';
    return hexBuffer;
}


const char *Strings::toString(bool value) {
    if (value) {
        return "true";
    } else if (value == false) {
        return "false";
    } else {
        return "null";
    }
}

uint64_t Strings::getStringLength(const char *str) {
    uint64_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

bool Strings::isStringEqual(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *str1 == *str2;
}