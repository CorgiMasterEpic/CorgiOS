#include "GUI.hpp"

void GUI::DrawTaskbarIcon(unsigned int x, unsigned int y) {
    corgiGL.DrawRect(x, y, taskbarHeight, taskbarHeight, 0xff3c3c3c); // Draw filled icon
    corgiGL.DrawRectHollow(x, y, taskbarHeight, taskbarHeight, 0xffffffff); // Draw hollow border

    // Calculate the center position for the '=' character
    unsigned int centerX = x + (taskbarHeight / 2) - 4; // Center horizontally
    unsigned int centerY = y + (taskbarHeight / 2) - (corgiGL.getFont()->psf1_Header->charsize / 2); // Center vertically

    Point OldPoint = corgiGL.getPrintPoint(); // Save the old print point
    corgiGL.setPrintPoint(centerX, centerY, 0xff121212); // Set print point to the center
    corgiGL.printc('='); // Print the '=' character
    corgiGL.setPrintPoint(OldPoint); // Restore the old print point
}

void GUI::DrawTaskbar() {
    corgiGL.DrawRect(0, corgiGL.getFramebuffer()->Height - taskbarHeight, corgiGL.getFramebuffer()->Width, taskbarHeight, 0xff16161a); // Draw taskbar background
    DrawTaskbarIcon(0, corgiGL.getFramebuffer()->Height - taskbarHeight); // Draw the first taskbar icon
}

void GUI::TryTaskbarClick(unsigned int x, unsigned int y) {
    if (x < taskbarHeight && y >= corgiGL.getFramebuffer()->Height - taskbarHeight) {
        corgiGL.println("Button Clicked!");
    }
}