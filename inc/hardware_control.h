
#ifndef _HARDWARE_CONTROL_H_
#define _HARDWARE_CONTROL_H_

uint8_t flag;

void initTimer();
void TIM2_IRQHandler();

void initJoyStick();
uint8_t readJoyStick();

void initPotentiometer();
uint16_t readPotentiometer1();
uint16_t readPotentiometer2();
void deactivatePotentiometer();

void initTemperature();
int8_t readTemperature();

#endif /*_HARDWARE_CONTROL_H_*/
