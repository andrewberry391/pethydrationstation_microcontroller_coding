/*
 * ECE 153B - Winter 2020
 *
 * Name(s): Alice Patig and Andrew Berry
 * Section:
 * Lab: 1B
 */
#include "stm32l476xx.h"
#include "EXTI.h"
#include "LCD.h"
#include "SysTimer.h"

void EXTI_Init(void) {
	// Initialize GPIO
	GPIOA->MODER &= ~0x00000CFF;
	GPIOA->PUPDR &= ~0x00000766;
	GPIOA->PUPDR |= 0x000008AA;
	
	// Configure SYSCFG EXTI
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	
	// Configure EXTI Trigger
	// Make it trigger on the rising and falling edges
	EXTI->FTSR1 |= EXTI_FTSR1_FT0;
	
	// Enable EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM0;
	
	// Configure and Enable in NVIC
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0); 
}

// Write Interrupt Handlers

int dispensing = 6; // this is 0 degrees 10 / 200 = 0.05
int not_dispensing = 12; // this is 90 degrees 15 / 200 = 0.075
int hold_dispensing = 11; // this is when there is a delay from the UART

void EXTI0_IRQHandler(void) { // When interrupt occurs fill bowl until interrupt stops
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF0;
	// Define behavior that occurs when interrupt occurs
	if(USART1->RDR & 0xFF){
		TIM1->CCR1 = hold_dispensing;
		LCD_DisplayString("HOLD");
	}
	if(TIM1->CCR1 != hold_dispensing) {
		printf("START\n");
		TIM1->CCR1 = dispensing;
		LCD_DisplayString("START");
		for(int i = 0; i < 1000000; i++);
	}
}