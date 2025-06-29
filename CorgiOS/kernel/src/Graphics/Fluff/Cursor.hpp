#pragma once
#include <stdint.h>
#include "../CorgiGL.hpp"

extern uint16_t cursorImage[];
extern uint16_t cursorImageFill[];

void DrawMouseCursor(unsigned int x, unsigned int y);
void ClearMouseCursor(unsigned int x, unsigned int y);