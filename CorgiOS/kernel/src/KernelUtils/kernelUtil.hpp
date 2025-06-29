#pragma once
#include "../Graphics/CorgiGL.hpp"
#include "../Graphics/Fonts.hpp"
#include "../Graphics/GOP.hpp"
#include "../Memory/efiMemory.hpp"
#include "../Memory/memory.hpp"
#include "../Memory/corgiAlloc.hpp"
#include "../Memory/pageMapIndexer.hpp"
#include "../Memory/paging.hpp"
#include "../Memory/pageTableManager.hpp"
#include "../GDT/gdt.hpp"
#include "../Interrupts/idt.hpp"
#include "../Interrupts/interrupts.hpp"
#include "../InputOutput/io.hpp"
#include "../InputOutput/UserInput/mouse.hpp"
#include "../Graphics/Fluff/Fluff.hpp"
#include "../ACPI/acpi.hpp"
#include "../PCI/pci.hpp"

struct BootInfo {
    Framebuffer *framebuffer;
    PSF_FONT *font;
	EFI_MEMORY_DESCRIPTOR *mMap;
    uint64_t mMapSize;
    uint64_t mMapDescSize;
    ACPI::RSDP2 *rsdp;
};

struct KernelInfo {
    PageTableManager *pageTableManager;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

KernelInfo InitializeKernel(BootInfo *bootInfo);