#include "CorgiGL.hpp"

CorgiGL corgiGL(nullptr, nullptr);

CorgiGL::CorgiGL(Framebuffer *framebuffer, PSF_FONT *font) {
    this->framebuffer = framebuffer;
    this->font = font;
    this->printPoint = {0, 0, 0xffffffff}; // Initialize printPoint with default values
}

void CorgiGL::DrawPixel(unsigned int x, unsigned int y, unsigned int color) {
    if (!framebuffer || x >= framebuffer->PixelsPerScanLine || y >= framebuffer->Height) {
        return; // Ensure framebuffer is valid and prevent out-of-bounds drawing
    }
    unsigned int *pixelPointer = (unsigned int *)framebuffer->BaseAddress;
    pixelPointer[x + (y * framebuffer->PixelsPerScanLine)] = color;
}

unsigned int CorgiGL::GetPixel(unsigned int x, unsigned int y) {
    if (!framebuffer || x >= framebuffer->PixelsPerScanLine || y >= framebuffer->Height) {
        return 0x00000000; // Ensure framebuffer is valid and prevent out-of-bounds access
    }
    unsigned int *pixelPointer = (unsigned int *)framebuffer->BaseAddress;
    return pixelPointer[x + (y * framebuffer->PixelsPerScanLine)];
}

void CorgiGL::DrawRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color) {
    if (!framebuffer) return; // Ensure framebuffer is valid
    unsigned int maxX = x + width;
    unsigned int maxY = y + height;
    for (unsigned int i = y; i < maxY; i++) {
        for (unsigned int j = x; j < maxX; j++) {
            DrawPixel(j, i, color);
        }
    }
}

void CorgiGL::DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color) {
    if (!framebuffer) return; // Ensure framebuffer is valid
    int dx = Math::abs((int)x2 - (int)x1), sx = x1 < x2 ? 1 : -1;
    int dy = Math::abs((int)y2 - (int)y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    while (true) {
        DrawPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}

void CorgiGL::DrawRectHollow(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color) {
    if (!framebuffer) return; // Ensure framebuffer is valid
    unsigned int maxX = x + width - 1;
    unsigned int maxY = y + height - 1;

    // Draw top and bottom edges
    for (unsigned int i = x; i <= maxX; i++) {
        DrawPixel(i, y, color);       // Top edge
        DrawPixel(i, maxY, color);   // Bottom edge
    }

    // Draw left and right edges
    for (unsigned int i = y; i <= maxY; i++) {
        DrawPixel(x, i, color);      // Left edge
        DrawPixel(maxX, i, color);  // Right edge
    }
}

void CorgiGL::ClearScreen(unsigned int color) {
    if (!framebuffer) return; // Ensure framebuffer is valid
    DrawRect(0, 0, framebuffer->Width, framebuffer->Height, color);
    setPrintPoint(0, 0, 0xffffffff); // Reset print point after clearing
}

void CorgiGL::printc(char c) {
    if (!framebuffer || !font) return; // Ensure framebuffer and font are valid

    if (printPoint.x + 8 > framebuffer->PixelsPerScanLine) {
        printPoint.x = 0; // Reset to start if beyond width
        printPoint.y += font->psf1_Header->charsize; // Move y position down for next line
    } if (printPoint.y > framebuffer->Height - font->psf1_Header->charsize) {
        return; // Prevent printing beyond framebuffer height
    }

    unsigned int *pixelPointer = (unsigned int *)framebuffer->BaseAddress;
    char *fontPointer = (char *)font->glyphBuffer + (c * font->psf1_Header->charsize);

    for (unsigned long y = printPoint.y; y < printPoint.y + font->psf1_Header->charsize; y++) {
        for (unsigned long x = printPoint.x; x < printPoint.x + 8; x++) {
            if ((*fontPointer & (0b10000000 >> (x - printPoint.x))) > 0) {
                pixelPointer[x + (y * framebuffer->PixelsPerScanLine)] = printPoint.color;
            }
        }
        fontPointer++;
    }
    printPoint.x += 8; // Move x position forward for next character
}

void CorgiGL::DrawCircle(unsigned int centerX, unsigned int centerY, unsigned int radius, unsigned int color) {
    if (!framebuffer) return;

    int x = 0;
    int y = radius;
    int d = 1 - (int)radius;

    auto drawCirclePoints = [&](int cx, int cy, int x, int y) {
        for (int dy = -y; dy <= y; ++dy) {
            DrawPixel(cx + x, cy + dy, color);
            DrawPixel(cx - x, cy + dy, color);
        }
        for (int dy = -x; dy <= x; ++dy) {
            DrawPixel(cx + y, cy + dy, color);
            DrawPixel(cx - y, cy + dy, color);
        }
    };

    drawCirclePoints(centerX, centerY, x, y);
    while (x < y) {
        x++;
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            y--;
            d += 2 * (x - y) + 1;
        }
        drawCirclePoints(centerX, centerY, x, y);
    }
}

void CorgiGL::DrawCircleHollow(unsigned int centerX, unsigned int centerY, unsigned int radius, unsigned int color) {
    if (!framebuffer) return;

    int x = 0;
    int y = radius;
    int d = 1 - (int)radius;

    auto drawCirclePoints = [&](int cx, int cy, int x, int y) {
        DrawPixel(cx + x, cy + y, color);
        DrawPixel(cx - x, cy + y, color);
        DrawPixel(cx + x, cy - y, color);
        DrawPixel(cx - x, cy - y, color);
        DrawPixel(cx + y, cy + x, color);
        DrawPixel(cx - y, cy + x, color);
        DrawPixel(cx + y, cy - x, color);
        DrawPixel(cx - y, cy - x, color);
    };

    drawCirclePoints(centerX, centerY, x, y);
    while (x < y) {
        x++;
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            y--;
            d += 2 * (x - y) + 1;
        }
        drawCirclePoints(centerX, centerY, x, y);
    }
}

void CorgiGL::DrawRoundedRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int radius, unsigned int color) {
    if (!framebuffer) return;

    if (radius * 2 > width || radius * 2 > height) {
        radius = (width < height ? width : height) / 2; // Clamp radius to fit inside rect
    }

    // Draw center rectangle (not including rounded corners)
    DrawRect(x + radius, y, width - 2 * radius, height, color);                // middle area
    DrawRect(x, y + radius, radius, height - 2 * radius, color);              // left bar
    DrawRect(x + width - radius, y + radius, radius, height - 2 * radius, color); // right bar

    // Draw rounded corners using quarter-circle method
    for (int dy = -radius; dy <= 0; dy++) {
        for (int dx = -radius; dx <= 0; dx++) {
            if (dx * dx + dy * dy <= radius * radius) {
                // Top-left
                DrawPixel(x + radius + dx, y + radius + dy, color);
                // Top-right
                DrawPixel(x + width - radius - dx - 1, y + radius + dy, color);
                // Bottom-left
                DrawPixel(x + radius + dx, y + height - radius - dy - 1, color);
                // Bottom-right
                DrawPixel(x + width - radius - dx - 1, y + height - radius - dy - 1, color);
            }
        }
    }
}

void CorgiGL::DrawTopRoundedRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int radius, unsigned int color) {
    if (!framebuffer) return;

    if (radius * 2 > width) {
        radius = width / 2;
    }
    if (radius > height) {
        radius = height;
    }

    // Draw main body
    DrawRect(x, y + radius, width, height - radius, color); // full-width body below rounded top

    // Draw top middle rectangle (between rounded corners)
    DrawRect(x + radius, y, width - 2 * radius, radius, color);

    // Draw top rounded corners
    for (int dy = -radius; dy <= 0; dy++) {
        for (int dx = -radius; dx <= 0; dx++) {
            if (dx * dx + dy * dy <= radius * radius) {
                // Top-left
                DrawPixel(x + radius + dx, y + radius + dy, color);
                // Top-right
                DrawPixel(x + width - radius - dx - 1, y + radius + dy, color);
            }
        }
    }
}

void CorgiGL::prints(const char *str) {
    if (!framebuffer || !font) return; // Ensure framebuffer and font are valid

    while (*str) {
        switch (*str) {
            case ' ':
                printPoint.x += 8; // Move x position forward by 8 pixels for space
                break;
            case '\n':
                printPoint.x = 0; // Reset x position
                printPoint.y += font->psf1_Header->charsize; // Move y position down for next line
                break;
            case '\r':
                printPoint.x = 0; // Reset x position
                break;
            case '\t':
                printPoint.x += 32; // Move x position forward by 32 pixels for tab
                break;
            default:
                printc(*str); // Print character
                break;
        }
        str++;
    }
}

void CorgiGL::println(const char *str) {
    prints(str);
    println(); // Move to the next line
}

void CorgiGL::println() {
    printPoint.x = 0; // Reset x position
    printPoint.y += font->psf1_Header->charsize; // Move y position down for next line
}

void CorgiGL::setPrintPoint(unsigned int x, unsigned int y, unsigned int color) {
    if (!framebuffer || !font) return; // Ensure framebuffer and font are valid
    printPoint = {x, y, color};
}

void CorgiGL::setPrintPoint(Point point) {
    if (!framebuffer || !font) return; // Ensure framebuffer and font are valid
    printPoint = point;
}