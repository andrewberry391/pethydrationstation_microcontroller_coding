/*
 * ECE 153B - Winter 2020
 *
 * Name(s): Andrew Berry and Alice Patig
 * Section: Tuesday
 * Final Project
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h" // Comment out to use LCD
#include "LCD.h" // Comment out to use UART
#include "I2C.h"

typedef struct {
	float humidity;
	float temperature;
} I2C_SOIL_SENSOR;


// For the servo motor
void PWM_Init() {
	// TODO
	// Enable the clock for GPIOE
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	// Set Mode of PB6 to alternative= 10
	GPIOE->MODER |= GPIO_MODER_MODE11_1;
	GPIOE->MODER &= ~GPIO_MODER_MODE11_0;
	// Set correct alternative function for PE11 to 0001
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL11_3; // 0
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL11_2; // 0
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL11_1; // 0
	GPIOE->AFR[1] |= GPIO_AFRH_AFSEL11_0; // 1
	// Set PB6 to No pull up, No pull down
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD11_1; //setting PE6 to NPUNPD
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD11_0; //setting PE6 to NPUNPD
	// Set to very high output speed = 11
	GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED11_1; // 1
	GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED11_0; // 1
	// Set the prescaler to 15
	TIM1->PSC = 15;
	// Enable timer 1 for RCC_APB2ENR
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //enable clock of timer 1
	// Enable auto preload in CR for TIM1
	TIM1->CR1 |= TIM_CR1_ARPE; // Setting register is buffered
	// Set auto reload to maximum value
	TIM1->ARR = 199;
	// Set the CCR2 value
	TIM1->CCR2 = 20; // set the duty cycle of the PWM output to 10
	//This section sets CCMR1 output compare mode to PWM mode 1, which corresponds to OC1M = 0110
	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_0; // Set the ouptput comapre mode bits to PWM mode 1
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_1; // Set the ouptput comapre mode bits to PWM mode 1
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2; // Set the ouptput comapre mode bits to PWM mode 1
	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_3; // Set the ouptput comapre mode bits to PWM mode 1
	// Enable the output compare preload
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE; //enable output preload
	// Enable output capture compare enable register
	TIM1->CCER |= TIM_CCER_CC2E;
	// in break and dead time register set bits main output enable 
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->BDTR |= TIM_BDTR_OSSR;
	// Event Generation Register, enable update generation 
	TIM1->EGR |= TIM_EGR_UG; 
	// Clear the update interrupt flag
	TIM1->SR &= ~TIM_SR_UIF;
	//enable update interrupt enable
	TIM1->DIER |= TIM_DIER_UIE;
	//Enable the counter of the control register
	TIM1->CR1 |= TIM_CR1_CEN;
	// Control register: Set the direction of the counter to up*** check if not up
	TIM1->CR1 &= ~TIM_CR1_DIR; 
}
 
int main() {
	System_Clock_Init(); // System Clock = 80 MHz
	
	// Initialize LCD
	LCD_Initialization();
	LCD_Clear();
	
	// Initialize I2C
	I2C_GPIO_Init();
	I2C_Initialization();
	
	// Initialize UART2 for USB Communication
	UART2_Init();
	UART2_GPIO_Init();
	USART_Init(USART2);
	
	// Initialization - We will use the default 4 MHz clock
	//PWM_Init();
	
	printf("TEST\n");
	
	int i;
	char message[6];
	uint8_t SlaveAddress = 0x36 << 1; // Address of the slave sensor I2C
	uint8_t SEESAW_TOUCH_BASE = 0x0F;
	uint8_t SEESAW_TOUCH_CHANNEL_OFFSET = 0x10;
//	uint8_t start_send = SEESAW_SERCOM0_BASE | SEESAW_STATUS_HW_ID;
//	I2C_SendData(I2C1, SlaveAddress, &start_send, 1);
	while(1) {
		// TODO (changing duty cycle, etc.)
		// Change CCR1 to decide how long the pulse is high for 1ms 0degrees, 1.5ms 90degrees, 2ms is 180degrees
		uint8_t data_send[2] = {SEESAW_TOUCH_BASE, SEESAW_TOUCH_CHANNEL_OFFSET};
		//uint8_t data_send = SEESAW_TOUCH_BASE | SEESAW_TOUCH_CHANNEL_OFFSET;
		uint8_t data_recieved[2];
		uint16_t ret;
		//do {
			//for(i = 0; i < 1000; ++i); // Some Delay
			I2C_SendData(I2C1, SlaveAddress, data_send, 2);
		
			//for(i = 0; i < 1000; ++i); // Some Delay
			I2C_ReceiveData(I2C1, SlaveAddress, data_recieved, 2);
			
			ret = ((uint16_t)data_recieved[0]<<8) | data_recieved[1];
		//}while (ret == 65535);
		
		sprintf(message, "%6d\n", ret);
		LCD_DisplayString((uint8_t*) message);
		//printf("Sensed: %d\n", ret);
		
		for(i = 0; i < 500; ++i); // Some Delay
		
		
//		for(i=0; i<1000000; ++i){ // Some Delay
//			TIM1->CCR2 = 20;
//		//for(i=0; i<1000; ++i); // Some Delay
//		}
//		for(i=0; i<1000000; ++i){ // Some Delay
//			TIM1->CCR2 = 15;
//		//for(i=0; i<1000; ++i); // Some Delay
//		}
//		for(i=0; i<1000000; ++i){ // Some Delay
//			TIM1->CCR2 = 10;
//		//for(i=0; i<1000; ++i); // Some Delay
//		}
//		for(i=0; i<1000000; ++i){ // Some Delay
//			TIM1->CCR2 = 15;
//		//for(i=0; i<1000; ++i); // Some Delay
//		}
	}
	
	return 0;
}
