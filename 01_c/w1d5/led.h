#ifndef LED_H
#define LED_H

/*
 * 硬件抽象层（HAL）的接口。
 *
 * 为什么单独抽一层？
 *   命令解析、业务逻辑这些"上层代码"只 include 这个头文件，
 *   不关心 LED 到底是接在 PC 的打印语句上，还是接在 STM32 的 PC13 上。
 *   换硬件时只改 led.c，上层一行都不用动。
 *
 * 这就是飞控固件里"分层"的最小样本：HAL → 驱动 → 算法 → 任务 → 通信。
 */

typedef enum {
    LED_OFF = 0,
    LED_ON  = 1
} led_state_t;

void        led_init(void);
void        led_on(void);
void        led_off(void);
void        led_toggle(void);
led_state_t led_get_state(void);
const char *led_state_str(void);   /* "ON" / "OFF" */

#endif /* LED_H */
