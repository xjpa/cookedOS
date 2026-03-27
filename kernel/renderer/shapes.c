// kernel/renderer/shapes.c
#include "../renderer/renderer.h"

void RenderRect(int x, int y, int width, int height, int r, int g, int b) {
    for (int j = y; j < (y + height); j++) {
        for (int i = x; i < (x + width); i++) {
            Render(i, j, r, g, b);
        }
    }
}

void RenderCircle(int x, int y, int radius, int r, int g, int b) {
    int rr = radius * radius;
    for (int j = -radius; j < radius; j++) {
        for (int i = -radius; i < radius; i++) {
            if ((i * i + j * j) <= rr)
                Render(x + i, y + j, r, g, b);
        }
    }
}