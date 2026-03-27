// kernel/tasks/tasks.c
#include "../tasks/task_core.h"
#include "../renderer/renderer.h"

int ClearScreenTask(int taskId) {
    ClearScreen(14, 27, 11); // desert beige
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
    if (left_clicked == FALSE)
        iparams[taskId * task_params_length + 9] = FALSE;

    if (iparams[taskId * task_params_length + 9] == TRUE || 
        (left_clicked == TRUE && mx > iparams[taskId * task_params_length + 0] &&
         mx < iparams[taskId * task_params_length + 0] + iparams[taskId * task_params_length + 2] - 30 &&
         my > iparams[taskId * task_params_length + 1] &&
         my < iparams[taskId * task_params_length + 1] + 20)) {
        left_clicked = FALSE;
        iparams[taskId * task_params_length + 9] = TRUE;
        iparams[taskId * task_params_length + 0] = mx - (iparams[taskId * task_params_length + 2] / 2);
        iparams[taskId * task_params_length + 1] = my - 10;
    }
    if (RenderWindow(
        iparams[taskId * task_params_length + 0],
        iparams[taskId * task_params_length + 1],
        iparams[taskId * task_params_length + 2],
        iparams[taskId * task_params_length + 3],
        0, 0, 0
    ) == 1)
        CloseTask(taskId);
    return 0;
}