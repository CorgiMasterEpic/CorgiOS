#include "panic.hpp"

void Panic::Trigger(PanicInfo panicInfo) {
    corgiGL.ClearScreen(0xff00000f);
    constexpr uint32_t BrightColor = 0xfffff0f0;

    const auto* font = corgiGL.getFont();
    const unsigned fontHeight = font->psf1_Header->charsize;
    constexpr unsigned FONT_WIDTH = 8; // << Correct width of characters
    const unsigned screenWidth = corgiGL.getFramebuffer()->PixelsPerScanLine;

    // Correct X centering using actual character width
    auto centerX = [&](const char* text) -> unsigned {
        unsigned len = Strings::getStringLength(text);
        return (screenWidth - (len * FONT_WIDTH)) / 2;
    };

    // Manual Y control
    unsigned y = 20;

    auto printlnCentered = [&](const char* text) {
        corgiGL.setPrintPoint(centerX(text), y, BrightColor);
        corgiGL.println(text);
        y += fontHeight + 2;
    };

    // === Header ===
    printlnCentered("!=== Corgi OS Kernel Panic ===!");
    printlnCentered("CorgiOS has encountered a kernel panic and cannot continue. (CorgiOS is splooting.)");
    y += fontHeight;

    // === FAQ ===
    printlnCentered("Kernel Panic FAQ:");
    printlnCentered("  Q: What just happened?");
    printlnCentered("  A: The kernel hit an unrecoverable condition (see info below).");
    y += fontHeight;

    printlnCentered("  Q: What caused this?");
    printlnCentered("  A:");
    printlnCentered("    - Faulty, Overheating, or unsupported hardware.");
    printlnCentered("    - Invalid memory access.");
    printlnCentered("    - A kernel bug or bad driver.");
    printlnCentered("    - Cosmic rays (we wish we were kidding).");
    printlnCentered("    - CorgiOS just needed a sploot break.");
    y += fontHeight;

    printlnCentered("  Q: What can I do?");
    printlnCentered("  A:");
    printlnCentered("    - Reboot your system.");
    printlnCentered("    - Report this issue if it keeps happening.");
    printlnCentered("    - Fix faulty hardware or wait for it to cooldown then reboot.");
    printlnCentered("    - (Last Resort). Reinstall CorgiOS... or install somthing else.");
    printlnCentered("    - Try not to panic. (The OS already did.)");
    y += fontHeight;

    // === Nerd Info ===
    printlnCentered("Info for nerds:");

    {
        const char* typeStr = PanicTypeToString(panicInfo.Type);
        char buffer[128];
        Memory::strcpy(buffer, "  Panic Type: ");
        Memory::strcat(buffer, typeStr);
        printlnCentered(buffer);
    }

    y += fontHeight;

    // === Panic Message ===
    printlnCentered("Panic Message:");
    if (panicInfo.Message && panicInfo.Message[0]) {
        const char* info = panicInfo.Message;
    
        // Break lines manually if needed
        char line[128];
        int i = 0;
        for (int j = 0; ; ++j) {
            if (info[j] == '\n' || info[j] == '\0') {
                line[i] = '\0';
                printlnCentered(line);
                i = 0;
                if (info[j] == '\0') break;
            } else if (i < 127) {
                line[i++] = info[j];
            }
        }
    } else {
        printlnCentered("  (no additional info)");
    }    

    while (true) {
        asm volatile("hlt");
    }
}

const char* Panic::PanicTypeToString(PanicType type) {
    switch (type) {
        case PageFault: return "Page Fault";
        case GeneralProtectionFault: return "General Protection Fault";
        case DoubleFault: return "Double Fault";
        default: return "Unknown Panic Type";
    }
}
