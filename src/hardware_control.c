#include "30010_io.h"
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
    TIM2->ARR = 999999; //Reload value (1/20 s period time, 20 Hz interrupt frequency)
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

void initPotentiometer() {
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA; // Enable clock for GPIO Port A

        // Set pin PA0 to input
    GPIOA->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
    GPIOA->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
        //Set PA0 to pull-down
    GPIOA->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
    GPIOA->PUPDR |= (0x00000000 << (0 * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

        // Set pin PA1 to input
    GPIOA->MODER &= ~(0x00000003 << (1 * 2)); // Clear mode register
    GPIOA->MODER |= (0x00000000 << (1 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
        //Set PA1 to pull-down
    GPIOA->PUPDR &= ~(0x00000003 << (1 * 2)); // Clear push/pull register
    GPIOA->PUPDR |= (0x00000000 << (1 * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

    //Configure clock source
    RCC->CFGR2 &= ~RCC_CFGR2_ADCPRE12; // Clear ADC12 prescaler bits
    RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV6; // Set ADC12 prescaler to 6
    RCC->AHBENR |= RCC_AHBPeriph_ADC12; // Enable clock for ADC12

    ADC1->CR = 0x00000000; // Clear CR register
    ADC1->CFGR &= 0xFDFFC007; // Clear ADC1 config register
    ADC1->SQR1 &= ~ADC_SQR1_L; // Clear regular sequence register 1

    ADC1->CR |= 0x10000000; // Enable internal ADC voltage regulator
    for (int i = 0 ; i < 1000 ; i++) {} // Wait for about 16 microseconds

    //Perform calib
    ADC1->CR |= 0x80000000; // Start ADC1 calibration
    while (!(ADC1->CR & 0x80000000)); // Wait for calibration to finish
    for (int i = 0 ; i < 100 ; i++) {} // Wait for a little while

    ADC1->CR |= 0x00000001; // Enable ADC1 (0x01 - Enable, 0x02 - Disable)
    while (!(ADC1->ISR & 0x00000001)); // Wait until ready
}

void deactivatePotentiometer() {
    ADC1->CR &= 0x0000002; // Enable ADC1 (0x01 - Enable, 0x02 - Disable)
    while (!(ADC1->ISR & 0x00000001)); // Wait until ready

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_1Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_1Cycles5);
}

uint16_t readPotentiometer1() {
    //Reading from channel 1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);

    ADC_StartConversion(ADC1); //Start ADC read
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0); //Wait for ADC read

    return 100 + ADC_GetConversionValue(ADC1)/41; //Read ADC value
}

uint16_t readPotentiometer2() {
    //Reading from channel 2
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);

    ADC_StartConversion(ADC1); //Start ADC read
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0); //Wait for ADC read

    return 42 + ADC_GetConversionValue(ADC1)/226; //Read ADC value
}

void initTemperature() {
    ADC1_2->CCR = 0x00000000; //Clear CCR register
    ADC1_2->CCR |= 0x00800000;//Enable TSEN for Temperature reading
}

int8_t readTemperature() {
    //Reading from channel 16 (temperature sensor)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_1Cycles5);

    ADC_StartConversion(ADC1); // Start ADC read
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0); //WAIT!!

    int16_t x = ADC_GetConversionValue(ADC1); // Read ADC value

    return 167 - x/10; // convert a number from 1690(min temp) - 1650(max temp) to a number from 0-8
}
