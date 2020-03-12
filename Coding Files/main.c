/*
 * ECE 153B - Winter 2020
 *
 * Name(s): Andrew Berry and Alice Patig
 * Section: Tuesday
 * Final Project
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "UART.h"
#include "LCD.h"
#include "L3GD20.h"
#include "SPI.h"
#include <stdio.h>
#include "EXTI.h"


// For the servo motor
void PWM_Init() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN; //enable clock for GPIOE
	
	// Enable TIM1 Clock
	// TODO
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //enable clock of timer 1
	
	// Configure PE8
	// TODO
	//alternative function for PE8, alternative function = 10
	GPIOE->MODER &= ~GPIO_MODER_MODE8_0 ; //set PE8 to alternative function
	GPIOE->MODER |= GPIO_MODER_MODE8_1 ; //set PE8 to alternative function
	
	//setting PE8 to very high output speed = 11
	GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED8_0;//setting PE8 very high output speed
	GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED8_1;//setting PE8 very high output speed
	
	//config. pe8 to npunpd = 00
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD8_0; //setting PE8 to NPUNPD
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD8_1; //setting PE8 to NPUNPD
	
	//config. alternative function for PE8 = 0001
	GPIOE->AFR[1] |= GPIO_AFRH_AFSEL8_0; //setting pin 0 of AF1 to high *** look for AFRH
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL8_1;
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL8_2;
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL8_3;
	
	TIM1->CR1 &= ~TIM_CR1_DIR; //setting pin 4 to low to set the direction to up **** check 
	TIM1->PSC = 7999; // *** just to test, set prescaler to /7999
	TIM1->ARR = 199; // *** just to test, set auto-reload to 16^2
	TIM1->CCR1 = 0; // set the duty cycle of the PWM output to 50% (i.e. 32768 -> in hex:0x8000)
	//TIM1->CCMR1 |= TIM_CCMR1_OC1CE; // Output compare 1 clear enable
	
	//This section sets CCMR1 output compare mode to PWM mode 1, which corresponds to OC1M = 0110
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0; // Set the ouptput comapre mode bits to PWM mode 1
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1; // Set the ouptput comapre mode bits to PWM mode 1
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_2; // Set the ouptput comapre mode bits to PWM mode 1
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_3; // Set the ouptput comapre mode bits to PWM mode 1
	
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE; //enable output preload for ch 1N
	TIM1->CCER &= ~TIM_CCER_CC1NP; // set output polarity to active high ch 1N ****** check
	TIM1->CCER |= TIM_CCER_CC1NE; // enable output for channel 1N
	TIM1->BDTR |= TIM_BDTR_MOE; //enable the main output 
	
	TIM1->CR1 |= TIM_CR1_CEN; //set bit 0 to 1 to enable the counter
}
 
typedef struct {
	float x;
	float y;
	float z;
} L3GD20_Data_t;

int main() {
	System_Clock_Init(); // System Clock = 80 MHz
	SysTick_Init();
	
	// Initialize Gyroscope
	GYRO_Init(); 
	
	// Initialize LCD
	LCD_Initialization();
	LCD_Clear();
	
	// Initialize UART2 for USB Communication
	UART2_Init();
	UART2_GPIO_Init();
	USART_Init(USART2);
	
	// Initialization - We will use the default 4 MHz clock
	PWM_Init();
	
	// Initialize intterupt from arduino
	EXTI_Init();
	
	L3GD20_Data_t data;
	int dispensing = 6; // this is 0 degrees 10 / 200 = 0.05
	int not_dispensing = 12; // this is 90 degrees 15 / 200 = 0.075
	while(1) {
		
		//read gyroscope's status register to verify that there is new data to be read
		uint8_t temp = 0x00;
		GYRO_IO_Read(&temp, L3GD20_STATUS_REG_ADDR, 1);	
			uint8_t templ = 0x00;
			uint8_t temph = 0x00;
			
			GYRO_IO_Read(&templ, L3GD20_OUT_X_L_ADDR, 1);
			GYRO_IO_Read(&temph, L3GD20_OUT_X_H_ADDR, 1);
			int16_t out_x = templ | temph<<8;
			
			GYRO_IO_Read(&templ, L3GD20_OUT_Y_L_ADDR, 1);
			GYRO_IO_Read(&temph, L3GD20_OUT_Y_H_ADDR, 1);
			int16_t out_y = templ | temph<<8;
			
			GYRO_IO_Read(&templ, L3GD20_OUT_Z_L_ADDR, 1);
			GYRO_IO_Read(&temph, L3GD20_OUT_Z_H_ADDR, 1);
			int16_t out_z = templ | temph<<8;
			
			float conversion = 0.07;
		printf("Difference: previous: %f, new: %f\n", data.x, conversion*out_x);
			if((data.x - conversion*out_x > 50)){
				printf("START");
				TIM1->CCR1 = dispensing;
				LCD_DisplayString("START");
				delay(3000); // 3 seconds
			} else {
				printf("Stop");
				TIM1->CCR1 = not_dispensing;
				LCD_DisplayString("STOP   ");
			}
			data.x = conversion*out_x;
			data.y = conversion*out_y;
			data.z = conversion*out_z;
			
			
			// printf("(out_x, out_y, out_z):   (%.00f, %.00f, %.00f)\n", data.x, data.y, data.z);
		}
		delay(500); // Small delay between receiving measurements

		//sprintf(message, "%6d\n", ret);
		//LCD_DisplayString((uint8_t*) message);
		//printf("Sensed: %d\n", ret);
		
//		for(i = 0; i < 500; ++i); // Some Delay
//		int open = 6; // this is 0 degrees 10 / 200 = 0.05
//		int closed = 12; // this is 90 degrees 15 / 200 = 0.075
//		LCD_DisplayString("OPEN  ");
//		TIM1->CCR1 = open;
//		for(int i=0; i<10000000; ++i);
//		LCD_DisplayString("CLOSED");
//		TIM1->CCR1 = closed;
//		for(int i=0; i<10000000; ++i);
	
	return 0;
}
