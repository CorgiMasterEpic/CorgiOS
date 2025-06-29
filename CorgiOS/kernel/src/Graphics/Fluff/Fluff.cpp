#include "Fluff.hpp"

Fluff fluff;

void Fluff::onStart() {
    corgiGL.ClearScreen(0xff003131); // Note to self: its ARGB not RGBA
    GUI::DrawTaskbar();
}

void Fluff::onUpdate() {
}

bool Fluff::isHotkeyComboFixed(bool ctrl, bool alt, bool shift, char ascii, KeyboardPacket keyboardPacket) {
    char asciiFixed = ascii;
    if (shift && ascii > 90) {
        asciiFixed -= 32;
    } else if (!shift && ascii < 97) {
        asciiFixed += 32;
    }
    return isHotkeyCombo(ctrl, alt, shift, ascii, keyboardPacket);
}

bool Fluff::isHotkeyCombo(bool ctrl, bool alt, bool shift, char ascii, KeyboardPacket keyboardPacket) {
    return (!ctrl || keyboardPacket.rawStates[Ctrl]) &&
           (!alt || keyboardPacket.rawStates[Alt]) &&
           (!shift || keyboardPacket.rawStates[LeftShift] || keyboardPacket.rawStates[RightShift]) &&
           (keyboardPacket.ascii_key == ascii);
}

void Fluff::onClick(unsigned int X, unsigned int Y, uint8_t Button) {
    switch (Button) {
        case 0: // Left click
            GUI::TryTaskbarClick(X, Y);
            break;
        
        default:
            break;
    }
}

void Fluff::onInputMouse() {
    MousePacket mousePacket = fluffAPI.getMousePacket();
    if (mousePacket.button1 && !oldMousePacket.button1) {
        onClick(mousePacket.x, mousePacket.y, 0);
    }
    ClearMouseCursor(oldMousePacket.x, oldMousePacket.y);
    DrawMouseCursor(mousePacket.x, mousePacket.y);
    oldMousePacket = mousePacket;
}

void Fluff::onInputKeyboard() {
    KeyboardPacket keyboardPacket = fluffAPI.getKeyboardPacket();
    corgiGL.printc(keyboardPacket.ascii_key);

    if (isHotkeyComboFixed(true, false, false, 's', keyboardPacket)) {
        corgiGL.println("Saving.");
    }
}