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

//static void Delay(uint32_t delay) {
//	while ((delay - t) < bekle);
//}


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
	PWM_Init();
	
	printf("TEST\n");
	
	int i;
	char message[6];
	uint8_t SlaveAddress = 0x36 << 1; // Address of the slave sensor I2C
	uint8_t SEESAW_TOUCH_BASE = 0x0F;
	uint8_t SEESAW_TOUCH_CHANNEL_OFFSET = 0x10;
	while(1) {
		// // TODO (changing duty cycle, etc.)
		// // Change CCR1 to decide how long the pulse is high for 1ms 0degrees, 1.5ms 90degrees, 2ms is 180degrees
		// uint8_t data_send[2] = {SEESAW_TOUCH_BASE, SEESAW_TOUCH_CHANNEL_OFFSET};
		// //uint8_t data_send = SEESAW_TOUCH_BASE | SEESAW_TOUCH_CHANNEL_OFFSET;
		// uint8_t data_recieved[2];
		// uint16_t ret;
		// //do {
		// 	//for(i = 0; i < 1000; ++i); // Some Delay
		// 	I2C_SendData(I2C1, SlaveAddress, data_send, 2);
		
		// 	//for(i = 0; i < 1000; ++i); // Some Delay
		// 	I2C_ReceiveData(I2C1, SlaveAddress, data_recieved, 2);
			
		// 	ret = ((uint16_t)data_recieved[0]<<8) | data_recieved[1];
		// //}while (ret == 65535);
		
		// sprintf(message, "%6d\n", ret);
		// LCD_DisplayString((uint8_t*) message);
		// //printf("Sensed: %d\n", ret);
		
		// for(i = 0; i < 500; ++i); // Some Delay
		int open = 6; // this is 0 degrees 10 / 200 = 0.05
		int closed = 12; // this is 90 degrees 15 / 200 = 0.075
		LCD_DisplayString("OPEN  ");
		TIM1->CCR1 = open;
		for(int i=0; i<10000000; ++i);
		LCD_DisplayString("CLOSED");
		TIM1->CCR1 = closed;
		for(int i=0; i<10000000; ++i);
	}
	
	return 0;
}
