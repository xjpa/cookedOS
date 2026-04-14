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

void RenderStringWrapped(int (*f)(int, int), int font_width, int font_height, char* string, int x, int y, int max_width, int max_height, int r, int g, int b) {
    int cursor_x = 0;
    int cursor_y = 0;
    int advance = font_width - (font_width / 5);

    if (advance <= 0 || max_width < font_width || max_height < font_height)
        return;

    for (int k = 0; *(string + k) != 0; k++) {
        char character = *(string + k);

        if (character == '\n') {
            cursor_x = 0;
            cursor_y += font_height;
            if (cursor_y + font_height > max_height)
                return;
            continue;
        }

        if (cursor_x + font_width > max_width) {
            cursor_x = 0;
            cursor_y += font_height;
        }

        if (cursor_y + font_height > max_height)
            return;

        RenderCharacter(f, font_width, font_height, character, x + cursor_x, y + cursor_y, r, g, b);
        cursor_x += advance;
    }
}
