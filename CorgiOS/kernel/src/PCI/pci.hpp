#pragma once
#include <stdint.h>
#include "../ACPI/acpi.hpp"
#include "../Memory/pageTableManager.hpp"
#include "../Graphics/CorgiGL.hpp"
#include "../Libs/Strings.hpp"

namespace PCI {
    struct PCIDeviceHeader{
        uint16_t VendorID;
        uint16_t DeviceID;
        uint16_t Command;
        uint16_t Status;
        uint8_t RevisionID;
        uint8_t ProgIF;
        uint8_t Subclass;
        uint8_t Class;
        uint8_t CacheLineSize;
        uint8_t LatencyTimer;
        uint8_t HeaderType;
        uint8_t BIST;
    };

    void EnumeratePCI(ACPI::MCFGHeader *MCFG);

    extern const char *GetVendorName(uint16_t VendorID);
    extern const char *GetDeviceName(uint16_t VendorID, uint16_t DeviceID);
    extern const char* GetSubclassName(uint8_t ClassCode, uint8_t SubclassCode);
    extern const char* GetProgIFName(uint8_t ClassCode, uint8_t SubclassCode, uint8_t ProgIF);

    extern const char *DeviceClasses[];
}