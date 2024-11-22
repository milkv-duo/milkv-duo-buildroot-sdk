#include <stdio.h>

#define GPIO_SWPORTA_DR 0x000
#define GPIO_SWPORTA_DDR 0x004

void duo_led_control(int enable);
