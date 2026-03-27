// kernel/tasks/task_core.c
#include "../tasks/task_core.h"

int TasksLength = 0;
struct Task tasks[256];
int iparams[100] = {10};

void ProcessTasks() {
    int priority = 0;
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
    for (int i = taskId; i < TasksLength - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    TasksLength--;
}