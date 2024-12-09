#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "common.h"

typedef struct {
    void (*taskFunction)(void);
    uint8_t priority;
    uint8_t state;
} Task;

// Task queue
extern Task taskQueue[MAX_TASKS];
extern uint8_t taskCount; // Declare taskCount as extern4
extern uint8_t currentRoundRobinIndex;  // Declare currentRoundRobinIndex as extern

void addTask(void (*taskFunction)(void), uint8_t priority);
void sortTasksByPriority(void);
void Scheduler(void);

#endif