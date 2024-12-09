#ifndef MUTEX_H
#define MUTEX_H

#include "common.h"

void acquire_mutex(volatile uint8_t *mutex);
void release_mutex(volatile uint8_t *mutex);

#endif
