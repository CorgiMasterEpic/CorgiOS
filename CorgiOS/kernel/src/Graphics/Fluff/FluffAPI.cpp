#include "FluffAPI.hpp"

FluffAPI fluffAPI;

void FluffAPI::setMousePacket(MousePacket mousePacket) {
    this->mousePacket = mousePacket;
}

MousePacket FluffAPI::getMousePacket() {
    return this->mousePacket;
}

void FluffAPI::setKeyboardPacket(KeyboardPacket keyboardPacket) {
    this->keyboardPacket = keyboardPacket;
}

KeyboardPacket FluffAPI::getKeyboardPacket() {
    return this->keyboardPacket;
}

