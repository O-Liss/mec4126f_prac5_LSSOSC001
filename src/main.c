#define STM32F051
#include <stdint.h>
#include "stm32f0xx.h"
#include "lcd_stm32f0.h"

void init_LEDs();
void display_on_LCD(uint8_t num);
void display_on_LEDs(uint8_t no);
void init_switches();
void init_external_interupts();
uint8_t sw3_count = 0;

void main()
{
	//initialization
	init_LEDs();
	init_LCD();
	init_switches();
	init_external_interupts();

	//display original zero count
	uint8_t count=0;
	display_on_LCD(count);
	display_on_LEDs(count);


	while(1)
	{
		//enters this if statement when switch 3 count is odd
		if(sw3_count != 0)
		{
		//enters this if statement when sw1 has been pressed (pin goes low)
		if((GPIOA -> IDR &= GPIO_IDR_1 ) == 0)
		{
			//checks to see if the count has hit the max value. If has will skip this set of if/else statements
			if(count==255)
			{
				continue;
			}
			//if not at max value then increment the count and display
			else
			{
				count +=1;
				display_on_LCD(count);
				display_on_LEDs(count);
				delay(50000);
			}
		}
		//enters this if statement when sw2 has been pressed (pin goes low)
		else if((GPIOA -> IDR &= GPIO_IDR_2 ) == 0)
		{
			//checks to see if the count has hit the min value. If has will skip this set of if/else statements
			if(count==0)
			{
				continue;
			}
			//if not at max value then decrement the count and display
			else
			{
				count -=1;
				display_on_LCD(count);
				display_on_LEDs(count);
				delay(50000);
			}
		}
		}
		//enters this if statement when switch 3 count is even
		else
		{
			lcd_command(CLEAR);
			display_on_LEDs(0);
			delay(50000);
		}
	//delays are used to stop the double bounce on the switches
	}
}


//====================================================================
// Display a number on the LCD screen - display_on_LCD(uint8_t num)
//====================================================================
// DESCRIPTION: Displays a unsigned 8 bit integer to the LCD screen.
//====================================================================
void display_on_LCD(uint8_t num)
{
	lcd_command(CLEAR);
	char string_num[3];
	sprintf(string_num, "%d",num);
	lcd_putstring(string_num);
}

//====================================================================
// Initialize the LEDs - void init_LEDs()
//====================================================================
// DESCRIPTION: Enable GPIOB clock and Setup GPIOB pins 0-7 as outputs.
//====================================================================
void init_LEDs()
{
	// Enable GPIOB clock
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	// Setup GPIOB pins 0-7 as outputs
	GPIOB->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0;
	GPIOB->MODER |= GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;
}

//====================================================================
// Display a number on the LEDs  - display_on_LEDs(uint8_t no)
//====================================================================
// DESCRIPTION: Writes a unsigned 8 bit integer to be displayed in
// binary on the LEDs.
//====================================================================
void display_on_LEDs(uint8_t no)
{
	GPIOB->ODR = no;
}

//====================================================================
// Initialize the switches - init_switches()
//====================================================================
// DESCRIPTION: Enable GPIOA clock and Setup GPIOA pins 0-3 as inputs.
// Enable the pull up resistors on pins 0-3.
//====================================================================
void init_switches()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER1;
	GPIOA->MODER &= ~GPIO_MODER_MODER2;
	GPIOA->MODER &= ~GPIO_MODER_MODER3;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR2_0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_0;
}

//====================================================================
// Initialize the external interrupt - init_external_interupts()
//====================================================================
// DESCRIPTION: Enable the clock for sysconfig. Route PA to EXTI3.
// Unmask the interrupt on PA3. Set the falling edge trigger. Enable
// the EXTI2_3 interrupt on the NVIC
//====================================================================
void init_external_interupts()
{
	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	SYSCFG ->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;
	EXTI -> IMR |= EXTI_IMR_MR3;
	EXTI -> FTSR |= EXTI_FTSR_TR3;
	NVIC_EnableIRQ(EXTI2_3_IRQn);
}

//====================================================================
// Interrupt handler - EXTI2_3_IRQHandler()
//====================================================================
// DESCRIPTION: When interrupt flag goes low increment the global
// sw3_count variable. Reset the flag to high
//====================================================================
void EXTI2_3_IRQHandler()
{

	sw3_count +=1;
	sw3_count = sw3_count%2;
	EXTI -> PR |= EXTI_PR_PR3;
}
