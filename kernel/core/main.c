// kernel/core/main.c
#include "../renderer/renderer.h"
#include "../tasks/task_core.h"

int start() {
    VBEInfoBlock* VBE = (VBEInfoBlock*) VBEInfoAddress;
    int desktopIconTaskId;
    int taskbarTaskId;
    int textEditorWindowTaskId;
    int textEditorInputTaskId;

    mx = VBE->x_resolution / 2;
    my = VBE->y_resolution / 2;

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
    desktopIconTaskId = TasksLength;
    iparams[desktopIconTaskId * task_params_length + 0] = 24;
    iparams[desktopIconTaskId * task_params_length + 1] = 24;
    iparams[desktopIconTaskId * task_params_length + 2] = 52;
    iparams[desktopIconTaskId * task_params_length + 3] = 52;
    TasksLength++;

    taskbarTaskId = TasksLength;
    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &TaskbarTask;
    iparams[taskbarTaskId * task_params_length + 0] = 0;
    iparams[taskbarTaskId * task_params_length + 1] = VBE->y_resolution - 44;
    iparams[taskbarTaskId * task_params_length + 2] = VBE->x_resolution;
    iparams[taskbarTaskId * task_params_length + 3] = 44;
    TasksLength++;

    textEditorWindowTaskId = TasksLength;
    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &TextEditorWindowTask;
    iparams[textEditorWindowTaskId * task_params_length + 0] = 10;
    iparams[textEditorWindowTaskId * task_params_length + 1] = 10;
    iparams[textEditorWindowTaskId * task_params_length + 2] = 300;
    iparams[textEditorWindowTaskId * task_params_length + 3] = 300;
    iparams[textEditorWindowTaskId * task_params_length + 8] = FALSE;
    TasksLength++;

    textEditorInputTaskId = TasksLength;
    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &TextEditorInputTask;
    iparams[textEditorInputTaskId * task_params_length + 0] = textEditorWindowTaskId;
    TasksLength++;

    iparams[desktopIconTaskId * task_params_length + 4] = textEditorWindowTaskId;
    iparams[taskbarTaskId * task_params_length + 4] = textEditorWindowTaskId;

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
