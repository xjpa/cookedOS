// kernel/tasks/task_core.h
#ifndef TASK_CORE_H
#define TASK_CORE_H

#define task_type_void 0
#define task_type_string_buffer 1
#define task_params_length 10

struct Task {
    int priority;  // 0 to 5, 0 being highest
    int taskId;
    char ca1[100];
    int i1;
    int (*function)(int);
};

extern int TasksLength;
extern struct Task tasks[256];
extern int iparams[100];

void ProcessTasks();
void CloseTask(int taskId);

#endif