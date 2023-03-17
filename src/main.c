#define STM32F051
#include <stdint.h>
#include "stm32f0xx.h"
#include "lcd_stm32f0.h"

void init_LEDs();
void display_on_LCD(uint8_t num);
void display_on_LEDs(uint8_t no);
void init_switches();
void init_external_interupts();

void main()
{
	//init_buttons();
	init_LEDs();
	init_LCD();
	init_switches();
	init_external_interupts();
	EXTI2_3_IRQnHandler();

	uint8_t count=0;
	display_on_LCD(count);
	display_on_LEDs(count);


	while(1)
	{
		if((GPIOA -> IDR &= GPIO_IDR_1 ) == 0)
		{
			if(count==255)
			{
				continue;
			}
			else
			{
				count +=1;
				display_on_LCD(count);
				display_on_LEDs(count);
				delay(50000);
			}
		}
		else if((GPIOA -> IDR &= GPIO_IDR_2 ) == 0)
		{
			if(count==0)
			{
				continue;
			}
			else
			{
				count -=1;
				display_on_LCD(count);
				display_on_LEDs(count);
				delay(50000);
			}
		}
	}
}

void display_on_LCD(uint8_t num)
{
	lcd_command(CLEAR);
	char string_num[3];
	sprintf(string_num, "%d",num);
	lcd_putstring(string_num);
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

void init_external_interupts()
{
	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	SYSCFG ->EXTICR[0] = SYSCFG_EXTICR1_EXTI3_PA;
	EXTI -> IMR |= EXTI_IMR_MR3;
	EXTI -> FTSR |= EXTI_FTSR_TR3;
	NVIC_EnableIRQ(EXTI2_3_IRQn);
}

void EXTI2_3_IRQnHandler()
{
}
