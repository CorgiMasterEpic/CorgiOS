#include "Cursor.hpp"

bool cursorInitialized = false;

uint16_t cursorImage[] {
    0b1111111111111111,
    0b1000000000000010,
    0b1000000000000100,
    0b1000000000001000,
    0b1000000000010000,
    0b1000000000100000,
    0b1000000001000000,
    0b1000000010000000,
    0b1000000100000000,
    0b1000001000000000,
    0b1000010000000000,
    0b1000100000000000,
    0b1001000000000000,
    0b1010000000000000,
    0b1100000000000000,
    0b1000000000000000,
};

uint16_t cursorImageFill[] {
    0b0000000000000000,
    0b0111111111111100,
    0b0111111111111000,
    0b0111111111110000,
    0b0111111111100000,
    0b0111111111000000,
    0b0111111110000000,
    0b0111111100000000,
    0b0111111000000000,
    0b0111110000000000,
    0b0111100000000000,
    0b0111000000000000,
    0b0110000000000000,
    0b0100000000000000,
    0b0000000000000000,
    0b0000000000000000,
};

unsigned int savedPixels[16][16]; // Fixed-size array to store the original pixels

void SavePixelsBehindCursor(unsigned int x, unsigned int y) {
    for (unsigned int yOff = 0; yOff < 16; yOff++) {
        for (unsigned int xOff = 0; xOff < 16; xOff++) {
            unsigned int drawX = x + xOff;
            unsigned int drawY = y + yOff;

            if (drawX < corgiGL.getFramebuffer()->PixelsPerScanLine &&
                drawY < corgiGL.getFramebuffer()->Height) {
                savedPixels[yOff][xOff] = corgiGL.GetPixel(drawX, drawY);
            }
        }
    }
}

void RestorePixelsBehindCursor(unsigned int x, unsigned int y) {
    for (unsigned int yOff = 0; yOff < 16; yOff++) {
        for (unsigned int xOff = 0; xOff < 16; xOff++) {
            unsigned int drawX = x + xOff;
            unsigned int drawY = y + yOff;

            if (drawX < corgiGL.getFramebuffer()->PixelsPerScanLine &&
                drawY < corgiGL.getFramebuffer()->Height) {
                corgiGL.DrawPixel(drawX, drawY, savedPixels[yOff][xOff]);
            }
        }
    }
}


void DrawMouseCursor(unsigned int x, unsigned int y) {
    // Clamp top-left (0,0) corner only
    if ((int)x < 0) x = 0;
    if ((int)y < 0) y = 0;

    SavePixelsBehindCursor(x, y);

    for (unsigned int yOff = 0; yOff < 16; yOff++) {
        for (unsigned int xOff = 0; xOff < 16; xOff++) {
            unsigned int drawX = x + xOff;
            unsigned int drawY = y + yOff;

            if (drawX >= corgiGL.getFramebuffer()->PixelsPerScanLine ||
                drawY >= corgiGL.getFramebuffer()->Height)
                continue;

            // Draw outline
            if ((cursorImage[yOff] & (1 << (15 - xOff)))) {
                corgiGL.DrawPixel(drawX, drawY, 0xff000000);
            }

            // Draw fill
            if (cursorImageFill[yOff] & (1 << (15 - xOff))) {
                corgiGL.DrawPixel(drawX, drawY, 0xffffffff);
            }
        }
    }
}

void ClearMouseCursor(unsigned int x, unsigned int y) {
    if (!cursorInitialized) {
        cursorInitialized = true;
        return;
    }

    if ((int)x < 0) x = 0;
    if ((int)y < 0) y = 0;

    RestorePixelsBehindCursor(x, y);
}
