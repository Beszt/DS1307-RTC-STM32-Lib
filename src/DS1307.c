/*
 * DS1307.c
 *
 *  Created on: 05.04.2019
 *  Author: Maciej Obarzanek
 */

#include "DS1307.h"

struct Rtc rtc;
I2C_HandleTypeDef *i2cHandle;
char bufferBcd[2] = "";

uint8_t rtcInit(I2C_HandleTypeDef *i2c) {
	if (i2c == NULL) {
		return 1;
	}

	i2cHandle = i2c;
	return 0;
}

char *BcdByteToStr(uint8_t byte, char *buffer) {

	//if (strnlen(&buffer[0]) >= 2) {
		buffer[0] = (byte >> 4) + 0x30;
		buffer[1] = (byte & 0b00001111) + 0x30;
		buffer[2] = '\0';
	//}

	return &buffer[0];
}

uint8_t StrToBcdByte(char *buffer) {

	//if (strlen(&buffer[0]) < 2){
		//return 0;
	//}

	unsigned char bcdByteOld = buffer[0] - 0x30;
	bcdByteOld = bcdByteOld << 4;

	unsigned char bcdByteYoung = buffer[1] - 0x30;

	return bcdByteOld | bcdByteYoung;
}

uint8_t getDateTime(char *buffer, uint8_t bufferLength) {

	if (bufferLength < DS1307_MINIMAL_BUFFER_LENGTH) {
		return 1;
	}

	memset(buffer,0,bufferLength);

	uint8_t rtcRegisterData = 0b00000000;

	HAL_I2C_Mem_Read(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_YEAR, 1, (uint8_t*)&rtcRegisterData, sizeof(rtcRegisterData), HAL_MAX_DELAY);
	rtcRegisterData &= DS1307_MASK_YEAR;
	strcat(buffer, (char*)&"2");
	strcat(buffer, (char*)&"0");
	strcat(buffer, BcdByteToStr(rtcRegisterData, &bufferBcd[0]));
	strcat(buffer, (char*)&"-");
	rtc.year = 2000 + atoi(&bufferBcd[0]);

	HAL_I2C_Mem_Read(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_MONTH, 1, (uint8_t*)&rtcRegisterData, sizeof(rtcRegisterData), HAL_MAX_DELAY);
	rtcRegisterData &= DS1307_MASK_MONTH;
	strcat(buffer, BcdByteToStr(rtcRegisterData, &bufferBcd[0]));
	strcat(buffer, (char*)&"-");
	rtc.month = atoi(&bufferBcd[0]);

	HAL_I2C_Mem_Read(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_DAY_OF_MONTH, 1, (uint8_t*)&rtcRegisterData, sizeof(rtcRegisterData), HAL_MAX_DELAY);
	rtcRegisterData &= DS1307_MASK_DAY_OF_MONTH;
	strcat(buffer, BcdByteToStr(rtcRegisterData, &bufferBcd[0]));
	strcat(buffer, (char*)&" ");
	rtc.dayOfMonth = atoi(&bufferBcd[0]);

	HAL_I2C_Mem_Read(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_HOUR, 1, (uint8_t*)&rtcRegisterData, sizeof(rtcRegisterData), HAL_MAX_DELAY);
	rtcRegisterData &= DS1307_MASK_HOUR;
	strcat(buffer, BcdByteToStr(rtcRegisterData, &bufferBcd[0]));
	strcat(buffer, (char*)&":");
	rtc.hour = atoi(&bufferBcd[0]);

	HAL_I2C_Mem_Read(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_MINUTE, 1, (uint8_t*)&rtcRegisterData, sizeof(rtcRegisterData), HAL_MAX_DELAY);
	rtcRegisterData &= DS1307_MASK_MINUTE;
	strcat(buffer, BcdByteToStr(rtcRegisterData, &bufferBcd[0]));
	strcat(buffer, (char*)&":");
	rtc.minute = atoi(&bufferBcd[0]);

	HAL_I2C_Mem_Read(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_SECOND, 1, (uint8_t*)&rtcRegisterData, sizeof(rtcRegisterData), HAL_MAX_DELAY);
	rtcRegisterData &= DS1307_MASK_SECOND;
	strcat(buffer, BcdByteToStr(rtcRegisterData, &bufferBcd[0]));
	rtc.second = atoi(&bufferBcd[0]);

	return 0;
}

uint8_t setDateTime(char *buffer, uint8_t bufferLength){

	char strTemp[5] = "";

	if (bufferLength < DS1307_MINIMAL_BUFFER_LENGTH) {
		return 1;
	}

	strncpy(&strTemp[0], &buffer[0], 4);
	rtc.year = 2000 + atoi(&strTemp[0]);
	rtc.yearBcd = StrToBcdByte(&strTemp[2]) & DS1307_MASK_YEAR;


	strncpy(&strTemp[0], &buffer[5], 2);
	rtc.month = atoi(&strTemp[0]);
	rtc.monthBcd = StrToBcdByte(&strTemp[0]) & DS1307_MASK_MONTH;

	strncpy(&strTemp[0], &buffer[8], 2);
	rtc.dayOfMonth = atoi(&strTemp[0]);
	rtc.dayOfMonthBcd = StrToBcdByte(&strTemp[0]) & DS1307_MASK_DAY_OF_MONTH;

	strncpy(&strTemp[0], &buffer[11], 2);
	rtc.hour = atoi(&strTemp[0]);
	rtc.hourBcd = StrToBcdByte(&strTemp[0]) & DS1307_MASK_HOUR;

	strncpy(&strTemp[0], &buffer[14], 2);
	rtc.minute = atoi(&strTemp[0]);
	rtc.minuteBcd = StrToBcdByte(&strTemp[0]) & DS1307_MASK_MINUTE;

	strncpy(&strTemp[0], &buffer[17], 2);
	rtc.second = atoi(&strTemp[0]);
	rtc.secondBcd = StrToBcdByte(&strTemp[0]) & DS1307_MASK_SECOND;

	HAL_I2C_Mem_Write(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_YEAR, 1, (uint8_t*)&rtc.yearBcd, sizeof(rtc.year), HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_MONTH, 1, (uint8_t*)&rtc.monthBcd, sizeof(rtc.month), HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_DAY_OF_MONTH, 1, (uint8_t*)&rtc.dayOfMonthBcd, sizeof(rtc.dayOfMonth), HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_HOUR, 1, (uint8_t*)&rtc.hourBcd, sizeof(rtc.hour), HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_MINUTE, 1, (uint8_t*)&rtc.minuteBcd, sizeof(rtc.minute), HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(i2cHandle, DS1307_ADDRESS, DS1307_REGISTER_SECOND, 1, (uint8_t*)&rtc.secondBcd, sizeof(rtc.second), HAL_MAX_DELAY);

	return 0;
}
