#include "mouse.hpp"

void MouseWait() {
    uint32_t timeout = 100000;
    while (timeout--) {
        if ((IO::inb(0x64) & 0b10) == 0) {
            return;
        }
    }
}

void MouseWaitInput() {
    uint32_t timeout = 100000;
    while (timeout--) {
        if (IO::inb(0x64) & 0b1) {
            return;
        }
    }
}

void MouseWrite(uint8_t value) {
    MouseWait();
    IO::outb(0x64, 0xd4);
    MouseWait();
    IO::outb(0x60, value);
}

uint8_t MouseRead() {
    MouseWaitInput();
    return IO::inb(0x60);
}

void PreparePS2Mouse() {
    IO::outb(0x64, 0xA8); // Enable the auxiliary device - mouse

    MouseWait();
    IO::outb(0x64, 0x20); // Request the current status byte
    MouseWaitInput();
    uint8_t status = IO::inb(0x60);
    status |= 0b10; // Enable IRQ12 (mouse interrupt)
    MouseWait();
    IO::outb(0x64, 0x60); // Write the status byte
    MouseWait();
    IO::outb(0x60, status);

    MouseWrite(0xf4); // Enable the mouse
    MouseRead();
}

uint8_t MouseCycle = 0;
uint8_t RawMousePacket[4];
bool MousePacketReady = false;
void HandleMouse(uint8_t data) {
    ProcessMousePacket();
    if (MousePacketReady) return; // If the packet is already ready, ignore further data

    switch (MouseCycle) {
        case 0:
            if ((data & 0b00001000) == 0) { // Out-of-sync check
                MouseCycle = 0; // Reset cycle to re-sync
                return;
            }
            RawMousePacket[0] = data;
            MouseCycle++;
            break;

        case 1:
            RawMousePacket[1] = data;
            MouseCycle++;
            break;

        case 2:
            RawMousePacket[2] = data;
            MousePacketReady = true; // Mark the packet as ready
            MouseCycle = 0;          // Reset cycle for the next packet
            break;
        default:
            MouseCycle = 0; // Reset cycle if something goes wrong
            break;
    }
}

MousePosition mousePosition;
void ProcessMousePacket() {
    if (!MousePacketReady) return;

    bool XNegative = RawMousePacket[0] & PS2XSign;
    bool YNegative = RawMousePacket[0] & PS2YSign;
    bool XOverflow = RawMousePacket[0] & PS2XOverflow;
    bool YOverflow = RawMousePacket[0] & PS2YOverflow;

    if (!XNegative) {
        mousePosition.X += RawMousePacket[1];
        if (XOverflow) {
            mousePosition.X += 255;
        }
    } else {
        RawMousePacket[1] = 256 - RawMousePacket[1];
        mousePosition.X -= RawMousePacket[1];
        if (XOverflow) {
            mousePosition.X -= 255;
        }
    }

    if (!YNegative) {
        mousePosition.Y -= RawMousePacket[2];
        if (YOverflow) {
            mousePosition.Y -= 255;
        }
    } else {
        RawMousePacket[2] = 256 - RawMousePacket[2];
        mousePosition.Y += RawMousePacket[2];
        if (YOverflow) {
            mousePosition.Y += 255;
        }
    }

    if (mousePosition.X < 0) mousePosition.X = 0;
    if (mousePosition.X > corgiGL.getFramebuffer()->PixelsPerScanLine - 1) mousePosition.X = corgiGL.getFramebuffer()->PixelsPerScanLine - 1;

    if (mousePosition.Y < 0) mousePosition.Y = 0;
    if (mousePosition.Y > corgiGL.getFramebuffer()->Height - 1) mousePosition.Y = corgiGL.getFramebuffer()->Height - 1;

    MousePacket mousePacket;
    mousePacket.x = mousePosition.X;
    mousePacket.y = mousePosition.Y;
    mousePacket.button1 = RawMousePacket[0] & PS2Leftbutton;
    mousePacket.button2 = RawMousePacket[0] & PS2Rightbutton;
    mousePacket.button3 = RawMousePacket[0] & PS2Middlebutton;

    fluffAPI.setMousePacket(mousePacket);
    fluff.onInputMouse();

    MousePacketReady = false;
}