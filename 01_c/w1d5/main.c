/*
 * W1D5 · 小项目：串口命令行控制 LED
 * ------------------------------------------------------------------
 * 今天把三样东西拼起来：
 *   W1D3 的命令表解析器  +  今天的 LED 硬件抽象层  +  输入通道
 *
 * 输入通道今天先用 stdin 模拟"串口"：
 *   键盘输入 = 串口收到命令，printf = 串口发出去
 * 等你有板子了，只需要把"从 stdin 读"换成"从 UART 读"，其他都不动。
 *
 * 要填的 TODO：
 *   TODO 1  split_args()   —— 直接从 W1D3 复制过来，不用重写
 *   TODO 2  dispatch()     —— 同上
 *   TODO 3  cmd_status()   —— 打印当前 LED 状态
 *   TODO 4  cmd_blink()    —— 解析次数参数，闪烁 N 次
 *   TODO 5  main() 里的输入循环 —— 或在 W1D3 基础上改
 *
 * 小提示：在 TODO 5 做完之前，编译会冒出几条 "defined but not used" 警告，
 *         因为命令表还没被主循环调用。做完就没了，不用管它。
 *
 * 编译运行（注意要带上 led.c）：
 *   gcc -std=c11 -Wall -Wextra -g main.c led.c -o w1d5.exe
 *   .\w1d5.exe
 *
 * 要交的产出：
 *   - 能跑的程序 + 一次演示录屏或终端截图（存 07_interview/evidence/）
 *   - notes.md：说明"如果换成真实串口，改哪几个文件、为什么不是全部"
 *
 * 收工前回答：
 *   1. 为什么核心逻辑没有依赖具体硬件？
 *   2. 命令参数合法性检查，应该放在解析层还是处理函数里？
 *   3. 这个结构和飞控固件里的调试命令是什么关系？
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "led.h"

#define MAX_ARGS 8
#define MAX_LINE 128

typedef void (*cmd_handler_t)(int argc, char **argv);

typedef struct {
    const char   *name;
    cmd_handler_t handler;
    const char   *help;
} command_t;

static void cmd_help(int argc, char **argv);
static void cmd_led(int argc, char **argv);
static void cmd_status(int argc, char **argv);
static void cmd_blink(int argc, char **argv);

static const command_t g_cmds[] = {
    {"help",   cmd_help,   "help              列出所有命令"},
    {"led",    cmd_led,    "led on|off|toggle 控制 LED"},
    {"status", cmd_status, "status            查看 LED 当前状态"},
    {"blink",  cmd_blink,  "blink <次数>      闪烁 N 次（1~10）"},
};

static const int g_cmd_count = (int)(sizeof(g_cmds) / sizeof(g_cmds[0]));

/* TODO 1：从 W1D3 复制你的实现过来 */
static int split_args(char *line, char **argv, int max_args)
{
    (void)line;
    (void)argv;
    (void)max_args;
    return 0; /* TODO: 改掉 */
}

/* TODO 2：从 W1D3 复制你的实现过来 */
static int dispatch(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return -1; /* TODO: 改掉 */
}

static void cmd_help(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("可用命令：\n");
    for (int i = 0; i < g_cmd_count; i++) {
        printf("  %s\n", g_cmds[i].help);
    }
}

/* 已给你写好，作为 cmd_status / cmd_blink 的参考样式 */
static void cmd_led(int argc, char **argv)
{
    if (argc < 2) {
        printf("用法: led on|off|toggle\n");
        return;
    }

    if (strcmp(argv[1], "on") == 0) {
        led_on();
    } else if (strcmp(argv[1], "off") == 0) {
        led_off();
    } else if (strcmp(argv[1], "toggle") == 0) {
        led_toggle();
    } else {
        printf("用法: led on|off|toggle\n");
        return;
    }

    printf("LED: %s\n", led_state_str());
}

/* TODO 3：打印一行，例如  LED: ON */
static void cmd_status(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    /* TODO: 改掉 */
}

/* TODO 4：blink <次数>
 *   要求：
 *     - 参数缺失或不是数字 → 打印 用法: blink <次数>
 *     - 次数限定 1~10，越界 → 打印 次数范围是 1~10
 *     - 循环 N 次：开 → 关，每次打印当前状态
 *   提示：atoi() 可以把字符串转成数字；
 *        在真板子上这里会换成 HAL_Delay(200)。
 */
static void cmd_blink(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    /* TODO: 改掉 */
}

int main(void)
{
    char  line[MAX_LINE];
    char *argv[MAX_ARGS];

    (void)line;   /* 占位：TODO 5 做完之后可以删掉 */
    (void)argv;

    led_init();
    printf("=== W1D5 串口命令行控制 LED（stdin 模拟串口）===\n");
    printf("输入 help 看命令，输入 quit 退出。\n\n");

    /* TODO 5：照着 W1D3 的 main() 写输入循环
     *   - fgets 读一行 → 去掉换行 → split_args → 处理 quit → dispatch
     */

    return 0;
}
