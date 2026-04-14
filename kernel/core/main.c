// kernel/core/main.c
#include "../renderer/renderer.h"
#include "../tasks/task_core.h"

int start() {
    VBEInfoBlock* VBE = (VBEInfoBlock*) VBEInfoAddress;
    mx = VBE->x_resolution / 2;
    my = VBE->y_resolution / 2;

    char characterBuffer[1000] = "\0";
    char* characterBufferPointer = characterBuffer;
    int characterBufferLength = 0;

    base = (unsigned int) &isr1;  // temporary, isr1 is defined elsewhere
    base12 = (unsigned int) &isr12;  // temporary isr12 is defined elsewhere

    InitialiseMouse();
    InitialiseIDT();

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &ClearScreenTask;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &DesktopIconTask;
    iparams[TasksLength * task_params_length + 0] = 24;
    iparams[TasksLength * task_params_length + 1] = 24;
    iparams[TasksLength * task_params_length + 2] = 52;
    iparams[TasksLength * task_params_length + 3] = 52;
    iparams[TasksLength * task_params_length + 4] = 2;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &TestGraphicalElementsTask;
    iparams[TasksLength * task_params_length + 0] = 10;
    iparams[TasksLength * task_params_length + 1] = 10;
    iparams[TasksLength * task_params_length + 2] = 300;
    iparams[TasksLength * task_params_length + 3] = 300;
    iparams[TasksLength * task_params_length + 8] = FALSE;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &HandleKeyboardTask;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &BootSequenceTask;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &WelcomeScreenTask;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &RenderMouseTask;
    TasksLength++;

    while (1) {
        ProcessTasks();
        Flush();
    }
}
