/*
 * W1D4 · 编译四阶段，观察 .elf / .map
 * ------------------------------------------------------------------
 * 今天不写业务逻辑，今天是"解剖一个程序的出生过程"。
 *
 * 在 01_c/w1d4 目录下依次执行：
 *
 *   1) 预处理：把 #include / #define 全部展开
 *      gcc -E main.c -o main.i
 *
 *   2) 编译：C 代码 → 汇编
 *      gcc -S main.i -o main.s
 *
 *   3) 汇编：汇编 → 目标文件（机器码，但地址还没定）
 *      gcc -c main.s -o main.o
 *      （util.c 也用同样方式生成 util.o）
 *
 *   4) 链接：把多个 .o 拼成一个可执行文件，顺便生成 .map
 *      gcc main.o util.o -o main.exe -Wl,-Map=main.map
 *
 *   5) 看各段大小
 *      size main.exe
 *
 * 要交的产出：
 *   - notes.md：每一步做了什么、你看到了什么
 *   - main.map 里的 .text / .data / .bss 三行数字
 *   - main.i 里你找到的第一个"函数体"（提示：找 util_add 的声明变成了什么）
 *
 * 收工前回答：
 *   1. 预处理后 #include 变成了什么？#define 去哪了？
 *   2. .text / .data / .bss 分别放什么？为什么 .bss 不占 Flash？
 *   3. .map 文件能帮你解决什么实际问题？
 */

#include <stdio.h>
#include "util.h"

/* 定义一个宏，观察它预处理之后变成什么 */
#define MAGIC_NUMBER 42

/* 有初值的全局变量 → .data */
int g_init_value = MAGIC_NUMBER;

/* 没初值的全局变量 → .bss（不占 Flash，运行时清零） */
int g_zero_value;

/* const 全局数组通常落到只读段 */
const char g_banner[] = "flight controller bring-up";

int main(void)
{
    printf("%s\n", g_banner);
    printf("MAGIC_NUMBER = %d\n", MAGIC_NUMBER);
    printf("util_add(1, 2) = %d\n", util_add(1, 2));
    printf("%s\n", util_version());
    printf("g_init_value = %d, g_zero_value = %d\n", g_init_value, g_zero_value);
    return 0;
}
