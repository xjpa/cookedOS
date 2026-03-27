// kernel/renderer/text.c
#include "../renderer/renderer.h"

void RenderCharacter(int (*f)(int, int), int font_width, int font_height, char character, int x, int y, int r, int g, int b) {
    for (int j = 0; j < font_height; j++) {
        unsigned int row = (*f)((int)(character), j);
        int shift = font_width - 1;
        int bit_val = 0;
        for (int i = 0; i < font_width; i++) {
            bit_val = (row >> shift) & 0b1;
            if (bit_val == 1)
                Render(x + i, y + j, r, g, b);
            shift -= 1;
        }
    }
}

void RenderString(int (*f)(int, int), int font_width, int font_height, char* string, int x, int y, int r, int g, int b) {
    int i = 0, j = 0;
    for (int k = 0; *(string + k) != 0; k++) {
        if (*(string + k) != '\n')
            RenderCharacter(f, font_width, font_height, *(string + k), x + i, y + j, r, g, b);
        i += font_width - (font_width / 5);
        if (*(string + k) == '\n') {
            i = 0;
            j += font_height;
        }
    }
}