#define STM32F051
#include <stdint.h>
#include "stm32f0xx.h"
#include "lcd_stm32f0.h"

void init_LEDs();
void display_on_LCD(uint8_t num);
void display_on_LEDs(uint8_t no);
void init_switches();

void main()
{
	//init_buttons();
	init_LEDs();
	init_LCD();
	init_switches();

	while(1)
	{
		display_on_LCD(16);
		display_on_LEDs(4);
	}
}

void display_on_LCD(uint8_t num)
{
	lcd_command(CLEAR);
	char string_num[3];
	sprintf(string_num, "%d",num);
	lcd_putstring(string_num);
	delay(500000);
}

void init_LEDs()
{
	// Enable GPIOB clock
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	// Setup GPIOB pins 0-7 as outputs
	GPIOB->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0;
	GPIOB->MODER |= GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;
}

void display_on_LEDs(uint8_t no)
{
	GPIOB->ODR = no;
}

void init_switches()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR2_0;
}
