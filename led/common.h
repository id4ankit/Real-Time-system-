#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>  // For uint8_t, uint16_t, etc.

#define MAX_TASKS 10

// GPIO Pin Definitions (Adjust for your board)
#define RED_LIGHT_PIN    0
#define GREEN_LIGHT_PIN  2
#define YELLOW_LIGHT_PIN 1
#define PEDESTRIAN_PIN   3
#define EMERGENCY_SENSOR_PIN 4
#define TRAFFIC_SENSOR_PIN   5
#define PEDESTRIAN_BUTTON_PIN 6

// Shared Mutex
extern volatile uint8_t traffic_light_mutex;

void acquire_mutex(volatile uint8_t *mutex);
void release_mutex(volatile uint8_t *mutex);

#endif
