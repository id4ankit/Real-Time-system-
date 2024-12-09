void Timer_Init(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;  // Enable TIM2 clock

    TIM2->PSC = 7999;  // Prescaler for 1 ms tick (80 MHz clock / 8000)
    TIM2->ARR = 1000;  // Auto-reload for 1-second interrupt
    TIM2->DIER |= TIM_DIER_UIE;  // Enable update interrupt
    TIM2->CR1 |= TIM_CR1_CEN;    // Start timer

    NVIC_EnableIRQ(TIM2_IRQn);   // Enable TIM2 interrupt in NVIC
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {  // Check interrupt flag
        TIM2->SR &= ~TIM_SR_UIF;  // Clear the flag
        Scheduler();  // Call the scheduler
    }
}
