#include "pci.hpp"

namespace PCI {
    void EnumerateFunction(uint64_t deviceAddress, uint64_t function) {
        uint64_t Offset = function << 12;

        uint64_t functionAddress = deviceAddress + Offset;
        pageTableManager.MapMemory((void*)functionAddress, (void*)functionAddress);

        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)functionAddress;

        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID == 0xffff) return;

        corgiGL.prints(GetVendorName(pciDeviceHeader->VendorID));
        corgiGL.prints(" / ");
        corgiGL.prints(GetDeviceName(pciDeviceHeader->VendorID, pciDeviceHeader->DeviceID));
        corgiGL.prints(" / ");
        corgiGL.prints(DeviceClasses[pciDeviceHeader->Class]);
        corgiGL.prints(" / ");
        corgiGL.prints(GetSubclassName(pciDeviceHeader->Class, pciDeviceHeader->Subclass));
        corgiGL.prints(" / ");
        corgiGL.println(GetProgIFName(pciDeviceHeader->Class, pciDeviceHeader->Subclass, pciDeviceHeader->ProgIF));
    }

    void EnumerateDevice(uint64_t busAddress, uint64_t device) {
        uint64_t Offset = device << 15;

        uint64_t deviceAddress = busAddress + Offset;
        pageTableManager.MapMemory((void*)deviceAddress, (void*)deviceAddress);

        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)deviceAddress;

        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID == 0xffff) return;

        for (uint64_t function = 0; function < 8; function++){
            EnumerateFunction(deviceAddress, function);
        }
    }

    void EnumerateBus(uint64_t BaseAddress, uint64_t Bus) {
        uint64_t Offset = Bus << 20;

        uint64_t BusAddress = BaseAddress + Offset;
        pageTableManager.MapMemory((void*)BusAddress, (void*)BusAddress);

        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)BusAddress;

        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID == 0xffff) return;

        for (uint64_t Device = 0; Device < 32; Device++){
            EnumerateDevice(BusAddress, Device);
        }
    }

    void EnumeratePCI(ACPI::MCFGHeader *MCFG) {
        int Entries = ((MCFG->Header.Length) - sizeof(ACPI::MCFGHeader)) / sizeof(ACPI::DeviceConfig);
        for (int t = 0; t < Entries; t++){
            ACPI::DeviceConfig* NewDeviceConfig = (ACPI::DeviceConfig*)((uint64_t)MCFG + sizeof(ACPI::MCFGHeader) + (sizeof(ACPI::DeviceConfig) * t));
            for (uint64_t Bus = NewDeviceConfig->StartBus; Bus < NewDeviceConfig->EndBus; Bus++){
                EnumerateBus(NewDeviceConfig->BaseAddress, Bus);
            }
        }
    }
}