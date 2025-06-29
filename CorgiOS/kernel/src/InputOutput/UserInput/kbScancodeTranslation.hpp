#pragma once
#include <stdint.h>

namespace QwertyUS {
    // Modifier keys
    #define LeftShift 0x2A
    #define RightShift 0x36
    #define Ctrl 0x1D
    #define Alt 0x38
    #define SuperKey 0x5B // Windows key (left)
    #define CapsLock 0x3A

    // Special keys
    #define Enter 0x1C
    #define BackSpace 0x0E
    #define Spacebar 0x39
    #define Tab 0x0F
    #define Escape 0x01

    // Function keys
    #define F1 0x3B
    #define F2 0x3C
    #define F3 0x3D
    #define F4 0x3E
    #define F5 0x3F
    #define F6 0x40
    #define F7 0x41
    #define F8 0x42
    #define F9 0x43
    #define F10 0x44
    #define F11 0x57
    #define F12 0x58

    // Navigation keys
    #define Insert 0x52
    #define Home 0x47
    #define PageUp 0x49
    #define Delete 0x53
    #define End 0x4F
    #define PageDown 0x51

    // Arrow keys
    #define ArrowUp 0x48
    #define ArrowDown 0x50
    #define ArrowLeft 0x4B
    #define ArrowRight 0x4D

    // Miscellaneous keys
    #define PrintScreen 0x37
    #define ScrollLock 0x46
    #define Pause 0x45
    #define FnKey 0x63 // Fn key (if present)

    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase, bool capsLockActive);
};