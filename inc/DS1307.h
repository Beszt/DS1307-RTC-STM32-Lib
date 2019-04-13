/**
 * HAL STM32 Library for RTC modules based on DS1307 chip.
 * Compatible with all Nucleo boards and can be easly port
 * to another devices.
 *
 * Required Uc functions: HAL_I2C_Mem_Read, HAL_I2C_Mem_Write.
 *
 *  Created on: 05.04.2019
 *  Author: Maciej Obarzanek
 */

#ifndef DS1307_H_
#define DS1307_H_

#include "stm32f1xx.h"
#include <stdlib.h>
#include <string.h>

/**
 * Main config.
 */
#define DS1307_ADDRESS 0xD0
#define DS1307_MINIMAL_BUFFER_LENGTH 19

/**
 * Registers addresses.
 */
#define DS1307_REGISTER_SECOND 0x00
#define DS1307_REGISTER_MINUTE 0x01
#define DS1307_REGISTER_HOUR 0x02
#define DS1307_REGISTER_DAY_OF_WEEK 0x03
#define DS1307_REGISTER_DAY_OF_MONTH 0x04
#define DS1307_REGISTER_MONTH 0x05
#define DS1307_REGISTER_YEAR 0x06
#define DS1307_REGISTER_CONTROL 0x07
#define DS1307_REGISTER_RAM_START 0x08
#define DS1307_REGISTER_RAM_END 0x3F

/**
 * Registers masks. All time data are stored in BCD.
 * Example 21 seconds are stored like:
 * DS1307_MASK_SECOND = 1 and DS1307_MASK_SECOND_10 = 2.
 */
#define DS1307_MASK_SECOND 0b01111111
#define DS1307_MASK_MINUTE 0b01111111
#define DS1307_MASK_HOUR 0b00111111
#define DS1307_MASK_DAY_OF_WEEK 0b00000111
#define DS1307_MASK_DAY_OF_MONTH 0b00111111
#define DS1307_MASK_MONTH 0b00011111
#define DS1307_MASK_YEAR 0b11111111

/**
 * Clock Halt (CH) - register DS1307_REGISTER_SECOND (0x00).
 * When this bit is set to 1 RTC was halt - default state.
 */
#define DS1307_FLAG_0X00_CH 0b1000000
/**
 * 12/24 mode - register DS1307_REGISTER_HOUR (0x02)
 */
#define DS1307_FLAG_0X02_24 0b0100000
/*
 * Bit sets enable / disable square wave output.
 */
#define DS1307_FLAG_SQWE 0b00010000

/**
 * Values of control register that config and enable
 * square wave generator. Freqs:
 *
 * DS1307_SET_SQ_1_HZ - 1Hz
 * DS1307_SET_SQ_4_KHZ - 4.096kHz
 * DS1307_SET_SQ_8_KHZ - 8.192kHz
 * DS1307_SET_SQ_32_KHZ - 32.768kHz
 */
#define DS1307_SQW_1_HZ 0b00010000
#define DS1307_SQW_4_KHZ 0b00010001
#define DS1307_SQW_8_KHZ 0b00010010
#define DS1307_SQW_32_KHZ 0b00010011

/*
 * Function that initialize RTC module. Must be use at first!
 *
 * Params:
 * @*i2c - Pointer to i2c interface.
 *
 * Return values:
 * 1 - OK.
 * 0 - i2c interface pointer is NULL.
 */

uint8_t rtcInit(I2C_HandleTypeDef *i2c);

/**
 * Function to decode BCD from register. Return pointer to declared buffer.
 *
 * Params:
 * @byte - BCD register to convert
 * @*buffer - Pointer to string buffer where the function will put result.
 *
 * Return value: self-pointer to buffer.
 */
char *BcdByteToStr(uint8_t byte, char *buffer);

/*
 * Function to decode String to BCD byte. Return byte in BCD.
 *
 * Params:
 * *buffer - Pointer to string to convert. String must be in decimal base.
 *
 * Return value: converted string to BCD RTC register.
 */
uint8_t StrToBcdByte(char *buffer);

/**
  * Function to get data and time from RTC and puts it
  * together to char array in format: YYYY-MM-DD HH:MM:SS.
  *
  * Params:
  * @buffer - A pointer to the memory address where the date
  * 	and time will be placed.
  * @bufferLength - Reserved buffer length in bytes.
  * 	Minimum value is 20.
  *
  * Return values:
  * 0 - OK.
  * 1 - Buffer for date & time (DS1307_MINIMAL_BUFFER_LENGTH)
  * 	is too small.
*/
uint8_t getDateTime(char *buffer, uint8_t bufferLength);

/**
  * Function to set data and time from RTC and sends it
  * to RTC. Params:
  *
  * @buffer - A pointer to the memory address where the date
  * and time will be placed. Parameter must be in format:
  * YYYY-MM-DD HH:MM:SS
  *
  * @bufferLength - Reserved buffer length in bytes.
  * Minimum value is 20.
  *
  * Return values:
  * 0 - OK.
  * 1 - Buffer for date & time (DS1307_MINIMAL_BUFFER_LENGTH)
  * 	is too small.
*/
uint8_t setDateTime(char *buffer, uint8_t bufferLength);

/**
 * Struct that contains basic elements of date and time.
 * Data is stored in two variants: normal and BCD values
 * (for RTC registers).
 */
struct Rtc {
	uint16_t year;
	uint8_t month;
	uint8_t dayOfMonth;
	uint8_t dayOfWeek;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;

	uint16_t yearBcd;
	uint8_t monthBcd;
	uint8_t dayOfMonthBcd;
	uint8_t dayOfWeekBcd;
	uint8_t hourBcd;
	uint8_t minuteBcd;
	uint8_t secondBcd;

};

#endif /* DS1307_H_ */
