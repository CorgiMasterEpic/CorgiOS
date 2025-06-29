#pragma once
#include <stdint.h>
#include "../CorgiGL.hpp"

class GUI {
    public:
        static void DrawTaskbar();
        static void TryTaskbarClick(unsigned int x, unsigned int y);

    private:
        static const uint8_t taskbarHeight = 32;
        static void DrawTaskbarIcon(unsigned int x, unsigned int y);
};