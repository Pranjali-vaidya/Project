



#ifndef LED_H_
#define LED_H_

#include "stm32f4xx.h"

#define GPIO_LED		GPIOB
#define LED_GREEN_PIN	15
#define LED_YELLOW_PIN	12
#define LED_RED_PIN		13
#define BUZZER			14

#define GPIO_LED_CLKEN	1

void LedInit(uint32_t pin);
void LedOn(uint32_t pin);
void LedOff(uint32_t pin);

#endif /* LED_H_ */












