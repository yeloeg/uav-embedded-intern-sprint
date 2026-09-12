/*
 * LED 的"模拟实现"——不需要硬件，跑在 PC 上。
 *
 * 换成真实硬件时，只改这个文件：
 *
 *   #include "stm32f4xx_hal.h"
 *   #define LED_GPIO_PORT  GPIOC
 *   #define LED_GPIO_PIN   GPIO_PIN_13
 *
 *   void led_init(void)
 *   {
 *       __HAL_RCC_GPIOC_CLK_ENABLE();
 *       GPIO_InitTypeDef gpio = {0};
 *       gpio.Pin   = LED_GPIO_PIN;
 *       gpio.Mode  = GPIO_MODE_OUTPUT_PP;
 *       gpio.Pull  = GPIO_NOPULL;
 *       gpio.Speed = GPIO_SPEED_FREQ_LOW;
 *       HAL_GPIO_Init(LED_GPIO_PORT, &gpio);
 *       led_off();
 *   }
 *
 *   void led_on(void)  { HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET); }
 *   void led_off(void) { HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);   }
 *   （板子上的 LED 一般是低电平点亮，接反了就先确认电路）
 *
 * 注意：上层代码（main.c）完全不知道这里换了实现。
 */

#include "led.h"
#include <stdio.h>

static led_state_t s_state = LED_OFF;

void led_init(void)
{
    s_state = LED_OFF;
    printf("[LED] 初始化完成（当前为模拟实现，未接硬件）\n");
}

void led_on(void)
{
    s_state = LED_ON;
    printf("[LED] ON\n");
}

void led_off(void)
{
    s_state = LED_OFF;
    printf("[LED] OFF\n");
}

void led_toggle(void)
{
    if (s_state == LED_ON) {
        led_off();
    } else {
        led_on();
    }
}

led_state_t led_get_state(void)
{
    return s_state;
}

const char *led_state_str(void)
{
    return (s_state == LED_ON) ? "ON" : "OFF";
}
