
#ifndef _HARDWARE_CONTROL_H_
#define _HARDWARE_CONTROL_H_

uint8_t flag; //A global variable is needed i order to have a flag in TIM2_TRQHandler. Because this function is not able to take any arguments.

void initTimer();
void TIM2_IRQHandler();
void enableTimer();
void disableTimer();

void initJoyStick();
uint8_t readJoyStick();


void initPotentiometer();
uint16_t readPotentiometer1();
uint16_t readPotentiometer2();
void deactivatePotentiometer();

void initTemperature();
int8_t readTemperature();

#endif /*_HARDWARE_CONTROL_H_*/
