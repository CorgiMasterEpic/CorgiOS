#pragma once
#include "../../InputOutput/UserInput/mouse.hpp"
#include "../../InputOutput/UserInput/keyboard.hpp"
#include "FluffAPI.hpp"
#include "Cursor.hpp"
#include "GUI.hpp"
#include "../../Libs/Math.hpp"

class Fluff {
    public:
        void onStart();
        void onUpdate();
        void onInputMouse();
        void onInputKeyboard();
        void onClick(unsigned int X, unsigned int Y, uint8_t Button);

    private:
        MousePacket oldMousePacket;
        bool isHotkeyComboFixed(bool ctrl, bool alt, bool shift, char ascii, KeyboardPacket keyboardPacket);
        bool isHotkeyCombo(bool ctrl, bool alt, bool shift, char ascii, KeyboardPacket keyboardPacket);
};

extern Fluff fluff;