# DS1307 library for STM32

Simple library for DS1307 Real Time Clock chip with i2c interface. Compatible with HAL - tested on Nucleo-F103RB and Tiny RTC i2c modules.

## Quick start

1. Inlude "DS1307.h"
2. Initialize lib with:

```
rtcInit(&i2c);
```

3. Make the space-time curvature!

Where &i2c is address to your i2c I2C_HandleTypeDef struct.

## Functions
1. Setting time in format YYYY-MM-DD HH:MM:SS:

```
setDateTime(char *buffer, uint8_t bufferLength)
```

Params:
* *buffer - pointer to array that contains date & time.
* BufferLength - Lentgth of buffer (without /0).

Example:
```
setDateTime("2019-04-13 18:56:10", DS1307_MINIMAL_BUFFER_LENGTH)
```

2. Getting time in format YYYY-MM-DD HH:MM:SS:

```
getDateTime(char *buffer, uint8_t bufferLength)
```

Params:
* *buffer - pointer to array where function will put date & time.
* BufferLength - Lentgth of buffer (without /0).

Example:

```
getDateTime(&timeBuffer[0], DS1307_MINIMAL_BUFFER_LENGTH);
```

## Additional info

You can also get the all individual elements of date & time from predefined rtc struct. Example:

```
uint8_t seconds = rtc.seconds;
```

Rtc struct definition:

```
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
```

For more addidtional info check the comments in the code!