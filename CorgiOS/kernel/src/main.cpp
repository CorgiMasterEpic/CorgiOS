#include "KernelUtils/kernelUtil.hpp"
 
extern "C" void _start(BootInfo *bootInfo) {
	KernelInfo kernelInfo = InitializeKernel(bootInfo);

	fluff.onStart();

	while (true) {
		fluff.onUpdate();
	}

    while (true) { asm volatile ("hlt"); };
}