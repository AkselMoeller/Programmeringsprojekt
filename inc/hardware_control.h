#include "30010_io.h"

#ifndef _HARDWARE_CONTROL_H_
#define _HARDWARE_CONTROL_H_

uint8_t flag;

void initTimer();
void initJoyStick();
void TIM2_IRQHandler();
uint8_t readJoyStick();

#endif /*! _HARDWARE_CONTROL_H_ */
