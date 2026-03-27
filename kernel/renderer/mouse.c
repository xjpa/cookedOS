// kernel/renderer/mouse.c
#include "../renderer/renderer.h"

void RenderMouse(int x, int y, int r, int g, int b) {
    // crosshair shape (10x10 bitmap)
    int mouse[] = {
        0b0000100000,  // vertical line
        0b0000100000,
        0b0000100000,
        0b0000100000,
        0b1111111111,  // horizontal line
        0b0000100000,
        0b0000100000,
        0b0000100000,
        0b0000100000,
        0b0000100000
    };
    int mouse_width = 10, mouse_height = 10;

    // render white outline (slightly larger area)
    for (int j = -1; j <= mouse_height; j++) {
        for (int i = -1; i <= mouse_width; i++) {
            int in_bounds = (i >= 0 && i < mouse_width && j >= 0 && j < mouse_height);
            int is_pixel = in_bounds && ((mouse[j] >> (mouse_width - 1 - i)) & 0b1);
            if (!is_pixel) {  // render white only around the shape
                int has_neighbor = 0;
                for (int dj = -1; dj <= 1 && !has_neighbor; dj++) {
                    for (int di = -1; di <= 1; di++) {
                        int nj = j + dj, ni = i + di;
                        if (nj >= 0 && nj < mouse_height && ni >= 0 && ni < mouse_width) {
                            if ((mouse[nj] >> (mouse_width - 1 - ni)) & 0b1) {
                                has_neighbor = 1;
                                break;
                            }
                        }
                    }
                }
                if (has_neighbor)
                    Render(x + i, y + j, 255, 255, 255);  // white outline
            }
        }
    }

    // render black cursor shape
    for (int j = 0; j < mouse_height; j++) {
        unsigned int row = mouse[j];
        int shift = mouse_width - 1;
        int bit_val = 0;
        for (int i = 0; i < mouse_width; i++) {
            bit_val = (row >> shift) & 0b1;
            if (bit_val == 1)
                Render(x + i, y + j, 0, 0, 0);  // black cursor
            shift -= 1;
        }
    }
}