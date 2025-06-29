#pragma once
#include "GOP.hpp"
#include "Fonts.hpp"
#include "../Libs/Math.hpp"

struct Point {
    unsigned int x, y, color;
};

class CorgiGL {
    public:
        CorgiGL(Framebuffer *framebuffer, PSF_FONT *font);
        void printc(char c);
        void prints(const char *str);
        void println();
        void println(const char *str);
        void DrawPixel(unsigned int x, unsigned int y, unsigned int color);
        unsigned int GetPixel(unsigned int x, unsigned int y);
        void DrawRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color);
        void DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);
        void DrawRectHollow(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color);
        void DrawCircle(unsigned int centerX, unsigned int centerY, unsigned int radius, unsigned int color);
        void DrawCircleHollow(unsigned int centerX, unsigned int centerY, unsigned int radius, unsigned int color);
        void DrawRoundedRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int radius, unsigned int color);
        void DrawTopRoundedRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int radius, unsigned int color);
        void ClearScreen(unsigned int color);
        void setPrintPoint(unsigned int x, unsigned int y, unsigned int color);
        void setPrintPoint(Point point);

        // Getters
        Point getPrintPoint() const {
            return printPoint;
        }
        Framebuffer *getFramebuffer() const {
            return framebuffer;
        }
        PSF_FONT *getFont() const {
            return font;
        }
    private:
        Framebuffer *framebuffer;
        PSF_FONT *font;
        Point printPoint;
};

extern CorgiGL corgiGL;