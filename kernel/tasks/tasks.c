// kernel/tasks/tasks.c
#include "../tasks/task_core.h"
#include "../renderer/renderer.h"

static void AppendString(char* destination, int* index, const char* source) {
    while (*source != '\0') {
        destination[*index] = *source;
        (*index)++;
        source++;
    }
    destination[*index] = '\0';
}

static void AppendNumber(char* destination, int* index, int value) {
    char digits[12];
    int digit_count = 0;

    if (value == 0) {
        destination[*index] = '0';
        (*index)++;
        destination[*index] = '\0';
        return;
    }

    while (value > 0 && digit_count < 11) {
        digits[digit_count] = '0' + (value % 10);
        digit_count++;
        value /= 10;
    }

    while (digit_count > 0) {
        digit_count--;
        destination[*index] = digits[digit_count];
        (*index)++;
    }
    destination[*index] = '\0';
}

static void BuildResolutionLine(char* buffer, VBEInfoBlock* VBE) {
    int index = 0;
    buffer[0] = '\0';
    AppendString(buffer, &index, "Video mode: ");
    AppendNumber(buffer, &index, VBE->x_resolution);
    AppendString(buffer, &index, "x");
    AppendNumber(buffer, &index, VBE->y_resolution);
    AppendString(buffer, &index, "x");
    AppendNumber(buffer, &index, VBE->bits_per_pixel);
}

static void BuildTaskLine(char* buffer) {
    int index = 0;
    buffer[0] = '\0';
    AppendString(buffer, &index, "Task scheduler: online (");
    AppendNumber(buffer, &index, TasksLength);
    AppendString(buffer, &index, " tasks)");
}

static void RenderBootLine(int x, int y, const char* label, int showStatus, int statusR, int statusG, int statusB) {
    RenderString(getArialCharacter, font_arial_width, font_arial_height,
                 (char*)label, x, y, 31, 63, 31);
    if (showStatus == TRUE) {
        RenderString(getArialCharacter, font_arial_width, font_arial_height,
                     "[ OK ]", x + 440, y, statusR, statusG, statusB);
    }
}

int ClearScreenTask(int taskId) {
    if (StartupPhase == startup_phase_boot)
        ClearScreen(0, 0, 0);
    else if (StartupPhase == startup_phase_welcome)
        ClearScreen(3, 8, 14);
    else
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

    if (StartupPhase != startup_phase_desktop)
        return 0;

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
    if (StartupPhase != startup_phase_desktop)
        return 0;
    RenderMouse(mx, my, 16, (100.0 / 255.0 * 32), (100.0 / 255.0 * 16));
    return 0;
}

int HandleKeyboardTask(int taskId) {
    int windowTaskId = 2;
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

    RenderRect(windowX + 8, windowY + 28, 2, 2, 24, 24, 24);
    RenderStringWrapped(getArialCharacter, font_arial_width, font_arial_height,
                        characterBuffer, windowX + 16, windowY + 36,
                        windowWidth - 32, windowHeight - 44, 0, 0, 0);
    return 0;
}

int TestGraphicalElementsTask(int taskId) {
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
    if (RenderWindow(
        iparams[taskId * task_params_length + 0],
        iparams[taskId * task_params_length + 1],
        width,
        height,
        31, 31, 31
    ) == 1)
        iparams[taskId * task_params_length + 8] = FALSE;
    return 0;
}

int BootSequenceTask(int taskId) {
    VBEInfoBlock* VBE = (VBEInfoBlock*) VBEInfoAddress;
    char resolutionLine[48];
    char taskLine[48];
    int bootFrame = StartupFrame;
    int x = 20;
    int y = 20;
    int lineHeight = font_arial_height + 4;

    if (StartupPhase != startup_phase_boot)
        return 0;

    BuildResolutionLine(resolutionLine, VBE);
    BuildTaskLine(taskLine);

    RenderString(getArialCharacter, font_arial_width, font_arial_height,
                 "cookedOS startup sequence v0.1", x, y, 31, 63, 31);
    RenderString(getArialCharacter, font_arial_width, font_arial_height,
                 "--------------------------------", x, y + lineHeight, 18, 38, 18);

    if (bootFrame > 15)
        RenderString(getArialCharacter, font_arial_width, font_arial_height,
                     resolutionLine, x, y + lineHeight * 3, 24, 48, 31);
    if (bootFrame > 35)
        RenderBootLine(x, y + lineHeight * 5, "Framebuffer: initialized", TRUE, 8, 54, 8);
    if (bootFrame > 55)
        RenderBootLine(x, y + lineHeight * 6, "Keyboard input: initialized", TRUE, 8, 54, 8);
    if (bootFrame > 75)
        RenderBootLine(x, y + lineHeight * 7, "Mouse input: initialized", TRUE, 8, 54, 8);
    if (bootFrame > 95)
        RenderBootLine(x, y + lineHeight * 8, "Interrupt table: loaded", TRUE, 8, 54, 8);
    if (bootFrame > 115)
        RenderString(getArialCharacter, font_arial_width, font_arial_height,
                     taskLine, x, y + lineHeight * 9, 31, 63, 31);
    if (bootFrame > 135)
        RenderBootLine(x, y + lineHeight * 10, "Desktop services: starting", TRUE, 8, 54, 8);
    if (bootFrame > 155)
        RenderBootLine(x, y + lineHeight * 11, "Window manager: starting", TRUE, 8, 54, 8);
    if (bootFrame > 175)
        RenderString(getArialCharacter, font_arial_width, font_arial_height,
                     "Launching session...", x, y + lineHeight * 13, 31, 54, 10);

    if (bootFrame > 220) {
        StartupPhase = startup_phase_welcome;
        StartupFrame = 0;
    }

    return 0;
}

int WelcomeScreenTask(int taskId) {
    int welcomeFrame = StartupFrame;
    int dotCount;

    if (StartupPhase != startup_phase_welcome)
        return 0;

    RenderRect(80, 70, 480, 280, 6, 14, 21);
    RenderRect(88, 78, 464, 264, 10, 20, 30);
    RenderString(getArialCharacter, font_arial_width, font_arial_height,
                 "Welcome to cookedOS", 180, 130, 31, 63, 31);
    RenderString(getArialCharacter, font_arial_width, font_arial_height,
                 "Preparing desktop session", 160, 178, 22, 44, 31);

    dotCount = (welcomeFrame / 18) % 4;
    if (dotCount >= 0)
        RenderString(getArialCharacter, font_arial_width, font_arial_height,
                     dotCount == 0 ? "" :
                     dotCount == 1 ? "." :
                     dotCount == 2 ? ".." : "...",
                     392, 178, 22, 44, 31);

    RenderRect(160, 226, 320, 18, 8, 16, 21);
    RenderRect(164, 230, (welcomeFrame * 312) / 150, 10, 10, 42, 18);
    RenderString(getArialCharacter, font_arial_width, font_arial_height,
                 "Loading user workspace", 176, 270, 24, 52, 24);

    if (welcomeFrame > 150) {
        StartupPhase = startup_phase_desktop;
        StartupFrame = 0;
    }

    return 0;
}
