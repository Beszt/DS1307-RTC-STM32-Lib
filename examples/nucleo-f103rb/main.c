#include "stm32f1xx.h"
#include "DS1307.h"

I2C_HandleTypeDef i2c;
UART_HandleTypeDef uart;

void send_char(char c)
{
	HAL_UART_Transmit(&uart, (uint8_t*)&c, 1, 1000);
}

int __io_putchar(int ch)
{
	if (ch == '\n')
		send_char('\r');
	send_char(ch);
	return ch;
}

int main(void)
{
	SystemCoreClock = 8000000; // taktowanie 8Mhz
	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_AF_OD;
	gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7;		// SCL, SDA
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &gpio);

	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_2;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT;
	gpio.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio);

	i2c.Instance             = I2C1;
	i2c.Init.ClockSpeed      = 100000;
	i2c.Init.DutyCycle       = I2C_DUTYCYCLE_2;
	i2c.Init.OwnAddress1     = 0xff;
	i2c.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c.Init.OwnAddress2     = 0xff;
	i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&i2c);

	uart.Instance = USART2;
	uart.Init.BaudRate = 115200;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&uart);

	rtcInit(&i2c);

	char timeBuffer[DS1307_MINIMAL_BUFFER_LENGTH];
	//setDateTime("2019-04-13 18:56:10", DS1307_MINIMAL_BUFFER_LENGTH);
	while (1) {
		getDateTime(&timeBuffer[0], DS1307_MINIMAL_BUFFER_LENGTH);
		printf("%s\n", &timeBuffer[0]);

		//printf("%d\n", getSeconds());
		HAL_Delay(1000);
	}
}
