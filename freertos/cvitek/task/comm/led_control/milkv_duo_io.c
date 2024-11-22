#include "milkv_duo_io.h"

#define GPIO_LED 0x03022000
#define GPIO_LED_NR 24

void duo_led_control(int enable)
{
	*(uint32_t*)(GPIO_LED | GPIO_SWPORTA_DDR) = 1 << GPIO_LED_NR;

	if (enable) {
		*(uint32_t*)(GPIO_LED | GPIO_SWPORTA_DR) = 1 << GPIO_LED_NR;
	} else {
		*(uint32_t*)(GPIO_LED | GPIO_SWPORTA_DR) = 0;
	}
}
