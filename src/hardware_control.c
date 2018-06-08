#include "hardware_control.h"

void initJoyStick() {
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA; // Enable clock for GPIO Port A
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB; // Enable clock for GPIO Port B
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC; // Enable clock for GPIO Port C

    // Set pin PA4 to input
    GPIOA->MODER &= ~(0x00000003 << (4 * 2)); // Clear mode register
    GPIOA->MODER |= (0x00000000 << (4 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    //Set PA4 to pull-down
    GPIOA->PUPDR &= ~(0x00000003 << (4 * 2)); // Clear push/pull register
    GPIOA->PUPDR |= (0x00000002 << (4 * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

    // Set pin PB0 to input
    GPIOB->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
    GPIOB->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    //Set PB0 to pull-down
    GPIOB->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
    GPIOB->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

    // Set pin PB5 to input
    GPIOB->MODER &= ~(0x00000003 << (5 * 2)); // Clear mode register
    GPIOB->MODER |= (0x00000000 << (5 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    //Set PB5 to pull-down
    GPIOB->PUPDR &= ~(0x00000003 << (5 * 2)); // Clear push/pull register
    GPIOB->PUPDR |= (0x00000002 << (5 * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

    // Set pin PC0 to input
    GPIOC->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
    GPIOC->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    //Set PC0 to pull-down
    GPIOC->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
    GPIOC->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

    // Set pin PC1 to input
    GPIOC->MODER &= ~(0x00000003 << (1 * 2)); // Clear mode register
    GPIOC->MODER |= (0x00000000 << (1 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    //Set PC1 to pull-down
    GPIOC->PUPDR &= ~(0x00000003 << (1 * 2)); // Clear push/pull register
    GPIOC->PUPDR |= (0x00000002 << (1 * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)
}

uint8_t readJoyStick() {
    //Joystick controls: PA4 = up, PB0 = down, PB5 = center, PC0 = right, PC1 = left
    uint8_t joyStickState = 0;

    if (GPIOA->IDR & (0x0001 << 4)) {
        joyStickState = 0x01 << 0; //00000001, 1 (up)
    }
    if (GPIOB->IDR & (0x0001 << 0)) {
        joyStickState = 0x01 << 1; //00000010, 2 (down)
    }
    if (GPIOB->IDR & (0x0001 << 5)) {
        joyStickState = 0x01 << 4; //00010000, 16 (center)
    }
    if (GPIOC->IDR & (0x0001 << 0)) {
        joyStickState = 0x01 << 3; //00001000, 8 (right)
    }
    if (GPIOC->IDR & (0x0001 << 1)) {
        joyStickState = 0x01 << 2; //00000100, 4 (left)
    }

    return joyStickState;
}

void initTimer() {
    RCC->APB1ENR |= RCC_APB1Periph_TIM2; //Enable clock for timer 2
    TIM2->CR1 = 0x0000; //Reset timer (disabling entire timer 2)
    TIM2->ARR = 3199999; //Reload value (1/20 s period time, 20 Hz interrupt frequency)
    TIM2->PSC = 0; //Prescale value
    TIM2->DIER |= 0x0001;
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler() {
    //The code below executes whenever an interrupt occurs
    flag = 1;

    TIM2->SR &= ~0x0001; //Clear interrupt bit
}