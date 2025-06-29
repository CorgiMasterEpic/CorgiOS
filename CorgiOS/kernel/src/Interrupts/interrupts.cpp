#include "interrupts.hpp"

extern PageTableManager pageTableManager;

__attribute__((interrupt)) void PageFault_handler(interrupt_frame *frame, uint64_t errorCode) {
    uint64_t faultingAddress;
    asm volatile("mov %%cr2, %0" : "=r"(faultingAddress)); // Get the faulting address from CR2

    // Attempt recovery if the page is not present
    bool present = errorCode & 0x1; // Page not present
    if (!present) {
        void *newPage = corgiAlloc.BorrowPage();
        if (newPage) {
            pageTableManager.MapMemory((void *)faultingAddress, newPage);
            return; // Recovery successful
        }
    }

    // If recovery fails, trigger a kernel panic
    char buffer[1024];
    Memory::strcpy(buffer, "Page Fault: Attempted to access an invalid or non-present page in memory.\n");
    Memory::strcat(buffer, "Faulting Address: ");
    Memory::strcat(buffer, Strings::toHexString(faultingAddress));

    PanicInfo panicInfo;
    panicInfo.Type = PanicType::PageFault;
    panicInfo.Message = buffer;
    Panic::Trigger(panicInfo);
}

__attribute__((interrupt)) void GPFault_handler(interrupt_frame *frame) {
    PanicInfo panicInfo;
    panicInfo.Type = PanicType::GeneralProtectionFault;
    panicInfo.Message = "General Protection Fault: A protection violation occurred.\n";
    Panic::Trigger(panicInfo);
}

__attribute__((interrupt)) void DoubleFault_handler(interrupt_frame *frame) {
    PanicInfo panicInfo;
    panicInfo.Type = PanicType::DoubleFault;
    panicInfo.Message = "Double Fault: A critical error occurred, and the CPU cannot recover.\n";
    Panic::Trigger(panicInfo);
}

__attribute__((interrupt)) void Keyboard_handler(interrupt_frame *frame) {
    uint8_t scancode = IO::inb(0x60);
    HandleKeyboard(scancode);
    PICStuff::PIC_EndMaster();
}

__attribute__((interrupt)) void Mouse_handler(interrupt_frame *frame) {
    uint8_t mouseData = IO::inb(0x60);
    HandleMouse(mouseData);
    PICStuff::PIC_EndSlave();
}

void PICStuff::PIC_EndMaster() {
    IO::outb(PIC1_COMMAND, PIC_EOI);
}

void PICStuff::PIC_EndSlave() {
    IO::outb(PIC2_COMMAND, PIC_EOI);
    IO::io_wait();
    IO::outb(PIC1_COMMAND, PIC_EOI);
}

void PICStuff::RemapPIC() {
    uint8_t a1, a2; 

    a1 = IO::inb(PIC1_DATA);
    IO::io_wait();
    a2 = IO::inb(PIC2_DATA);
    IO::io_wait();

    IO::outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    IO::io_wait();
    IO::outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    IO::io_wait();

    IO::outb(PIC1_DATA, 0x20);
    IO::io_wait();
    IO::outb(PIC2_DATA, 0x28);
    IO::io_wait();

    IO::outb(PIC1_DATA, 4);
    IO::io_wait();
    IO::outb(PIC2_DATA, 2);
    IO::io_wait();

    IO::outb(PIC1_DATA, ICW4_8086);
    IO::io_wait();
    IO::outb(PIC2_DATA, ICW4_8086);
    IO::io_wait();

    IO::outb(PIC1_DATA, a1);
    IO::io_wait();
    IO::outb(PIC2_DATA, a2);
}