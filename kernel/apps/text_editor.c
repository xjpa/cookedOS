#include "../tasks/task_core.h"
#include "../renderer/renderer.h"

static char TextEditorTitle[] = "Text Editor";

int TextEditorInputTask(int taskId) {
    int windowTaskId = iparams[taskId * task_params_length + 0];
    char* characterBuffer = tasks[taskId].ca1;
    int* characterBufferLength = &tasks[taskId].i1;
    char character = ProcessScancode(Scancode);
    int windowVisible = iparams[windowTaskId * task_params_length + 8];
    int windowX = iparams[windowTaskId * task_params_length + 0];
    int windowY = iparams[windowTaskId * task_params_length + 1];
    int windowWidth = iparams[windowTaskId * task_params_length + 2];
    int windowHeight = iparams[windowTaskId * task_params_length + 3];

    if (StartupPhase != startup_phase_desktop)
        return 0;

    if (windowVisible == FALSE)
        return 0;

    if (backspace_pressed == TRUE) {
        if (*characterBufferLength > 0) {
            characterBuffer[*characterBufferLength - 1] = '\0';
            (*characterBufferLength)--;
        }
        backspace_pressed = FALSE;
        Scancode = -1;
    } else if (character != '\0' &&
               *characterBufferLength < (int)(sizeof(tasks[taskId].ca1) - 1)) {
        characterBuffer[*characterBufferLength] = character;
        characterBuffer[*characterBufferLength + 1] = '\0';
        (*characterBufferLength)++;
        Scancode = -1;
    }

    RenderStringWrapped(getArialCharacter, font_arial_width, font_arial_height,
                        characterBuffer, windowX + 16, windowY + 36,
                        windowWidth - 32, windowHeight - 44, 0, 0, 0);
    return 0;
}

int TextEditorWindowTask(int taskId) {
    int x = iparams[taskId * task_params_length + 0];
    int y = iparams[taskId * task_params_length + 1];
    int width = iparams[taskId * task_params_length + 2];
    int height = iparams[taskId * task_params_length + 3];
    int isVisible = iparams[taskId * task_params_length + 8];

    if (StartupPhase != startup_phase_desktop)
        return 0;

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

    if (RenderWindowFrame(
            iparams[taskId * task_params_length + 0],
            iparams[taskId * task_params_length + 1],
            width,
            height,
            TextEditorTitle) == 1)
        iparams[taskId * task_params_length + 8] = FALSE;

    return 0;
}
