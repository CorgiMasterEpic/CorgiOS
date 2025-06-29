#include "kernelUtil.hpp"

KernelInfo kernelInfo;
void PrepareMemory(BootInfo *bootInfo) {
    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

	corgiAlloc = CorgiAlloc();
	corgiAlloc.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t kernelPages = (uint64_t)(kernelSize / 4096 + 1);

	corgiAlloc.StealPages(&_KernelStart, kernelPages);

	PageTable *PML4 = (PageTable*)corgiAlloc.BorrowPage();
	Memory::memset(PML4, 0, 0x1000);

	pageTableManager = PageTableManager(PML4);

	for (uint64_t i = 0; i < Memory::GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); i += 0x1000) {
		pageTableManager.MapMemory((void*)i, (void*)i);
	}

	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;

	for (uint64_t i = fbBase; i < fbBase + fbSize; i += 0x1000) {
		pageTableManager.MapMemory((void*)i, (void*)i);
	}

	asm ("mov %0, %%cr3" : : "r" (PML4));

    kernelInfo.pageTableManager = &pageTableManager;
}

void PrepareGraphics(BootInfo *bootInfo) { // Very sad at the moment but hopefully it will come in use later...
    corgiGL = CorgiGL(bootInfo->framebuffer, bootInfo->font);
    Memory::memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize);
}

IDTR idtr;
void SetIDTGate(void *handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector) {
	IDTDescEntry *interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
	interrupt->SetOffset((uint64_t)handler);
	interrupt->type_attr = type_attr;
	interrupt->selector = selector;
}

void PrepareInterrupts() {
	idtr.Limit = 0x0fff;
	idtr.Offset = (uint64_t)corgiAlloc.BorrowPage();

	SetIDTGate((void*)PageFault_handler, 0xe, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)GPFault_handler, 0xd, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)DoubleFault_handler, 0x8, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)Keyboard_handler, 0x21, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)Mouse_handler, 0x2c, IDT_TA_InterruptGate, 0x08);

	asm ("lidt %0" : : "m" (idtr));

	PICStuff::RemapPIC();
}

void DisablePS2Devices() {
    // Disable both PS/2 ports
    IO::outb(0x64, 0xAD); // Disable second PS/2 port
    IO::outb(0x64, 0xAE); // Disable first PS/2 port

    // Flush the output buffer
    while (IO::inb(0x64) & 0x01) {
        IO::inb(0x60); // Read and discard any data in the output buffer
    }
}

void EnablePS2Devices() {
    // Enable both PS/2 ports
    IO::outb(0x64, 0xAE); // Enable first PS/2 port
    IO::outb(0x64, 0xAD); // Enable second PS/2 port

    // Reset keyboard
    IO::outb(0x60, 0xFF); // Send reset command to the keyboard
    while (IO::inb(0x64) & 0x01) {
        uint8_t response = IO::inb(0x60);
        if (response == 0xFA || response == 0xAA) {
            break; // Acknowledge or self-test passed
        }
    }

    // Reset mouse
    IO::outb(0x64, 0xD4); // Send command to second PS/2 port
    IO::outb(0x60, 0xFF); // Reset mouse
    while (IO::inb(0x64) & 0x01) {
        uint8_t response = IO::inb(0x60);
        if (response == 0xFA || response == 0xAA) {
            break; // Acknowledge or self-test passed
        }
    }

    // Enable keyboard and mouse
    IO::outb(0x60, 0xF4); // Enable keyboard
    IO::outb(0x64, 0xD4); // Send command to second PS/2 port
    IO::outb(0x60, 0xF4); // Enable mouse
}

void PrepareACPI(BootInfo *bootInfo) {
    ACPI::SDTHeader* XSDT = (ACPI::SDTHeader*)(bootInfo->rsdp->XSDTAddress);
    ACPI::MCFGHeader* MCFG = (ACPI::MCFGHeader*)ACPI::FindTable(XSDT, (char*)"MCFG");

    PCI::EnumeratePCI(MCFG);
}

KernelInfo InitializeKernel(BootInfo *bootInfo) {
	GDTDescriptor gdtDescriptor;
	gdtDescriptor.Size = sizeof(GDT) - 1;
	gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
	LoadGDT(&gdtDescriptor);

    PrepareMemory(bootInfo);
    PrepareGraphics(bootInfo);
    PrepareACPI(bootInfo);
	DisablePS2Devices();
	PrepareInterrupts();
	PreparePS2Mouse();
	EnablePS2Devices();

	IO::outb(PIC1_DATA, 0b11111001);
    IO::outb(PIC2_DATA, 0b11101111);

    asm ("sti");

    return kernelInfo;
}