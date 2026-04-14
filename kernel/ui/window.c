#include "../renderer/renderer.h"

static int frame = 0;

int RenderCircleButton(int x, int y, int radius, int r, int g, int b) {
    frame = (frame + 1) % 20;
    int pulse_radius = radius + (frame < 10 ? frame / 2 : (20 - frame) / 2);

    if (((mx - x) * (mx - x) + (my - y) * (my - y)) <= radius * radius) {
        RenderCircle(x, y, pulse_radius, r, g, b);
        if (left_clicked == TRUE) {
            left_clicked = FALSE;
            return 1;
        }
    } else {
        RenderCircle(x, y, radius, r, g, b);
    }

    return 0;
}

int RenderWindowFrame(int x, int y, int width, int height, char* title) {
    RenderRect(x, y, width, 20, 22, 44, 22);
    RenderRect(x, y + 20, width, height, 25, 50, 25);
    RenderRect(x + 1, y + 21, width - 2, height - 2, 31, 63, 31);

    int closeClicked = RenderCircleButton(x + 10, y + 10, 8, 31, 0, 0);

    // Yellow button at x + 30.
    RenderCircle(x + 30, y + 10, 8, 31, 31, 0);
    // Green button at x + 50.
    RenderCircle(x + 50, y + 10, 8, 0, 31, 0);

    RenderString(getArialCharacter, font_arial_width, font_arial_height,
                 title, x + 120, y + 3, 8, 16, 8);

    return closeClicked;
}
