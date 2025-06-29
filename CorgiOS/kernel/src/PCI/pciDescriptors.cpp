#include <stdint.h>
#include "../Libs/Strings.hpp"

namespace PCI {
    const char* DeviceClasses[] {
        "Unclassified (Undefined fluff -- basically useless)",
        "Mass Storage Controller (Holds all the treats!)",
        "Network Controller (Magic box that sends barkmail)",
        "Display Controller (Shows bones and moving squirrels)",
        "Multimedia Controller (Plays kibble crunch sounds)",
        "Memory Controller (Remembers where the stick was buried... last month)",
        "Bridge Device (Connects the tail to the snoot -- metaphorically)",
        "Simple Communication Controller (Zoomies across rooms enabled)",
        "Base System Peripheral (Basic good-boy core component)",
        "Input Device Controller (For chewing on keyboards and mice)",
        "Docking Station (Nap station... maybe full of treats?)",
        "Processor (The big brain of the operation)",
        "Serial Bus Controller (Boops one thing at a time, no rush)",
        "Wireless Controller (No leash required -- wireless zoomies!)",
        "Intelligent Controller (Artificial intelligence, but fluffier)",
        "Satellite Communication Controller (Talks to space -- spooky magic)",
        "Encryption Controller (Shreds secrets into unreadable fluff)",
        "Signal Processing Controller (Sniffs out invisible frequencies)",
        "Processing Accelerator (Extra zoomies module installed)",
        "Non-Essential Instrumentation (Totally optional fluff -- but cute)"
    };

    const char *GetVendorName(uint16_t VendorID) {
        switch (VendorID) {
            case 0x8086: return "Intel Corporation";
            case 0x1022: return "AMD Corporation";
            case 0x10DE: return "NVIDIA Corporation";
            case 0x10EC: return "Realtek Semiconductor Corp.";
            case 0x14E4: return "Broadcom Inc.";
            case 0x168C: return "Qualcomm Atheros";
            case 0x1969: return "Atheros Communications";
            case 0x104C: return "Texas Instruments";
            case 0x1106: return "VIA Technologies";
            case 0x103C: return "Hewlett-Packard (HP)";
            case 0x17AA: return "Lenovo";
            case 0x19E5: return "HiSilicon (Huawei)";
            case 0x1AF4: return "Red Hat, Inc.";
            case 0x15AD: return "VMware Inc.";
            case 0x1234: return "QEMU Virtual Device";
            case 0x1AB8: return "Parallels, Inc.";
            case 0x1B36: return "QEMU Virtual Machine";
            case 0x1A03: return "ASPEED Technology, Inc.";
            case 0x1C5C: return "TUXEDO Computers";
            case 0x13B5: return "ARM Ltd.";
            case 0x1057: return "Motorola Semiconductor";
            case 0x16C3: return "Synopsys, Inc.";
            case 0x1D17: return "Framework Computer, Inc.";
            case 0x144D: return "Samsung Electronics Co Ltd";
            case 0x1E4B: return "Fuzhou Rockchip Electronics Co.";
            case 0x1AE0: return "Apple Inc. (M1/M2 Silicon)";
            case 0x1CC5: return "PINE64 (Pine Microsystems)";
            default:
                return Strings::toHexString(VendorID); // fallback: show raw hex
        }
    }

    const char* GetDeviceName(uint16_t VendorID, uint16_t DeviceID) {
        switch (VendorID) {
            case 0x8086: // Intel
                switch (DeviceID) {
                    case 0x29C0: return "Intel Express DRAM Controller";
                    case 0x2918: return "Intel LPC Interface Controller";
                    case 0x2922: return "Intel 6-port SATA AHCI Controller";
                    case 0x2930: return "Intel SMBus Controller";
                    case 0xA382: return "Intel 400-series SATA AHCI Controller";
                    default: return Strings::toHexString(DeviceID);
                }
    
            case 0x1022: // AMD
                switch (DeviceID) {
                    case 0x145D: return "AMD Renoir USB Controller";
                    case 0x15E0: return "AMD Ryzen SATA Controller";
                    case 0x73BF: return "AMD Radeon RX 6800 XT";
                    case 0x73DF: return "AMD Radeon RX 6900 XT";
                    case 0x744C: return "AMD Radeon RX 7600";
                    case 0x73FF: return "AMD Radeon RX 6700 XT";
                    default: return Strings::toHexString(DeviceID);
                }
    
            case 0x10DE: // NVIDIA
                switch (DeviceID) {
                    // Older GPUs
                    case 0x1B80: return "NVIDIA GeForce GTX 1080";
                    case 0x1B81: return "NVIDIA GTX 1080 (Variant)";
                    case 0x1C82: return "NVIDIA GeForce GTX 1660 Super";
                    case 0x1C86: return "NVIDIA GeForce GTX 1660 Ti";
    
                    // RTX 20 Series
                    case 0x1E04: return "NVIDIA GeForce RTX 2060";
                    case 0x1F08: return "NVIDIA GeForce RTX 2060 Super";
                    case 0x1E87: return "NVIDIA GeForce RTX 2070";
                    case 0x1F02: return "NVIDIA GeForce RTX 2070 Super";
                    case 0x1E82: return "NVIDIA GeForce RTX 2080";
                    case 0x1E81: return "NVIDIA GeForce RTX 2080 Ti";
    
                    // RTX 30 Series
                    case 0x2238: return "RTX 3060 Laptop GPU";
                    case 0x2504: return "RTX 3060 Desktop GPU";
                    case 0x2520: return "RTX 3060 Ti Desktop GPU";
                    case 0x2508: return "RTX 3070 Desktop GPU";
                    case 0x2560: return "RTX 3070 Ti Desktop GPU";
                    case 0x2484: return "RTX 3080 Desktop GPU";
                    case 0x2490: return "RTX 3080 Ti Desktop GPU";
                    case 0x2566: return "RTX 3090 Desktop GPU";
                    case 0x25A8: return "RTX 3090 Ti Desktop GPU";
    
                    // RTX 40 Series
                    case 0x2810: return "RTX 4070 Laptop GPU";
                    case 0x2806: return "RTX 4070 Desktop GPU";
                    case 0x2813: return "RTX 4070 Ti Desktop GPU";
                    case 0x2834: return "RTX 4080 Desktop GPU";
                    case 0x2838: return "RTX 4080 Laptop GPU";
                    case 0x2940: return "RTX 4090 Desktop GPU";
    
                    default: return Strings::toHexString(DeviceID);
                }
    
            case 0x10EC: // Realtek
                switch (DeviceID) {
                    case 0x8168: return "Realtek RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller (rev 15)";
                    case 0xC821: return "Realtek RTL8821CE 802.11ac PCIe Wireless Network Adapter";
                    case 0x8136: return "Realtek RTL810xE PCI Express Fast Ethernet controller";
                    default: return Strings::toHexString(DeviceID);
                }
            
            case 0x14E4: // Broadcom
                switch (DeviceID) {
                    case 0x43A0: return "Broadcom BCM4360 802.11ac Wireless Network Adapter";
                    default: return Strings::toHexString(DeviceID);
                }
    
            case 0x1A03: // ASPEED
                switch (DeviceID) {
                    case 0x2000: return "ASPEED Graphics Family";
                    default: return Strings::toHexString(DeviceID);
                }
    
            case 0x1234: // QEMU/Bochs Virtual
                switch (DeviceID) {
                    case 0x1111: return "QEMU VGA Compatible Controller";
                    case 0x1112: return "QEMU SVGA Extended Adapter";
                    default: return Strings::toHexString(DeviceID);
                }
    
            case 0x1AF4: // Red Hat / VirtIO
                switch (DeviceID) {
                    case 0x1000: return "Virtio PCI Host Device";
                    case 0x1001: return "Virtio Network Controller";
                    case 0x1002: return "Virtio Block Device";
                    case 0x1003: return "Virtio Console";
                    case 0x1040: return "Virtio GPU Device";
                    default: return Strings::toHexString(DeviceID);
                }
    
            default:
                return Strings::toHexString(DeviceID);
        }
    }

    const char* MassStorageControllerSubclassName(uint8_t SubclassCode) {
        switch (SubclassCode) {
            case 0x00:
                return "SCSI Bus Controller (Stick retrieval specialist)";
            case 0x01:
                return "IDE Controller (Old school pupper)";
            case 0x02:
                return "Floppy Disk Controller (Vintage chew toy)";
            case 0x03:
                return "IPI Bus Controller (Invisible leash handler)";
            case 0x04:
                return "RAID Controller (Guard dog squad)";
            case 0x05:
                return "ATA Controller (Fetch command center)";
            case 0x06:
                return "Serial ATA (Synchronized tail wagging)";
            case 0x07:
                return "Serial Attached SCSI (Extra snoot-powered fetch)";
            case 0x08:
                return "Non-Volatile Memory Controller (Memory of a good boy)";
            case 0x80:
                return "Other (Mystery pupper)";
        }
        return Strings::toHexString(SubclassCode);
    }

    const char* SerialBusControllerSubclassName(uint8_t SubclassCode) {
        switch (SubclassCode) {
            case 0x00:
                return "FireWire (IEEE 1394) Controller (Super speedy zoomies)";
            case 0x01:
                return "ACCESS Bus (Secret puppy handshake)";
            case 0x02:
                return "SSA (Sniff sniff approval)";
            case 0x03:
                return "USB Controller (Snack delivery pipeline)";
            case 0x04:
                return "Fibre Channel (Laser pointer chaser)";
            case 0x05:
                return "SMBus (Synchronized meal barking)";
            case 0x06:
                return "Infiniband (Endless zoomies connection)";
            case 0x07:
                return "IPMI Interface (Pupper health check)";
            case 0x08:
                return "SERCOS Interface (IEC 61491) (Corgi dance coordinator)";
            case 0x09:
                return "CANbus (Woof code translator)";
            case 0x80:
                return "SerialBusController - Other (Secret squirrel mode)";
        }
        return Strings::toHexString(SubclassCode);
    }

    const char* BridgeDeviceSubclassName(uint8_t SubclassCode) {
        switch (SubclassCode) {
            case 0x00:
                return "Host Bridge (The pupper's bridge to the world)";
            case 0x01:
                return "ISA Bridge (Ancient tail wagging relic)";
            case 0x02:
                return "EISA Bridge (Extended zoomies access)";
            case 0x03:
                return "MCA Bridge (Mystical corgi artifact)";
            case 0x04:
                return "PCI-to-PCI Bridge (Pupper teleportation gateway)";
            case 0x05:
                return "PCMCIA Bridge (Pupper card trick)";
            case 0x06:
                return "NuBus Bridge (The new bone exchange)";
            case 0x07:
                return "CardBus Bridge (Treat card reader)";
            case 0x08:
                return "RACEway Bridge (Zoomies race track)";
            case 0x09:
                return "PCI-to-PCI Bridge (Tail wagging repeater)";
            case 0x0a:
                return "InfiniBand-to-PCI Host Bridge (Infinite zoomies conduit)";
            case 0x80:
                return "Other (Unexpected pupper bridge)";
        }
        return Strings::toHexString(SubclassCode);
    }

    const char* GetSubclassName(uint8_t ClassCode, uint8_t SubclassCode) {
        switch (ClassCode) {
            case 0x01:
                return MassStorageControllerSubclassName(SubclassCode);
            case 0x03:
                switch (SubclassCode) {
                    case 0x00:
                        return "VGA Compatible Controller (Bone display handler)";
                }
            case 0x06:
                return BridgeDeviceSubclassName(SubclassCode);
            case 0x0C:
                return SerialBusControllerSubclassName(SubclassCode);
        }
        return Strings::toHexString(SubclassCode);
    }

    const char* GetProgIFName(uint8_t ClassCode, uint8_t SubclassCode, uint8_t ProgIF) {
        switch (ClassCode) {
            case 0x01:
                switch (SubclassCode) {
                    case 0x06:
                        switch (ProgIF) {
                            case 0x00:
                                return "Vendor Specific Interface (Secret pupper handshake)";
                            case 0x01:
                                return "AHCI 1.0 (Stick-fetching protocol v1)";
                            case 0x02:
                                return "Serial Storage Bus (One bone at a time)";
                        }
                }
            case 0x03:
                switch (SubclassCode) {
                    case 0x00:
                        switch (ProgIF) {
                            case 0x00:
                                return "VGA Controller (Squirrel watcher)";
                            case 0x01:
                                return "8514-Compatible Controller (Old pupper display)";
                        }
                }
            case 0x0C:
                switch (SubclassCode) {
                    case 0x03:
                        switch (ProgIF) {
                            case 0x00:
                                return "UHCI Controller (Tiny pupper paws USB)";
                            case 0x10:
                                return "OHCI Controller (Old pupper paws USB)";
                            case 0x20:
                                return "EHCI (USB2) Controller (Zoomy pupper USB)";
                            case 0x30:
                                return "XHCI (USB3) Controller (Super zoomy pupper USB)";
                            case 0x80:
                                return "Unspecified (Mystery pupper USB)";
                            case 0xFE:
                                return "USB Device (Not a Host Controller) (Good pupper gadget)";
                        }
                }    
        }
        return Strings::toHexString(ProgIF);
    }
}
