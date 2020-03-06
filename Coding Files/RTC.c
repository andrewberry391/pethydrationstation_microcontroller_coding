/*
 * ECE 153B - Winter 2020
 *
 * Name(s): Andrew Berry, Alice Patig
 * Section: T: 7-10
 * Lab: 2B
 */
 
#include "RTC.h"

// Use the 32.768 kHz low-speed external clock as RTC clock source

// Helper macro to convert a value from 2 digit decimal format to BCD format
// __VALUE__ Byte to be converted
#define __RTC_CONVERT_BIN2BCD(__VALUE__) (uint8_t)((((__VALUE__) / 10) << 4) | ((__VALUE__) % 10))
#define __RTC_CONVERT_BCD2BIN(__VALUE__) (uint8_t)(((uint8_t)((__VALUE__) & (uint8_t)0xF0) >> (uint8_t)0x4) * 10 + ((__VALUE__) & (uint8_t)0x0F))

#define RTC_WEEKDAY_MONDAY              ((uint32_t)0x01) /*!< Monday    */
#define RTC_WEEKDAY_TUESDAY             ((uint32_t)0x02) /*!< Tuesday   */
#define RTC_WEEKDAY_WEDNESDAY           ((uint32_t)0x03) /*!< Wednesday */
#define RTC_WEEKDAY_THURSDAY            ((uint32_t)0x04) /*!< Thrusday  */
#define RTC_WEEKDAY_FRIDAY              ((uint32_t)0x05) /*!< Friday    */
#define RTC_WEEKDAY_SATURDAY            ((uint32_t)0x06) /*!< Saturday  */
#define RTC_WEEKDAY_SUNDAY              ((uint32_t)0x07) /*!< Sunday    */

#define RTC_MONTH_JANUARY               ((uint8_t)0x01)  /*!< January   */
#define RTC_MONTH_FEBRUARY              ((uint8_t)0x02)  /*!< February  */
#define RTC_MONTH_MARCH                 ((uint8_t)0x03)  /*!< March     */
#define RTC_MONTH_APRIL                 ((uint8_t)0x04)  /*!< April     */
#define RTC_MONTH_MAY                   ((uint8_t)0x05)  /*!< May       */
#define RTC_MONTH_JUNE                  ((uint8_t)0x06)  /*!< June      */
#define RTC_MONTH_JULY                  ((uint8_t)0x07)  /*!< July      */
#define RTC_MONTH_AUGUST                ((uint8_t)0x08)  /*!< August    */
#define RTC_MONTH_SEPTEMBER             ((uint8_t)0x09)  /*!< September */
#define RTC_MONTH_OCTOBER               ((uint8_t)0x10)  /*!< October   */
#define RTC_MONTH_NOVEMBER              ((uint8_t)0x11)  /*!< November  */
#define RTC_MONTH_DECEMBER              ((uint8_t)0x12)  /*!< December  */

void RTC_BAK_SetRegister(uint32_t BackupRegister, uint32_t Data) {
	register uint32_t tmp = 0;

	tmp = (uint32_t)(&(RTC->BKP0R));
	tmp += (BackupRegister * 4);

	/* Write the specified register */
	*(__IO uint32_t *)tmp = (uint32_t)Data;
}

void RTC_Init(void) {
	
	/* Enables the PWR Clock and Enables access to the backup domain #######*/
	/* To change the source clock of the RTC feature (LSE, LSI), you have to:
	 - Enable the power clock
	 - Enable write access to configure the RTC clock source (to be done once after reset).
	 - Reset the Back up Domain
	 - Configure the needed RTC clock source */
	RTC_Clock_Init();
	
	// Disable RTC registers write protection
	RTC_Disable_Write_Protection();
	
	// Enter in initialization mode
	RTC->ISR |= RTC_ISR_INIT;
	while( (RTC->ISR & RTC_ISR_INITF) == 0); // Wait until INITF is set after entering initialization mode
	
	// Configure the Date 
	/* Note: __LL_RTC_CONVERT_BIN2BCD helper macro can be used if user wants to */
	/*       provide directly the decimal value:                                */
	RTC_Set_Calendar_Date(RTC_WEEKDAY_WEDNESDAY, 0x15, RTC_MONTH_MAY, 0x73); /* (weekday, day, month, year) STUB (Wednesday February 4, 2020): Fill in current date */
	
	// Configure the Time 
	RTC_Set_Time(RTC_TR_PM, 0x05, 0x17, 0x41); /* (hours, min, sec) STUB (09:34:40 PM): Fill in current time */
  
	// Exit of initialization mode 
	RTC->ISR &= ~RTC_ISR_INIT;
	while((RTC->ISR & RTC_ISR_RSF) == 0); /* Wait for synchro */
                                          /* Note: Needed only if Shadow registers is enabled */
                                          /* LL_RTC_IsShadowRegBypassEnabled function can be used */
	
	// Enable RTC registers write protection 
	RTC_Enable_Write_Protection();

	// Writes a data in a RTC Backup data Register1 
	// to indicate date/time updated 
	RTC_BAK_SetRegister(1, 0x32F2);
}

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))
#define RTC_POSITION_TR_HT    (uint32_t)POSITION_VAL(RTC_TR_HT)
#define RTC_POSITION_TR_HU    (uint32_t)POSITION_VAL(RTC_TR_HU)
#define RTC_POSITION_TR_MT    (uint32_t)POSITION_VAL(RTC_TR_MNT)
#define RTC_POSITION_TR_MU    (uint32_t)POSITION_VAL(RTC_TR_MNU)
#define RTC_POSITION_TR_ST    (uint32_t)POSITION_VAL(RTC_TR_ST)
#define RTC_POSITION_TR_SU    (uint32_t)POSITION_VAL(RTC_TR_SU)
#define RTC_POSITION_DR_YT    (uint32_t)POSITION_VAL(RTC_DR_YT)
#define RTC_POSITION_DR_YU    (uint32_t)POSITION_VAL(RTC_DR_YU)
#define RTC_POSITION_DR_MT    (uint32_t)POSITION_VAL(RTC_DR_MT)
#define RTC_POSITION_DR_MU    (uint32_t)POSITION_VAL(RTC_DR_MU)
#define RTC_POSITION_DR_DT    (uint32_t)POSITION_VAL(RTC_DR_DT)
#define RTC_POSITION_DR_DU    (uint32_t)POSITION_VAL(RTC_DR_DU)
#define RTC_POSITION_DR_WDU   (uint32_t)POSITION_VAL(RTC_DR_WDU)
#define RTC_POSITION_TR_PM		(uint32_t)POSITION_VAL(RTC_TR_PM)

void RTC_Set_Calendar_Date(uint32_t WeekDay, uint32_t Day, uint32_t Month, uint32_t Year) {
	// TODO -- Write the date values in the correct place within the RTC Date Register
	
	// First clear all of the registers so we can mask them with the ones to be set
	RTC->DR &= ~RTC_DR_WDU;
	RTC->DR &= ~RTC_DR_DT;
	RTC->DR &= ~RTC_DR_DU;
	RTC->DR &= ~RTC_DR_MT;
	RTC->DR &= ~RTC_DR_MU;
	RTC->DR &= ~RTC_DR_YT;
	RTC->DR &= ~RTC_DR_YU;
	RTC->DR &= ~RTC_DR_WDU;
	RTC->DR &= ~RTC_DR_WDU;
	
	// Temporary variable we will mask with WeekDay, Day, Month, and Year to set equal to the register RTC->DR
	uint32_t rtc_dr = 0;
	// Then set the Weekday
	rtc_dr |= (WeekDay)<<(RTC_POSITION_DR_WDU);
	
	// Then set the Day
	rtc_dr |= (Day)<<(RTC_POSITION_DR_DU);
	
	// Then set the Month
	rtc_dr |= (Month)<<(RTC_POSITION_DR_MU);
	
	// Lastly, set the Year
	rtc_dr |= (Year)<<(RTC_POSITION_DR_YU);
	
	// Now set the register equal to our temp rtc_dr
	RTC->DR = rtc_dr;
}

void RTC_Set_Time(uint32_t Format12_24, uint32_t Hour, uint32_t Minute, uint32_t Second) {
	// TODO -- Write the time values in the correct place within the RTC Time Register
	
	// First clear all of the registers so we can mask them with the ones to be set
	RTC->TR &= ~RTC_TR_PM;
	RTC->TR &= ~RTC_TR_HT;
	RTC->TR &= ~RTC_TR_HU;
	RTC->TR &= ~RTC_TR_MNT;
	RTC->TR &= ~RTC_TR_MNU;
	RTC->TR &= ~RTC_TR_ST;
	RTC->TR &= ~RTC_TR_SU;
	
	// Temporary variable we will mask with Format12_24, Hour, Minute, and Second to set equal to the register RTC->TR
	uint32_t rtc_tr = 0;
	// Then set the Format12_24
	rtc_tr |= (Format12_24)<<(RTC_POSITION_TR_PM);
	
	// Then set the Hour
	rtc_tr |= (Hour)<<(RTC_POSITION_TR_HU);
	
	// Then set the Minute
	rtc_tr |= (Minute)<<(RTC_POSITION_TR_MU);
	
	// Lastly, set the Second
	rtc_tr |= (Second)<<(RTC_POSITION_TR_SU);
	
	// Now set the register equal to our temp rtc_tr
	RTC->TR = rtc_tr;
}

void RTC_Clock_Init(void) {
	
	// Enable write access to Backup domain
	if ( (RCC->APB1ENR1 & RCC_APB1ENR1_PWREN) == 0 ) {
		RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;	// Power interface clock enable
		(void) RCC->APB1ENR1;  // Delay after an RCC peripheral clock enabling
	}
	
	// Select LSE as RTC clock source 
	if ( (PWR->CR1 & PWR_CR1_DBP) == 0) {
		PWR->CR1  |= PWR_CR1_DBP;				  			// Enable write access to Backup domain
		while((PWR->CR1 & PWR_CR1_DBP) == 0);  	// Wait for Backup domain Write protection disable
	}
	
	// Reset LSEON and LSEBYP bits before configuring the LSE
	RCC->BDCR &= ~(RCC_BDCR_LSEON | RCC_BDCR_LSEBYP);

	// RTC Clock selection can be changed only if the Backup Domain is reset
	RCC->BDCR |=  RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;
	
	// Note from STM32L4 Reference Manual: 	
	// RTC/LCD Clock:  (1) LSE is in the Backup domain. (2) HSE and LSI are not.	
	while((RCC->BDCR & RCC_BDCR_LSERDY) == 0){  // Wait until LSE clock ready
		RCC->BDCR |= RCC_BDCR_LSEON;
	}
	
	// Select LSE as RTC clock source
	// BDCR = Backup Domain Control Register 
	RCC->BDCR	&= ~RCC_BDCR_RTCSEL;	  // RTCSEL[1:0]: 00 = No Clock, 01 = LSE, 10 = LSI, 11 = HSE
	RCC->BDCR	|= RCC_BDCR_RTCSEL_0;   // Select LSE as RTC clock	
	
	RCC->BDCR |= RCC_BDCR_RTCEN;      // Enable RTC
	
	RCC->APB1ENR1 &= ~RCC_APB1ENR1_PWREN;	// Power interface clock disable
}

void RTC_Disable_Write_Protection(void) {
	// TODO
	// To disable write protection sequentially write 0xCA and 0x53 to the WPR register.
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
}
	
void RTC_Enable_Write_Protection(void) {
	// TODO
	// To enable write protection we reverse what we did to disable but any hex would do the trick
	RTC->WPR = ~0xCA;
	RTC->WPR = ~0x53;
}

uint32_t RTC_TIME_GetHour(void) {
	// TODO
  uint32_t hour; // this is the whole hour value we are setting un BCD format to be returned
	int hour_tens; // this is the tens place value
	int hour_units; // this is the units(ones) place value
	
	hour_tens = ((RTC->TR)&RTC_TR_HT)>>(RTC_POSITION_TR_HT - 4); // This uses the RTC_TR_HT macro to get only the hour's tens place by anding it and
																															 // then shifting it by the position it is in, RTC_POSITION_TR_HT - 4, to make it in the tens place (LSB+4) to be returned
	hour_units = ((RTC->TR)&RTC_TR_HU)>>(RTC_POSITION_TR_HU); // This uses the RTC_TR_HU macro to get only the hour's units place from the whole RTC->TR register
																													  // by anding it and then shifting it by, RTC_POSITION_TR_HU, its postion in the register to get it to the LSB
	hour = hour_tens + hour_units;
	return hour;
}

uint32_t RTC_TIME_GetMinute(void) {
	// TODO
	uint32_t minutes; // this is the whole minutes value we are setting un BCD format to be returned
	int minute_tens; // this is the tens place value
	int minutes_units; // this is the units(ones) place value
	
	minute_tens = ((RTC->TR)&RTC_TR_MNT)>>(RTC_POSITION_TR_MT - 4); // This uses the RTC_TR_MNT macro to get only the minutes's tens place by anding it and
																															 // then shifting it by the position it is in, RTC_POSITION_TR_MT - 4, to make it in the tens place (LSB+4) to be returned
	minutes_units = ((RTC->TR)&RTC_TR_MNU)>>(RTC_POSITION_TR_MU); // This uses the RTC_TR_MNU macro to get only the minutes's units place from the whole RTC->TR register
																													  // by anding it and then shifting it by, RTC_POSITION_TR_MU, its postion in the register to get it to the LSB
	minutes = minute_tens + minutes_units;
	return minutes;
}

uint32_t RTC_TIME_GetSecond(void) {
	// TODO
	uint32_t second; // this is the whole second value we are setting un BCD format to be returned
	int second_tens; // this is the tens place value
	int second_units; // this is the units(ones) place value
	
	second_tens = ((RTC->TR)&RTC_TR_ST)>>(RTC_POSITION_TR_ST - 4); // This uses the RTC_TR_ST macro to get only the second's tens place by anding it and
																															 // then shifting it by the position it is in, RTC_POSITION_TR_ST - 4, to make it in the tens place (LSB+4) to be returned
	second_units = ((RTC->TR)&RTC_TR_SU)>>(RTC_POSITION_TR_SU); // This uses the RTC_TR_SU macro to get only the second's units place from the whole RTC->TR register
																													  // by anding it and then shifting it by, RTC_POSITION_TR_SU, its postion in the register to get it to the LSB
	second = second_tens + second_units;
	return second;
	
}

uint32_t RTC_DATE_GetMonth(void) {
	// TODO
	uint32_t month; // this is the whole month value we are setting un BCD format to be returned
	int month_tens; // this is the tens place value
	int month_units; // this is the units(ones) place value
	
	month_tens = ((RTC->DR)&RTC_DR_MT)>>(RTC_POSITION_DR_MT - 4); // This uses the RTC_DR_MT macro to get only the month's tens place by anding it and
																																// then shifting it by the position it is in, RTC_POSITION_DR_MT - 4, to make it in the tens place (LSB+4) to be returned
	month_units = ((RTC->DR)&RTC_DR_MU)>>(RTC_POSITION_DR_MU); // This uses the RTC_DR_MU macro to get only the month's units place from the whole RTC->DR register
																														 // by anding it and then shifting it by, RTC_POSITION_DR_MU, its postion in the register to get it to the LSB
	month = month_tens + month_units;
	return month;
}

uint32_t RTC_DATE_GetDay(void) {
	// TODO
	uint32_t day; // this is the whole day value we are setting un BDC format to be returned
	int day_tens; // this is the tens place value
	int day_units; // this is the units(ones) place value
	
	day_tens = ((RTC->DR)&RTC_DR_DT)>>(RTC_POSITION_DR_DT - 4); // This uses the RTC_DR_DT macro to get only the day's tens place by anding it and
																																// then shifting it by the position it is in, RTC_POSITION_DR_DT - 4, to make it in the tens place (LSB+4) to be returned
	day_units = ((RTC->DR)&RTC_DR_DU)>>(RTC_POSITION_DR_DU); // This uses the RTC_DR_DU macro to get only the day's units place from the whole RTC->DR register
																														 // by anding it and then shifting it by, RTC_POSITION_DR_DU, its postion in the register to get it to the LSB
	day = day_tens + day_units;
	return day;
}

uint32_t RTC_DATE_GetYear(void) {
	// TODO
	uint32_t year; // this is the whole year value we are setting un BDC format to be returned
	int year_tens; // this is the tens place value
	int year_units; // this is the units(ones) place value
	
	year_tens = ((RTC->DR)&RTC_DR_YT)>>(RTC_POSITION_DR_YT-4); // This uses the RTC_DR_YU macro to get only the year's tens place by anding it and
																																// then shifting it by the position it is in, RTC_POSITION_DR_YT - 4, to make it in the tens place (LSB+4) to be returned
	year_units = ((RTC->DR)&RTC_DR_YU)>>(RTC_POSITION_DR_YU); // This uses the RTC_DR_YU macro to get only the year's units place from the whole RTC->DR register
																														 // by anding it and then shifting it by, RTC_POSITION_DR_YU, its postion in the register to get it to the LSB
	year = year_tens + year_units;
	return year;
}

uint32_t RTC_DATE_GetWeekDay(void) {
	// TODO
	uint32_t weekday; // this is the whole weekday value we are setting un BDC format to be returned
	int weekday_units; // this is the whole weekday value place value. Weekday is represtend in one 3 bit number for Monday through Sunday
	
	weekday_units = ((RTC->DR)&RTC_DR_WDU)>>(RTC_POSITION_DR_WDU); // This uses the RTC_DR_WDU macro to get only the wekday units by anding it and
																																// then shifting it by, RTC_POSITION_DR_WDU, the position to make it in the LSB to be returned
	weekday = weekday_units;
	return weekday;
}

void Get_RTC_Calendar(char * strTime, char * strDate) {
	/* Note: need to convert in decimal value in using __LL_RTC_CONVERT_BCD2BIN helper macro */
	/* Display time Format : hh:mm:ss */
	sprintf((char*)strTime,"%.2d:%.2d:%.2d", 
		__RTC_CONVERT_BCD2BIN(RTC_TIME_GetHour()), 
		__RTC_CONVERT_BCD2BIN(RTC_TIME_GetMinute()), 
		__RTC_CONVERT_BCD2BIN(RTC_TIME_GetSecond()));
	/* Display date Format : mm-dd-yy */
	sprintf((char*)strDate,"%.2d-%.2d-%.2d", 
		__RTC_CONVERT_BCD2BIN(RTC_DATE_GetMonth()), 
		__RTC_CONVERT_BCD2BIN(RTC_DATE_GetDay()), 
		2000 + __RTC_CONVERT_BCD2BIN(RTC_DATE_GetYear()));
}
