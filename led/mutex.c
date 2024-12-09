#include "mutex.h"

volatile uint8_t traffic_light_mutex = 0;

void acquire_mutex(volatile uint8_t *mutex) {
    while (__atomic_test_and_set(mutex, __ATOMIC_ACQUIRE));
}

void release_mutex(volatile uint8_t *mutex) {
    __atomic_clear(mutex, __ATOMIC_RELEASE);
}
