#include "tasks.h"
#include "gpio.h"
#include "mutex.h"
#include "stm32l4xx.h"
#include "stdio.h"


void closeAllLED(void){
	GPIOA->ODR &= ~(1U << RED_LIGHT_PIN);
	GPIOA->ODR &= ~(1U << GREEN_LIGHT_PIN);
	GPIOA->ODR &= ~(1U << YELLOW_LIGHT_PIN);
}

// Emergency Vehicle Task (High Priority)
void EmergencyVehicleTask(void) {
    /*
		if (GPIOA->IDR & (1 << EMERGENCY_SENSOR_PIN)) {  // Emergency vehicle detected
        acquire_mutex(&traffic_light_mutex);
        set_traffic_light(0, 1, 0);  // Green light for emergency vehicle
        for (volatile int i = 0; i < 1000000; i++);  // Simulate delay
        release_mutex(&traffic_light_mutex);
    }*/
		acquire_mutex(&traffic_light_mutex);
        //set_traffic_light(0, 0, 0, 1);  // Green light for emergency vehicle
				//GPIOA->ODR |= (1U << 5U);
				closeAllLED();
				GPIOA->ODR |= (1U << GREEN_LIGHT_PIN);
				printf("test_emer");
        for (volatile int i = 0; i < 1000000; i++);  // Simulate delay
        release_mutex(&traffic_light_mutex);
}

// Traffic Flow Control Task (Medium Priority)
void TrafficFlowControlTask(void) {
    static uint8_t state = 0;  // 0: Green, 1: Yellow, 2: Red
	  //set_traffic_light(1, 0, 0);
    acquire_mutex(&traffic_light_mutex);

    switch (state) {
        case 0: set_traffic_light(1, 0, 0, 0); break;  // red
        case 1: set_traffic_light(0, 0, 1, 0); break;  // yellow
        case 2: set_traffic_light(0, 1, 0, 0); break;  // green
    }
    state = (state + 1) % 3;

    release_mutex(&traffic_light_mutex);
    for (volatile int i = 0; i < 500000; i++);  // Simulate delay
}

// Pedestrian Signal Task (Low Priority)
void PedestrianSignalTask(void) {
    
        acquire_mutex(&traffic_light_mutex);
				closeAllLED();
        GPIOA->ODR |= (1U << 5U);  // Turn on pedestrian light
        for (volatile int i = 0; i < 1000000; i++);  // Simulate delay
        //GPIOA->ODR &= ~(1 << PEDESTRIAN_PIN);  // Turn off pedestrian light
        release_mutex(&traffic_light_mutex);
    
}
