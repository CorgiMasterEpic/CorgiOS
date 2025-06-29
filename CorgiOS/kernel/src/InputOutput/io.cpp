#include "io.hpp"

void IO::outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t IO::inb(uint16_t port) {
    uint8_t returnValue;
    asm volatile ("inb %1, %0" : "=a"(returnValue) : "Nd"(port));
    return returnValue;
}

void IO::outw(uint16_t port, uint16_t value) {
    asm volatile ("outw %0, %1" : : "a"(value), "Nd"(port)); // Output a word (16 bits)
}

uint16_t IO::inw(uint16_t port) {
    uint16_t returnValue;
    asm volatile ("inw %1, %0" : "=a"(returnValue) : "Nd"(port)); // Input a word (16 bits)
    return returnValue;
}

void IO::io_wait() {
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}