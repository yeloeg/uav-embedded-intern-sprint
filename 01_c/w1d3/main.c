/*
 * W1D3 · 函数指针 + 命令表，实现命令行解析
 * ------------------------------------------------------------------
 * 今天的目标：写一个"表驱动"的命令行解析器。
 *   输入: led on 3
 *   流程: 切成 {"led","on","3"} → 在命令表里查 "led" → 调用它的 handler
 *
 * 要填的 TODO：
 *   TODO 1  split_args()   切词
 *   TODO 2  dispatch()     查表 + 分发 + 未知命令提示
 *   TODO 3  cmd_echo()     回显参数
 *   TODO 4  cmd_led()      解析 on / off / toggle 参数
 *   TODO 5  cmd_sys()      打印系统信息
 *
 * 编译运行：
 *   gcc -std=c11 -Wall -Wextra -g main.c -o w1d3.exe
 *   .\w1d3.exe
 *
 * 收工前回答：
 *   1. 用函数指针表分发，比一长串 if / else if 好在哪？
 *   2. int (*f)(int, char**) 这种声明，怎么一眼读出它是函数指针？
 *   3. 想让 quit 和 exit 等价，命令表要怎么改？
 */

#include <stdio.h>
#include <string.h>

#define MAX_ARGS 8
#define MAX_LINE 128

typedef void (*cmd_handler_t)(int argc, char **argv);

typedef struct {
    const char  *name;
    cmd_handler_t handler;
    const char  *help;
} command_t;

/* 前置声明（因为命令表里要用到这些函数） */
static void cmd_help(int argc, char **argv);
static void cmd_echo(int argc, char **argv);
static void cmd_led(int argc, char **argv);
static void cmd_sys(int argc, char **argv);

/* 命令表：想加命令，只需要在这里加一行，分发逻辑一行都不用改 */
static const command_t g_cmds[] = {
    {"help", cmd_help, "help              列出所有命令"},
    {"echo", cmd_echo, "echo <文本...>    原样回显，用来验证切词"},
    {"led",  cmd_led,  "led on|off|toggle 控制 LED"},
    {"sys",  cmd_sys,  "sys               打印系统信息"},
};

static const int g_cmd_count = (int)(sizeof(g_cmds) / sizeof(g_cmds[0]));

/* LED 状态先用一个变量模拟，第 W1D5 天会换成真正的硬件抽象层 */
static int g_led_on = 0;

/* TODO 1：把一行输入切成参数，返回参数个数
 *   要求：
 *     - 用空格分隔，连续空格当成一个
 *     - 直接在这行字符串上打 '\0'，把 argv[i] 指向每段开头（strtok 也行）
 *     - 最多 MAX_ARGS 个，超出就不再切
 *     - argc 最多返回 MAX_ARGS
 *   例：
 *     "led on 3"  → argc=3, argv={"led","on","3"}
 *     "  help  "  → argc=1, argv={"help"}
 */
static int split_args(char *line, char **argv, int max_args)
{
    char *p =line;
    int argc = 0;
    while (*p != '\0' && argc < max_args){
        while (*p == ' '){p++;}
        if (*p == '\0'){break;}
        argv[argc++] = p;
        while (*p != '\0' && *p != ' '){p++;}
        if (*p == ' '){*p++ = '\0';}

    }
    return argc; /* TODO: 改掉 */
}

/* TODO 2：查表并调用对应的 handler
 *   要求：
 *     - argc == 0 时直接返回
 *     - 用 strcmp 在 g_cmds 里找名字
 *     - 找到就调用 g_cmds[i].handler(argc, argv)
 *     - 没找到就打印：未知命令: xxx，并提示输入 help
 *   返回值：找到返回 0，没找到返回 -1
 */
static int dispatch(int argc, char **argv)
{
    if (argc == 0){
        return -1;
    }
    for (int i = 0; i< g_cmd_count;i++){
        if (strcmp(argv[0],g_cmds[i].name) == 0){
            g_cmds[i].handler(argc,argv);
            return 0;
        }
        
    }
    printf("未知命令: %s\n",argv[0]);
    printf("输入 help 看命令\n");
    return -1; /* TODO: 改掉 */
}

/* 这个已经给你写好了，照着它的样子写后面三个 */
static void cmd_help(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("可用命令：\n");
    for (int i = 0; i < g_cmd_count; i++) {
        printf("  %s\n", g_cmds[i].help);
    }
}


/* TODO 3：把 argv[1..argc-1] 用空格连起来打印一次 */
static void cmd_echo(int argc, char **argv)
{
    for(int i = 1;i< argc;i++){
        printf("%s",argv[i]);
        if (i != argc - 1){
            printf(" ");
        
        }
    }
    printf("\n");
    /* TODO: 改掉 */
}

/* TODO 4：led on / led off / led toggle
 *   要求：
 *     - 没有参数或参数不认识时，打印用法：用法: led on|off|toggle
 *     - 每次操作后打印当前状态，例如：LED: ON
 */
static void cmd_led(int argc, char **argv)
{
    if (argc < 2 ){
        printf("用法: led on|off|toggle\n");
        return;
    }
    if (strcmp(argv[1],"on") == 0){
        g_led_on = 1;
    }else if (strcmp(argv[1],"off") ==0){
        g_led_on = 0;
    }else if (strcmp(argv[1],"toggle") == 0){
        g_led_on = !g_led_on;
    }else{
        printf("用法: led on|off|toggle\n");
        return;
    }
    printf("LED: %s\n",g_led_on ? "ON" : "OFF");
    /* 占位：防止编译警告，实现之后可以删掉 */
    /* TODO: 改掉 */
}

/* TODO 5：打印三条信息即可（编译器、编译时间、本文件行数用 __LINE__ 取也行）
 *   例如：
 *     [sys] 编译器    : GCC
 *     [sys] 编译时间  : Sep 12 2026 21:00:00
 *     [sys] LED 状态  : OFF
 */
static void cmd_sys(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("[sys] 编译器    : GCC\n");
    printf("[sys] 编译时间  : %s\n", __DATE__ " " __TIME__);
    printf("[sys] LED 状态  : %s\n", g_led_on ? "ON" : "OFF");
    /* TODO: 改掉 */
}

int main(void)
{
    char  line[MAX_LINE];
    char *argv[MAX_ARGS];

    printf("=== W1D3 命令行解析器 ===\n");
    printf("输入 help 看命令，输入 quit 退出。\n\n");

    while (1) {
        printf("w1d3> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break; /* Ctrl+Z 回车 或 Ctrl+D */
        }

        line[strcspn(line, "\r\n")] = '\0'; /* 去掉行尾换行 */

        int argc = split_args(line, argv, MAX_ARGS);
        if (argc == 0) {
            continue; /* 空行 */
        }

        if (strcmp(argv[0], "quit") == 0 || strcmp(argv[0], "exit") == 0) {
            printf("bye\n");
            break;
        }

        dispatch(argc, argv);
    }

    return 0;
}
