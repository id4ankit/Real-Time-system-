#include "gpio.h"
#include "stm32l4xx.h"
#include<stdio.h>

void GPIO_Init(void) {
    // Enable GPIOA clock
    RCC->AHB2ENR |= (1U << 0U);  // Enable GPIOA clock (bit 0)

    // Configure PA0, PA1, and PA2 as output (for Traffic lights)
    GPIOA->MODER &= ~((3U << (0U * 2U)) | (3U << (1U * 2U)) | (3U << (2U * 2U)));  // Clear MODER bits for PA0, PA1, PA2
    GPIOA->MODER |= (1U << (0U * 2U)) | (1U << (1U * 2U)) | (1U << (2U * 2U));    // Set PA0, PA1, and PA2 as General Purpose Output

    // Configure PA0, PA1, and PA2 as push-pull
    GPIOA->OTYPER &= ~((1U << 0U) | (1U << 1U) | (1U << 2U));  // Push-pull mode for PA0, PA1, and PA2

    // Configure PA0, PA1, and PA2 for low speed
    GPIOA->OSPEEDR &= ~((3U << (0U * 2U)) | (3U << (1U * 2U)) | (3U << (2U * 2U)));  // Low speed for PA0, PA1, and PA2

    // No pull-up, pull-down for PA0, PA1, and PA2
    GPIOA->PUPDR &= ~((3U << (0U * 2U)) | (3U << (1U * 2U)) | (3U << (2U * 2U)));   // No pull-up/pull-down

// Configure PA5 as output (for LED)
    GPIOA->MODER &= ~(3U << (5U * 2U));  // Clear MODER bits for PA5
    GPIOA->MODER |= (1U << (5U * 2U));   // Set PA5 as General Purpose Output

    // Configure PA5 as push-pull
    GPIOA->OTYPER &= ~(1U << 5U);   // Push-pull mode for PA5 (LED)

    // Configure PA5 for low speed
    GPIOA->OSPEEDR &= ~(3U << (5U * 2U));  // Low speed for PA5

    // No pull-up, pull-down for PA5 (LED)
    GPIOA->PUPDR &= ~(3U << (5U * 2U));  // No pull-up/pull-down for PA5

    // Configure PA4 as input (button)
    GPIOA->MODER &= ~(3U << (4U * 2U));  // Set PA4 as input mode (00)
    GPIOA->PUPDR &= ~(3U << (4U * 2U));  // No pull-up/pull-down for PA4 (button)
}

void EXTI_Init(void) {
    // Enable SYSCFG clock (needed to configure EXTI)
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Configure EXTI line for PA4 (mapping PA4 to EXTI4)
    SYSCFG->EXTICR[1] &= ~(0xFU << (4U * 0U));  // Clear EXTI4 bits
    SYSCFG->EXTICR[1] |= (0x0U << (4U * 0U));   // Map PA4 to EXTI4 (in SYSCFG_EXTICR1)

    // Configure EXTI for PA4
    EXTI->IMR1 |= (1U << 4U);    // Unmask EXTI line 4 (enable interrupt for PA4)
    EXTI->RTSR1 |= (1U << 4U);   // Enable rising edge trigger for EXTI4

    // Enable EXTI4 interrupt in NVIC (interrupt vector table)
    NVIC_EnableIRQ(EXTI4_IRQn);
    NVIC_SetPriority(EXTI4_IRQn, 1);  // Optional: Set interrupt priority
}




void set_traffic_light(uint8_t red, uint8_t green, uint8_t yellow, uint8_t white) {
    // Control RED light
			if (red) {
        GPIOA->ODR |= (1U << RED_LIGHT_PIN); // Turn on RED
			} else {
        GPIOA->ODR &= ~(1U << RED_LIGHT_PIN); // Turn off RED
			}

    // Control GREEN light
			if (green) {
        GPIOA->ODR |= (1U << GREEN_LIGHT_PIN); // Turn on GREEN
			} else {
        GPIOA->ODR &= ~(1U << GREEN_LIGHT_PIN); // Turn off GREEN
			}

    // Control YELLOW light
			if (yellow) {
        GPIOA->ODR |= (1U << YELLOW_LIGHT_PIN); // Turn on YELLOW
			} else {
        GPIOA->ODR &= ~(1U << YELLOW_LIGHT_PIN); // Turn off YELLOW
			}
		
    
		//GPIOA->ODR &= ~(1U << 5U);
}

