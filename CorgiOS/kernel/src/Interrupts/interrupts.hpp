#pragma once
#include "../KernelUtils/panic.hpp"
#include "../Memory/pageTableManager.hpp"
#include "../Memory/corgiAlloc.hpp"
#include "../InputOutput/io.hpp"
#include "../InputOutput/UserInput/keyboard.hpp"
#include "../InputOutput/UserInput/mouse.hpp"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define PIC_EOI      0x20

#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

struct interrupt_frame;

__attribute__((interrupt)) void PageFault_handler(interrupt_frame *frame, uint64_t errorCode);
__attribute__((interrupt)) void GPFault_handler(interrupt_frame *frame);
__attribute__((interrupt)) void DoubleFault_handler(interrupt_frame *frame);
__attribute__((interrupt)) void Keyboard_handler(interrupt_frame *frame);
__attribute__((interrupt)) void Mouse_handler(interrupt_frame *frame);

class PICStuff {
    public:
        static void RemapPIC();
        static void PIC_EndMaster();
        static void PIC_EndSlave();
};