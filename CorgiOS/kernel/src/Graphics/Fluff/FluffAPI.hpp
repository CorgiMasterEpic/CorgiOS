#pragma once
#include <stdint.h>

enum SpecialKey {
    CTRL = 0,
    ALT = 1,
    SUPER = 2,
    ESC = 3,
    PRTSC = 4,
    SCRLK = 5,
    PAUSE = 6,
    INSERT = 7,
    HOME = 8,
    PGUP = 9,
    DEL = 10,
    END = 11,
    PGDN = 12,
    UP = 13,
    DOWN = 14,
    LEFT = 15,
    RIGHT = 16,
    FN = 17,
    FU1 = 18,
    FU2 = 19,
    FU3 = 20,
    FU4 = 21,
    FU5 = 22,
    FU6 = 23,
    FU7 = 24,
    FU8 = 25,
    FU9 = 26,
    FU10 = 27,
    FU11 = 28,
    FU12 = 29,
    NONE = 30,
};

struct MousePacket {
    unsigned int x;
    unsigned int y;
    bool button1;
    bool button2;
    bool button3;
};

struct KeyboardPacket {
    char ascii_key;
    SpecialKey special_key;
    bool rawStates[256] = { false };
};

class FluffAPI {
    public:
        bool mousePacketReady;
        void setMousePacket(MousePacket mousePacket);
        MousePacket getMousePacket();
        void setKeyboardPacket(KeyboardPacket mousePacket);
        KeyboardPacket getKeyboardPacket();
    
    private:
        MousePacket mousePacket;
        KeyboardPacket keyboardPacket;
};

extern FluffAPI fluffAPI;