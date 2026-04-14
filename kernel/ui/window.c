#include "../renderer/renderer.h"
static int frame = 0;

// Update your RenderCircleButton so that it always shows full color when not hovered,
// and only pulsates (using full color) when the mouse is over it.
int RenderCircleButton(int x, int y, int radius, int r, int g, int b) {
    frame = (frame + 1) % 20;
    int pulse_radius = radius + (frame < 10 ? frame / 2 : (20 - frame) / 2);
    // Compute squared distance from the mouse to the button center.
    if (((mx - x) * (mx - x) + (my - y) * (my - y)) <= radius * radius) {
        // If hovering, use the pulsating effect.
        RenderCircle(x, y, pulse_radius, r, g, b);
        if (left_clicked == TRUE) {
            left_clicked = FALSE;
            return 1;
        }
    } else {
        // Otherwise, always render with full color (no dimming).
        RenderCircle(x, y, radius, r, g, b);
    }
    return 0;
}

// Modify RenderWindow to draw 3 buttons in the title bar, as in macOS.
// The leftmost button (red) is interactive; the other two (yellow and green) are just aesthetic.
int RenderWindow(int x, int y, int width, int height, int r, int g, int b) {
    // Draw a TextEdit-like window with a soft grey title bar and white document area.
    RenderRect(x, y, width, 20, 22, 44, 22);
    RenderRect(x, y + 20, width, height, 25, 50, 25);
    RenderRect(x + 1, y + 21, width - 2, height - 2, 31, 63, 31);

    // Render the interactive red close button.
    // Placed at x + 10 so it sits at the left side of the title bar.
    int closeClicked = RenderCircleButton(x + 10, y + 10, 8, 31, 0, 0);

    // Render two static aesthetic buttons:
    // Yellow button at x + 30.
    RenderCircle(x + 30, y + 10, 8, 31, 31, 0);
    // Green button at x + 50.
    RenderCircle(x + 50, y + 10, 8, 0, 31, 0);

    RenderString(getArialCharacter, font_arial_width, font_arial_height,
                 "Untitled", x + 120, y + 3, 8, 16, 8);

    return closeClicked;
}
