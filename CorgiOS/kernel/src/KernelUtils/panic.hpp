#pragma once
#include "../Graphics/CorgiGL.hpp"
#include "../Libs/Strings.hpp"
#include "../Memory/memory.hpp"

enum PanicType {
    PageFault = 0,
    GeneralProtectionFault = 1,
    DoubleFault = 2,
};

struct PanicInfo {
    const char *Message;
    PanicType Type;
};

class Panic {
    public:
        static void Trigger(PanicInfo info);
        //void EnterRecoveryShell(); // A Basic shell to save files before making the system: 0xDEADBEEF
    private:
        static const char *PanicTypeToString(PanicType type);
};