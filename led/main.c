#include "stm32l4xx.h" // Include STM32 device header (CMSIS)
#include <stdint.h>    // Include standard integer types
#include "gpio.h"
#include "stdio.h"
#include "scheduler.h"
#include "tasks.h"

#include "core_cm4.h"  // Replace with "core_cm7.h" for Cortex-M7


void SystemClock_Config(void) {
    // Enable HSI16
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY)); // Wait for HSI16 to stabilize

    // Select HSI16 as system clock
    RCC->CFGR |= RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI); // Wait for HSI16 to be active

    // Update SystemCoreClock
    SystemCoreClock=16000000;
}


// Redirect printf to ITM
int fputc(int ch, FILE *f) {
    // Send character to ITM Stimulus Port 0 Stimulus Port 0
    ITM_SendChar(ch);
    return ch;
}


//void GPIOA_Init(void);       // Function to initialize GPIO for LEDs
void delay_ms(uint32_t ms);  // Delay function

int main(void) {
		SystemClock_Config();
    GPIO_Init(); // Initialize GPIOA for LEDs
		EXTI_Init();

		addTask(EmergencyVehicleTask, 3);  // High Priority
		//addTask(PedestrianSignalTask, 2);
    addTask(TrafficFlowControlTask, 2);
	
		while(1){
			Scheduler();
		}
    
}

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (4000U * ms) - 1U;  // Assuming 4 MHz clock
    SysTick->VAL = 0U;                  // Clear current value
    SysTick->CTRL = 5U;                 // Enable SysTick with processor clock

    while ((SysTick->CTRL & (1U << 16U)) == 0U); // Wait for COUNTFLAG

    SysTick->CTRL = 0U;                 // Disable SysTick
}
