// kernel/tasks/tasks.c
#include "../tasks/task_core.h"
#include "../renderer/renderer.h"

int ClearScreenTask(int taskId) {
    ClearScreen(14, 27, 11); // desert beige
    return 0;
}

int DesktopIconTask(int taskId) {
    int x = iparams[taskId * task_params_length + 0];
    int y = iparams[taskId * task_params_length + 1];
    int width = iparams[taskId * task_params_length + 2];
    int height = iparams[taskId * task_params_length + 3];
    int targetTaskId = iparams[taskId * task_params_length + 4];
    int isPressed = iparams[taskId * task_params_length + 9];
    int targetWindowVisible = iparams[targetTaskId * task_params_length + 8];

    if (left_clicked == FALSE)
        iparams[taskId * task_params_length + 9] = FALSE;

    if (targetWindowVisible == FALSE &&
        left_clicked == TRUE &&
        mx >= x && mx <= x + width &&
        my >= y && my <= y + height + font_arial_height + 6) {
        if (isPressed == FALSE) {
            iparams[targetTaskId * task_params_length + 8] = TRUE;
            left_clicked = FALSE;
        }
        iparams[taskId * task_params_length + 9] = TRUE;
    }

    RenderRect(x + 10, y + 6, width - 20, height - 16, 6, 18, 31);
    RenderRect(x + 16, y, width - 20, 12, 31, 31, 31);
    RenderRect(x + 18, y + 18, width - 28, height - 28, 12, 24, 31);
    RenderString(getArialCharacter, font_arial_width, font_arial_height,
                 "Files", x - 2, y + height + 4, 31, 31, 31);
    return 0;
}

int RenderMouseTask(int taskId) {
    RenderMouse(mx, my, 16, (100.0 / 255.0 * 32), (100.0 / 255.0 * 16));
    return 0;
}

int HandleKeyboardTask(int taskId) {
    char* characterBuffer = tasks[taskId].ca1;
    int* characterBufferLength = &tasks[taskId].i1;
    char character = ProcessScancode(Scancode);

    if (backspace_pressed == TRUE) {
        characterBuffer[*characterBufferLength - 1] = '\0';
        (*characterBufferLength)--;
        backspace_pressed = FALSE;
        Scancode = -1;
    } else if (character != '\0') {
        characterBuffer[*characterBufferLength] = character;
        characterBuffer[*characterBufferLength + 1] = '\0';
        (*characterBufferLength)++;
        Scancode = -1;
    }
    RenderString(getArialCharacter, font_arial_width, font_arial_height, characterBuffer, 100, 100, 0, 0, 0);
    return 0;
}

int TestGraphicalElementsTask(int taskId) {
    int x = iparams[taskId * task_params_length + 0];
    int y = iparams[taskId * task_params_length + 1];
    int width = iparams[taskId * task_params_length + 2];
    int height = iparams[taskId * task_params_length + 3];
    int isVisible = iparams[taskId * task_params_length + 8];

    if (isVisible == FALSE)
        return 0;

    if (left_clicked == FALSE)
        iparams[taskId * task_params_length + 9] = FALSE;

    if (iparams[taskId * task_params_length + 9] == TRUE || 
        (left_clicked == TRUE && mx > x + 60 &&
         mx < x + width &&
         my > y &&
         my < y + 20)) {
        left_clicked = FALSE;
        iparams[taskId * task_params_length + 9] = TRUE;
        iparams[taskId * task_params_length + 0] = mx - (width / 2);
        iparams[taskId * task_params_length + 1] = my - 10;
    }
    if (RenderWindow(
        iparams[taskId * task_params_length + 0],
        iparams[taskId * task_params_length + 1],
        width,
        height,
        0, 0, 0
    ) == 1)
        iparams[taskId * task_params_length + 8] = FALSE;
    return 0;
}
