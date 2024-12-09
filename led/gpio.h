#ifndef GPIO_H
#define GPIO_H

#include "common.h"

void GPIO_Init(void);
void EXTI_Init(void);
void set_traffic_light(uint8_t red, uint8_t green, uint8_t yellow, uint8_t white);

#endif
