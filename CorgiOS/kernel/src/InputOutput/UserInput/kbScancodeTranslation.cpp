#include "kbScancodeTranslation.hpp"

namespace QwertyUS {
    const char ASCIITable[] = {
        0 ,  0 , '1', '2', // 0x00 - 0x03
        '3', '4', '5', '6', // 0x04 - 0x07
        '7', '8', '9', '0', // 0x08 - 0x0B
        '-', '=',  0 ,  0 , // 0x0C - 0x0F
        'q', 'w', 'e', 'r', // 0x10 - 0x13
        't', 'y', 'u', 'i', // 0x14 - 0x17
        'o', 'p', '[', ']', // 0x18 - 0x1B
         0 ,  0 , 'a', 's', // 0x1C - 0x1F
        'd', 'f', 'g', 'h', // 0x20 - 0x23
        'j', 'k', 'l', ';', // 0x24 - 0x27
        '\'', '`',  0 , '\\', // 0x28 - 0x2B
        'z', 'x', 'c', 'v', // 0x2C - 0x2F
        'b', 'n', 'm', ',', // 0x30 - 0x33
        '.', '/',  0 , '*', // 0x34 - 0x37
         0 , ' '             // 0x38 - 0x39
    };

    const char ShiftedASCIITable[] = {
        0 ,  0 , '!', '@', // 0x00 - 0x03
        '#', '$', '%', '^', // 0x04 - 0x07
        '&', '*', '(', ')', // 0x08 - 0x0B
        '_', '+',  0 ,  0 , // 0x0C - 0x0F
        'Q', 'W', 'E', 'R', // 0x10 - 0x13
        'T', 'Y', 'U', 'I', // 0x14 - 0x17
        'O', 'P', '{', '}', // 0x18 - 0x1B
         0 ,  0 , 'A', 'S', // 0x1C - 0x1F
        'D', 'F', 'G', 'H', // 0x20 - 0x23
        'J', 'K', 'L', ':', // 0x24 - 0x27
        '"', '~',  0 , '|', // 0x28 - 0x2B
        'Z', 'X', 'C', 'V', // 0x2C - 0x2F
        'B', 'N', 'M', '<', // 0x30 - 0x33
        '>', '?',  0 , '*', // 0x34 - 0x37
         0 , ' '             // 0x38 - 0x39
    };

    char Translate(uint8_t scancode, bool shiftPressed, bool capsLockActive) {
        if (scancode > 58) return 0; // Invalid scancode

        char key = shiftPressed ? ShiftedASCIITable[scancode] : ASCIITable[scancode];

        // Caps Lock only affects alphabetic characters
        if (capsLockActive && key >= 'a' && key <= 'z') {
            key -= 32; // Convert to uppercase
        } else if (capsLockActive && key >= 'A' && key <= 'Z') {
            key += 32; // Convert to lowercase
        }

        return key;
    }
}