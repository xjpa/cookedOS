// kernel/tasks/task_core.c
#include "../tasks/task_core.h"

int TasksLength = 0;
struct Task tasks[256];
int iparams[100] = {10};
int StartupPhase = startup_phase_boot;
int StartupFrame = 0;

void ProcessTasks() {
    int priority = 0;
    StartupFrame++;
    while (priority <= 5) {
        for (int i = 0; i < TasksLength; i++) {
            if (tasks[i].priority == priority) {
                tasks[i].function(tasks[i].taskId);
            }
        }
        priority++;
    }
}

void CloseTask(int taskId) {
    int lastTaskIndex = TasksLength - 1;

    for (int i = taskId; i < TasksLength - 1; i++) {
        tasks[i] = tasks[i + 1];
        tasks[i].taskId = i;

        for (int j = 0; j < task_params_length; j++) {
            iparams[i * task_params_length + j] =
                iparams[(i + 1) * task_params_length + j];
        }
    }

    for (int j = 0; j < task_params_length; j++) {
        iparams[lastTaskIndex * task_params_length + j] = 0;
    }

    TasksLength--;
}
