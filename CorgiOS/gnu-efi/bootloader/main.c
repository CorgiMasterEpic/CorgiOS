#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <stddef.h>

// TODO: Add a bootloader menu to select the os to boot if one is present load it automatically and if none are present show boot error and finally if the user has selected one for more then 5 seconds then boot it automatically
// TODO: Add more comments to the code
// TODO: Add a command line interface to the bootloader to allow messing with partitions, loading kernels, creating/deleting: files and folders, a very simple text editor, and other things

// Bootloader metadata
const CHAR16 *BOOTLOADER_NAME = L"Corgiloader";
const CHAR16 *BOOTLOADER_CREATOR = L"CorgiMasterEpic";
const CHAR16 *BOOTLOADER_VERSION = L"25.0"; // Format: year.version // year = last two digits of the year, version = 0 to 9

typedef struct {
	void *BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
} Framebuffer;

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF_HEADER;

typedef struct {
	PSF_HEADER *psf1_Header;
	void *glyphBuffer;
} PSF_FONT;

typedef struct {
    Framebuffer *framebuffer;
    PSF_FONT *font;
    EFI_MEMORY_DESCRIPTOR *mMap;
    UINTN mMapSize;
    UINTN mMapDescSize;
    void *rsdp;
} BootInfo;

// Function to load a file from the EFI file system
EFI_FILE *LoadFile(EFI_FILE *Directory, CHAR16 *Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	if (Path == NULL || Path[0] == L'\0') return NULL; // Prevent path injection :D

    EFI_FILE *LoadedFile;

    // Get the loaded image protocol
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

    // Get the file system protocol
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

    // Open the root directory if no directory is specified
    if (Directory == NULL) {
        FileSystem->OpenVolume(FileSystem, &Directory);
    }

    // Attempt to open the specified file
    EFI_STATUS Status = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    return (Status == EFI_SUCCESS) ? LoadedFile : NULL;
}

PSF_FONT *LoadFont(EFI_FILE *Directory, CHAR16 *Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_FILE *font = LoadFile(Directory, Path, ImageHandle, SystemTable);
    if (font == NULL) {
        Print(L"> Status: ");
        ST->ConOut->SetAttribute(ST->ConOut, EFI_YELLOW | EFI_BACKGROUND_BLACK);
        Print(L"Failed to load font file '%s'.\n", Path);
        ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
        return NULL;
    }

    PSF_HEADER *fontHeader;
    SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF_HEADER), (void**)&fontHeader);
    UINTN size = sizeof(PSF_HEADER);
    font->Read(font, &size, fontHeader);

    if (fontHeader->magic[0] != 0x36 || fontHeader->magic[1] != 0x04) {
        Print(L"> Status: ");
        ST->ConOut->SetAttribute(ST->ConOut, EFI_YELLOW | EFI_BACKGROUND_BLACK);
        Print(L"Invalid PSF font file '%s'.\n", Path);
        ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
        SystemTable->BootServices->FreePool(fontHeader);
        font->Close(font);
        return NULL;
    }

    UINTN glyphBufferSize = fontHeader->charsize * 256;
    if (fontHeader->mode == 1) {
        glyphBufferSize = fontHeader->charsize * 512;
    }

    void *glyphBuffer;
    {
        font->SetPosition(font, sizeof(PSF_HEADER));
        SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
        font->Read(font, &glyphBufferSize, glyphBuffer);
    }

    PSF_FONT *psfFont;
    SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF_FONT), (void**)&psfFont);
    psfFont->psf1_Header = fontHeader;
    psfFont->glyphBuffer = glyphBuffer;
    return psfFont;
}

// Simple implementation of memcmp for comparing memory regions
int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = s1, *p2 = s2;
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return (p1[i] < p2[i]) ? -1 : 1;
        }
    }
    return 0;
}

// Function to initialize UEFI Graphics Output Protocol
Framebuffer framebuffer;
Framebuffer *InitializeGraphicsOutputProtocol() {
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;

    if (EFI_ERROR(uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&GOP))) {
        Print(L"> Status: ");
        ST->ConOut->SetAttribute(ST->ConOut, EFI_YELLOW | EFI_BACKGROUND_BLACK);
        Print(L"Failed to locate Graphics Output Protocol.\n");
        ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
        return NULL;
    } else {
        Print(L"> Status: ");
        ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
        Print(L"Graphics Output Protocol initialized successfully.\n");
        ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    }

    framebuffer.BaseAddress = (void*)GOP->Mode->FrameBufferBase;
    framebuffer.BufferSize = GOP->Mode->FrameBufferSize;
    framebuffer.Width = GOP->Mode->Info->HorizontalResolution;
    framebuffer.Height = GOP->Mode->Info->VerticalResolution;
    framebuffer.PixelsPerScanLine = GOP->Mode->Info->PixelsPerScanLine;

    return &framebuffer;
}

// Function to print the bootloader logo and metadata
void PrintLogo() {
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    Print(L"┌────────────────────────────────────────┐\n│");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTCYAN | EFI_BACKGROUND_BLACK);
    Print(L" * ");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    Print(L"%s ", BOOTLOADER_NAME);
    ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);
    Print(L"v%s ", BOOTLOADER_VERSION);
    ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTMAGENTA | EFI_BACKGROUND_BLACK);
    Print(L"by %s ", BOOTLOADER_CREATOR);
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    Print(L"│\n");
    Print(L"└────────────────────────────────────────┘\n");
}

UINTN strcmp(CHAR8 *a, CHAR8 *b, UINTN length) {
    for (UINTN i = 0; i < length; i++) {
        if (*a != *b) {
            return 0;
        }
    }
    return 1;
}

// Main entry point for the EFI application
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    // Initialize the EFI library
    InitializeLib(ImageHandle, SystemTable);

    // Print the bootloader logo
    PrintLogo();
    Print(L"> Status: ");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
    Print(L"Bootloader started successfully.\n");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);

    // Load the kernel file
    EFI_FILE *Kernel = LoadFile(NULL, L"CorgiOS.elf", ImageHandle, SystemTable);
    if (Kernel == NULL) {
        Print(L"> Status: ");
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED | EFI_BACKGROUND_BLACK);
        Print(L"Failed to load 'CorgiOS.elf'.\n");
        ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
        return EFI_LOAD_ERROR;
    }

    Print(L"> Status: ");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
    Print(L"'CorgiOS.elf' loaded successfully.\n");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);

    // Read the ELF header
    Elf64_Ehdr header;
    {
        UINTN FileInfoSize;
        EFI_FILE_INFO *FileInfo;
        Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL);
        SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
        Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

        UINTN size = sizeof(header);
        Kernel->Read(Kernel, &size, &header);
    }

    // Validate the ELF header
    if (
        memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 || // Check ELF magic number
        header.e_ident[EI_CLASS] != ELFCLASS64 ||                // Ensure 64-bit ELF
        header.e_ident[EI_DATA] != ELFDATA2LSB ||                // Ensure little-endian
        header.e_type != ET_EXEC ||                              // Ensure executable type
        header.e_machine != EM_X86_64 ||                         // Ensure x86_64 architecture
        header.e_version != EV_CURRENT                           // Ensure current ELF version
    ) {
        Print(L"> Status: ");
        ST->ConOut->SetAttribute(ST->ConOut, EFI_YELLOW | EFI_BACKGROUND_BLACK);
        Print(L"Invalid ELF format for 'CorgiOS.elf'.\n");
        ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
        return EFI_LOAD_ERROR;
    }

    Print(L"> Status: ");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
    Print(L"Valid ELF format detected.\n");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);

    // Load the program headers
    Elf64_Phdr *phdrs;
    {
        Kernel->SetPosition(Kernel, header.e_phoff);
        UINTN size = header.e_phnum * header.e_phentsize;
        SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
        Kernel->Read(Kernel, &size, phdrs);
    }

    // Load segments into memory
    for (
        Elf64_Phdr *phdr = phdrs;
        (char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
        phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
    ) {
        if (phdr->p_type == PT_LOAD) {
            UINTN pages = (phdr->p_memsz + 0xFFF) / 0x1000; // Align to page size
            Elf64_Addr segment = phdr->p_paddr;
            SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

            Kernel->SetPosition(Kernel, phdr->p_offset);
            UINTN size = phdr->p_filesz;
            Kernel->Read(Kernel, &size, (void*)segment);
        }
    }

    Print(L"> Status: ");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
    Print(L"'CorgiOS.elf' loaded into memory.\n");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);

    Framebuffer *frameBuffer = InitializeGraphicsOutputProtocol();
    PSF_FONT *newFont = LoadFont(NULL, L"zap-light16.psf", ImageHandle, SystemTable);
    Print(L"> Status: ");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
    Print(L"UEFI Graphics Output Protocol & PSF1 Font Info:\n");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTCYAN | EFI_BACKGROUND_BLACK);
    Print(L"  -> Base: 0x%x\n  -> Size: 0x%x\n  -> Resolution: %dx%d\n  -> Font char size: %d\n", 
        frameBuffer->BaseAddress,
        frameBuffer->BufferSize,
        frameBuffer->Width,
        frameBuffer->Height,
        newFont->psf1_Header->charsize);
    if (frameBuffer->PixelsPerScanLine != frameBuffer->Width) {
        Print(L"  -> PixelsPerScanLine: %d\n", frameBuffer->PixelsPerScanLine);
    }

    EFI_MEMORY_DESCRIPTOR *Map = NULL;
    UINTN MapSize = 0, MapKey = 0;
    UINTN DescriptorSize = 0;
    UINT32 DescriptorVersion = 0;
    {
        SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
        SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&Map);
        SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
    }

    EFI_CONFIGURATION_TABLE *configTable = SystemTable->ConfigurationTable;
    void *rsdp = NULL;
    EFI_GUID Acpi2TableGuid = ACPI_20_TABLE_GUID;

    for (UINTN i = 0; i < SystemTable->NumberOfTableEntries; i++) {
        if (CompareGuid(&configTable[i].VendorGuid, &Acpi2TableGuid)) {
            if (strcmp((CHAR8*)"RSD PTR ", (CHAR8*)configTable->VendorTable, 8)) {
                rsdp = (void*)configTable->VendorTable;
                //break; // remove this to fix weird GP Fault bug!
            }
        }
        configTable++;
    }

    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    
    BootInfo bootInfo;
    bootInfo.framebuffer = frameBuffer;
    bootInfo.font = newFont;
    bootInfo.mMap = Map;
    bootInfo.mMapSize = MapSize;
    bootInfo.mMapDescSize = DescriptorSize;
    bootInfo.rsdp = rsdp;

    Print(L"> Status: ");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
    Print(L"BootInfo structure initialized successfully.\n");
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);

    // Jump to the kernel's entry point
    void (*KernelStart)(BootInfo *) = ((__attribute__((sysv_abi)) void (*)(BootInfo *)) header.e_entry);

    SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

    // Call the kernel's entry point with the boot information
    KernelStart(&bootInfo);

    // Should never reach here
    return EFI_SUCCESS;
}