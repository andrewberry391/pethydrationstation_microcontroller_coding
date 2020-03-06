#include "UART.h"

void UART1_Init(void) {
	// TODO
	// Enable the USART1 clock in the peripheral clock register
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// Select system clock as the USART1 clock source in peripherals independent clock config. register = 01
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL_1;
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
}

void UART2_Init(void) {
	// TODO
	// Enable the USART2 clock in the peripheral clock register
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	// Select system clock as the USART2 clock source in peripherals independent clock config. register = 01
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL_1;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
}

void UART1_GPIO_Init(void) {
	// TODO Bluetooth
	// Enable GPIO Port B Clock RCC
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	// Set both GPIO pins PB6 and PB7 to alternative function = 10
	// PB6
	GPIOB->MODER |= GPIO_MODER_MODE6_1 ; // 1
	GPIOB->MODER &= ~GPIO_MODER_MODE6_0 ; // 0
	// PB7
	GPIOB->MODER |= GPIO_MODER_MODE7_1 ; // 1
	GPIOB->MODER &= ~GPIO_MODER_MODE7_0 ; // 0
	
	//config. alternative function for PB6 and PB7 = 0111
	// PB6
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6_3; // 0
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2; // 1
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1; // 1
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_0; // 1
	// PB7
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7_3; // 0
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_2; // 1
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_1; // 1
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_0; // 1
	
	// Set both bins to very high speed = 11
	// PB6
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1; // 1
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_0; // 1
	// PB7
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7_1; // 1
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7_0; // 1
	
	// Set both pins to push pull output type = 0
	// PB6
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT6; // 0
	// PB7
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT7; // 0
	// Configure both pins to use pull-up resistors = 01
	// PB6
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6_1; // 0
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0; // 1
	// PB7
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD7_1; // 0
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD7_0; // 1
}

void UART2_GPIO_Init(void) {
	// TODO USB
	// Enable GPIO Port D Clock RCC
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	
	// Set both GPIO pins 5 and 6 to alternative function = 10
	// PD5
	GPIOD->MODER |= GPIO_MODER_MODE5_1 ; // 1
	GPIOD->MODER &= ~GPIO_MODER_MODE5_0 ; // 0
	// PD6
	GPIOD->MODER |= GPIO_MODER_MODE6_1 ; // 1
	GPIOD->MODER &= ~GPIO_MODER_MODE6_0 ; // 0
	
	//config. alternative function for PD5 and PD6 = 0111
	// PD5
	GPIOD->AFR[0] &= ~GPIO_AFRL_AFSEL5_3; // 0
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL5_2; // 1
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL5_1; // 1
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL5_0; // 1
	// PD6
	GPIOD->AFR[0] &= ~GPIO_AFRL_AFSEL6_3; // 0
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL6_2; // 1
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL6_1; // 1
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL6_0; // 1
	
	// Set both bins to very high speed = 11
	// PD5
	GPIOD->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1; // 1
	GPIOD->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_0; // 1
	// PD6
	GPIOD->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1; // 1
	GPIOD->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_0; // 1
	
	// Set both pins to push pull output type = 0
	// PD5
	GPIOD->OTYPER &= ~GPIO_OTYPER_OT5; // 0
	// PD6
	GPIOD->OTYPER &= ~GPIO_OTYPER_OT6; // 0
	// Configure both pins to use pull-up resistors = 01
	// PD5
	GPIOD->PUPDR &= ~GPIO_PUPDR_PUPD5_1; // 0
	GPIOD->PUPDR |= GPIO_PUPDR_PUPD5_0; // 1
	// PD6
	GPIOD->PUPDR &= ~GPIO_PUPDR_PUPD6_1; // 0
	GPIOD->PUPDR |= GPIO_PUPDR_PUPD6_0; // 1
	
}

void USART_Init(USART_TypeDef* USARTx) {
	// TODO
	// Disable USART so we can modify the settings = 0
	USARTx->CR1 &= ~USART_CR1_UE;
	
	// Set the word length to 8 bits = 00
	USARTx->CR1 &= ~USART_CR1_M1; // 0
	USARTx->CR1 &= ~USART_CR1_M0; // 0
	
	// Set oversampling mode to oversampling by 16 = 0
	USARTx->CR1 &= ~USART_CR1_OVER8;
	
	// Set the number of stop bits to 1 = 00
	USARTx->CR2 &= ~USART_CR2_STOP_1; // 0
	USARTx->CR2 &= ~USART_CR2_STOP_0; // 0
	
	// Set the baud rate to 9600 which is calculated by baud rate = fclk/USARTDIV thus USARTDIV = 80MHz/9600 = 8333
	USARTx->BRR = 8333;
	
	// Eanble the transmitter
	USARTx->CR1 |= USART_CR1_TE;
	// Enable the reciever
	USARTx->CR1 |= USART_CR1_RE;
	
	// Enable the USART because were done changing settings
	USARTx->CR1 |= USART_CR1_UE;
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}

void USART_IRQHandler(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t * pRx_counter) {
	if(USARTx->ISR & USART_ISR_RXNE) {						// Received data                         
		buffer[*pRx_counter] = USARTx->RDR;         // Reading USART_DR automatically clears the RXNE flag 
		(*pRx_counter)++;  
		if((*pRx_counter) >= BufferSize )  {
			(*pRx_counter) = 0;
		}   
	} else if(USARTx->ISR & USART_ISR_TXE) {
 		//USARTx->ISR &= ~USART_ISR_TXE;            // clear interrupt 
		//Tx1_Counter++;
	} else if(USARTx->ISR & USART_ISR_ORE) {			// Overrun Error
		while(1);
	} else if(USARTx->ISR & USART_ISR_PE) {				// Parity Error
		while(1);
	} else if(USARTx->ISR & USART_ISR_PE) {				// USART_ISR_FE	
		while(1);
	} else if (USARTx->ISR & USART_ISR_NE){ 			// Noise Error Flag
		while(1);     
	}	
}
