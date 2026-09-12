/*
 * W1D4 辅助文件：用来演示"多文件编译 + 链接"。
 * 这个文件不需要改，重点看 main.c 里的注释。
 */
#include "util.h"

int util_add(int a, int b)
{
    return a + b;
}

const char *util_version(void)
{
    return "util v1.0";
}
