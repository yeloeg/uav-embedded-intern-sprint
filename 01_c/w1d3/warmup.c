/*
 * W1D3 前置热身 · 4 个最小例子
 * ------------------------------------------------------------------
 * 这份文件不用提交、也不用填 TODO。目的只有一个：
 * 把 W1D3 要用到的积木单独拎出来看一遍，看完再去做 main.c。
 *
 *   例 1  函数指针是什么
 *   例 2  把函数指针放进结构体数组 = 一张"表"
 *   例 3  char **argv：一行字符串是怎么被切成一段段的
 *   例 4  strcmp：为什么字符串比较不能用 ==
 *
 * 编译运行：
 *   gcc -std=c11 -Wall -Wextra -g warmup.c -o warmup.exe
 *   .\warmup.exe
 *
 * ⚠️ 运行前如果中文变成乱码（比如「渚� 1锛氬嚱鏁版寚閽�」）：
 *   这是终端代码页的问题，不是程序的问题。先切一次代码页再运行：
 *     chcp 65001
 *     .\warmup.exe
 *   原因：GCC 输出的字符串是 UTF-8 字节，而中文 Windows 的终端默认按 GBK(936) 解码。
 *   （本工具链不带 iconv，用 -finput-charset / -fexec-charset 转 GBK 会报错，
 *     所以正解是「源码保持 UTF-8 + 终端切成 UTF-8」。详见同目录的 前置知识.md 第六节。）
 *
 * 看完之后建议动手改一改，比如：
 *   - 在 g_ops 表里加一条 {"sub", op_sub}
 *   - 把"  led   on   3  "改成别的命令，看切词结果
 */

#include <stdio.h>
#include <string.h>

/* ==================== 例 1：函数指针 ==================== */

static int op_add(int a, int b) { return a + b; }
static int op_mul(int a, int b) { return a * b; }
static int op_sub(int a, int b) { return a - b; }

static void demo_1_function_pointer(void)
{
    printf("=== 例 1：函数指针 ===\n");

    /* 读法：f 是一个指针，指向「接收两个 int、返回 int」的函数 */
    int (*f)(int, int);

    f = op_add;                                   /* 函数名本身就是地址，不用写 & */
    printf("  f = op_add → f(3, 4) = %d\n", f(3, 4));

    f = op_mul;
    printf("  f = op_mul → f(3, 4) = %d\n", f(3, 4));

    printf("  (*f)(3, 4) 和 f(3, 4) 等价 → %d\n", (*f)(3, 4));

    /* 这两个长得很像，但完全不是一回事： */
    /*   int (*g)(int, int);   ← g 是「函数指针变量」      */
    /*   int *h(int, int);     ← h 是「返回 int* 的函数」  */
    printf("  记住：被括号包住的 (*名字) 说明它是指针\n\n");
}

/* ==================== 例 2：表驱动 ==================== */

/* 给「接收两个 int、返回 int」这类函数指针起个短名字，表里写着好看 */
typedef int (*op_fn)(int, int);

typedef struct {
    const char *name;    /* 命令名 */
    op_fn       fn;      /* 处理它的函数 */
} op_entry_t;

static const op_entry_t g_ops[] = {
    {"add", op_add},
    {"mul", op_mul},
    {"sub", op_sub},
    /* 想加新命令？在这里加一行就行，下面的循环一行都不用改 */
};

static void demo_2_table(void)
{
    printf("=== 例 2：表驱动（结构体数组 + 函数指针）===\n");

    const int count = (int)(sizeof(g_ops) / sizeof(g_ops[0]));

    printf("  遍历整张表：\n");
    for (int i = 0; i < count; i++) {
        printf("    %-4s(6, 7) = %d\n", g_ops[i].name, g_ops[i].fn(6, 7));
    }

    /* 模拟「按名字查找并调用」——这正是 dispatch 的核心几行 */
    const char *wanted = "mul";
    printf("  查找 \"%s\"：\n", wanted);
    for (int i = 0; i < count; i++) {
        if (strcmp(g_ops[i].name, wanted) == 0) {
            printf("    命中！结果是 %d\n", g_ops[i].fn(6, 7));
            break;
        }
    }
    printf("  没找到时就应该打印「未知命令」并提示输入 help\n\n");
}

/* ==================== 例 3：切词 ==================== */

static void demo_3_split(void)
{
    printf("=== 例 3：把一行字符串切成 argv[] ===\n");

    const char original[] = "  led   on   3  ";   /* 故意留多余空格 */
    char       line[64];
    char      *argv[8];
    int        argc = 0;

    strcpy(line, original);   /* 切词会改字符串内容，所以先复制一份 */

    char *p = line;
    while (*p != '\0') {
        while (*p == ' ') { p++; }                  /* 跳过空格 */
        if (*p == '\0') { break; }                  /* 已经到结尾 */

        argv[argc++] = p;                           /* 记下这一段的开头 */

        while (*p != '\0' && *p != ' ') { p++; }    /* 往前找到这一段的结尾 */
        if (*p != '\0') {                           /* 不是结尾，说明遇到空格 */
            *p = '\0';                              /* 用 '\0' 就地封口 */
            p++;                                    /* 继续往后走 */
        }
    }

    printf("  原始字符串：\"%s\"\n", original);
    printf("  切出 %d 段：\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("    argv[%d] = \"%s\"\n", i, argv[i]);
    }
    printf("  空格去哪了？被就地改成了 '\\0'，字符串被切成几段小字符串\n");
    printf("  char *argv[8] 作函数参数时写成 char **argv，是一回事\n\n");
}

/* ==================== 例 4：strcmp ==================== */

static void demo_4_strcmp(void)
{
    printf("=== 例 4：字符串比较为什么不能用 == ===\n");

    char        a[] = "led";
    char        b[] = "led";
    const char *p   = a;      /* p 指向数组 a 的第一个字符 */
    const char *q   = b;      /* q 指向数组 b 的第一个字符 */

    printf("  p == q             → %d   （比的是地址：两个不同数组，地址不同）\n", p == q);
    printf("  strcmp(p, q)       → %d   （0 表示内容完全相同）\n", strcmp(p, q));
    printf("  strcmp(p, \"ledx\")  → %d   （非 0 表示内容不同）\n", strcmp(p, "ledx"));
    printf("  所以判断相等要写：  if (strcmp(x, y) == 0)\n\n");
}

int main(void)
{
    demo_1_function_pointer();
    demo_2_table();
    demo_3_split();
    demo_4_strcmp();

    printf("热身结束。现在回到 01_c/w1d3/main.c，从 TODO 1（切词）开始。\n");
    return 0;
}
