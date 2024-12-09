#include "stm32l4xx.h"
#include "gpio.h"

#define MAX_TASKS 10

typedef struct {
    void (*taskFunction)(void);  // Pointer to the task function
    uint8_t priority;           // Task priority
    uint8_t state;              // 1: Ready, 2: Running, 0: Inactive
} Task;

static Task taskQueue[MAX_TASKS];
static uint8_t taskCount = 0;
static uint8_t currentRoundRobinIndex = 0;

// Function to sort tasks by priority (descending order)
void sortTasksByPriority(void) {
    for (int i = 0; i < taskCount - 1; i++) {
        for (int j = 0; j < taskCount - i - 1; j++) {
            if (taskQueue[j].priority < taskQueue[j + 1].priority) {
                Task temp = taskQueue[j];
                taskQueue[j] = taskQueue[j + 1];
                taskQueue[j + 1] = temp;
            }
        }
    }
}

// Function to add tasks to the queue
void addTask(void (*taskFunction)(void), uint8_t priority) {
    if (taskCount < MAX_TASKS) {
        taskQueue[taskCount].taskFunction = taskFunction;
        taskQueue[taskCount].priority = priority;
        taskQueue[taskCount].state = 1;  // Ready state
        taskCount++;
    }
}



// Scheduler function
void Scheduler(void) {
		sortTasksByPriority();
    if (GPIOA->IDR & (1U << 4U)) {  // Check if PA4 (button) is pressed (interrupt triggered)
        uint8_t highestPriority = taskQueue[0].priority;
        uint8_t roundRobinStartIndex = 0;

        // Find range of tasks with highest priority
        for (int i = 0; i < taskCount; i++) {
            if (taskQueue[i].priority != highestPriority) break;
            roundRobinStartIndex++;
        }

        // Execute tasks with the highest priority in round-robin fashion
        for (int i = 0; i < roundRobinStartIndex; i++) {
            currentRoundRobinIndex = (currentRoundRobinIndex + 1) % roundRobinStartIndex;
            if (taskQueue[currentRoundRobinIndex].state == 1) {  // If task is ready
                taskQueue[currentRoundRobinIndex].state = 2;  // Set to running
                taskQueue[currentRoundRobinIndex].taskFunction();  // Execute task
                taskQueue[currentRoundRobinIndex].state = 1;  // Set back to ready
            }
        }
    } else {
        // Always run TrafficFlowControlTask if no higher-priority task needs to run
        taskQueue[1].taskFunction();  // TrafficFlowControlTask runs continuously
    }
}

// EXTI4 interrupt handler
void EXTI4_IRQHandler(void)
{
    if (EXTI->PR1 & EXTI_PR1_PIF4) // Check if interrupt is from PA4
    {
        EXTI->PR1 |= EXTI_PR1_PIF4;  // Clear the interrupt flag
        Scheduler();                 // Call the scheduler to handle high-priority tasks
    }
}
