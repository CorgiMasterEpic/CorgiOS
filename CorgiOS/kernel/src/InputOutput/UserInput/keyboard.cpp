#include "keyboard.hpp"

bool isLeftShiftPressed;
bool isRightShiftPressed;
bool isCapsLockActive;

KeyboardPacket keyboardPacket;
void HandleKeyboard(uint8_t scancode) {
    keyboardPacket.ascii_key = ' ';
    keyboardPacket.special_key = NONE;

    bool isRelease = scancode & 0x80;
    uint8_t keyCode = scancode & 0x7F;
    keyboardPacket.rawStates[keyCode] = !isRelease;

    switch (scancode) {
        case LeftShift:
            isLeftShiftPressed = true;
            break;
        case LeftShift + 0x80:
            isLeftShiftPressed = false;
            break;
        case RightShift:
            isRightShiftPressed = true;
            break;
        case RightShift + 0x80:
            isRightShiftPressed = false;
            break;
        case CapsLock:
            isCapsLockActive = !isCapsLockActive; // Toggle Caps Lock
            break;
        case Ctrl:
            keyboardPacket.special_key = CTRL;
            break;
        case Alt:
            keyboardPacket.special_key = ALT;
            break;
        case SuperKey:
            keyboardPacket.special_key = SUPER;
            break;
        case Escape:
            keyboardPacket.special_key = ESC;
            break;
        case Enter:
            keyboardPacket.ascii_key = '\n';
            fluffAPI.setKeyboardPacket(keyboardPacket);
            break;
        case BackSpace:
            keyboardPacket.ascii_key = '\b';
            fluffAPI.setKeyboardPacket(keyboardPacket);
            break;
        case Tab:
            keyboardPacket.ascii_key = '\t';
            fluffAPI.setKeyboardPacket(keyboardPacket);
            break;
        case Spacebar:
            keyboardPacket.ascii_key = ' ';
            fluffAPI.setKeyboardPacket(keyboardPacket);
            break;
        case PrintScreen:
            keyboardPacket.special_key = PRTSC;
            break;
        case ScrollLock:
            keyboardPacket.special_key = SCRLK;
            break;
        case Pause:
            keyboardPacket.special_key = PAUSE;
            break;
        case Insert:
            keyboardPacket.special_key = INSERT;
            break;
        case Home:
            keyboardPacket.special_key = HOME;
            break;
        case PageUp:
            keyboardPacket.special_key = PGUP;
            break;
        case Delete:
            keyboardPacket.special_key = DEL;
            break;
        case End:
            keyboardPacket.special_key = END;
            break;
        case PageDown:
            keyboardPacket.special_key = PGDN;
            break;
        case ArrowUp:
            keyboardPacket.special_key = UP;
            break;
        case ArrowDown:
            keyboardPacket.special_key = DOWN;
            break;
        case ArrowLeft:
            keyboardPacket.special_key = LEFT;
            break;
        case ArrowRight:
            keyboardPacket.special_key = RIGHT;
            break;
        case FnKey:
            keyboardPacket.special_key = FN;
            break;
        case F1:
            keyboardPacket.special_key = FU1;
            break;
        case F2:
            keyboardPacket.special_key = FU2;
            break;
        case F3:
            keyboardPacket.special_key = FU3;
            break;
        case F4:
            keyboardPacket.special_key = FU4;
            break;
        case F5:
            keyboardPacket.special_key = FU5;
            break;
        case F6:
            keyboardPacket.special_key = FU6;
            break;
        case F7:
            keyboardPacket.special_key = FU7;
            break;
        case F8:
            keyboardPacket.special_key = FU8;
            break;
        case F9:
            keyboardPacket.special_key = FU9;
            break;
        case F10:
            keyboardPacket.special_key = FU10;
            break;
        case F11:
            keyboardPacket.special_key = FU11;
            break;
        case F12:
            keyboardPacket.special_key = FU12;
            break;
    }

    char ascii = QwertyUS::Translate(scancode, (isLeftShiftPressed || isRightShiftPressed), isCapsLockActive);

    if (ascii != 0) {
        keyboardPacket.ascii_key = ascii;
        fluffAPI.setKeyboardPacket(keyboardPacket);
        fluff.onInputKeyboard();
    }
}