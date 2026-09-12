/*
 * W1D1 · 手写 memcpy / strcpy / swap
 * ------------------------------------------------------------------
 * 要求：
 *   1. my_memcpy / my_strcpy / swap_int / swap_generic 四个函数自己实现，
 *      不许调用 <string.h> 里的同名函数（memcmp 在测试里可以用）。
 *   2. main() 里的测试用例是裁判。全部 PASS 才算今天过关。
 *      禁止修改测试用例来"让它通过"。
 *
 * 编译运行：
 *   gcc -std=c11 -Wall -Wextra -Wpedantic -g main.c -o w1d1.exe
 *   .\w1d1.exe
 *
 * 收工前给自己口头回答（写在下面「我的结论」里）：
 *   1. memcpy 遇到 src / dst 内存重叠会怎样？为什么这是未定义行为？
 *   2. strcpy 的返回值为什么设计成 char* 而不是 void？
 *   3. volatile / static / const 各自解决什么问题？飞控里怎么用？
 *
 * 我的结论：
 *   （写在这里）
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg)                                              \
    do {                                                              \
        if (cond) {                                                   \
            g_pass++;                                                 \
            printf("  [PASS] %s\n", (msg));                           \
        } else {                                                      \
            g_fail++;                                                 \
            printf("  [FAIL] %s   (line %d)\n", (msg), __LINE__);     \
        }                                                             \
    } while (0)

/* ================= TODO 1：按字节复制 n 个字节，返回 dst ================= */
void *my_memcpy(void *dst, const void *src, size_t n)
{
    /* 提示：
     *   - 参数是 void*，必须先转成 unsigned char* 才能逐字节访问
     *   - 循环 i 从 0 到 n-1，做 d[i] = s[i]
     *   - 按标准要返回 dst
     *   - n == 0 时不许碰 dst
     */
    (void)dst;
    (void)src;
    (void)n;
    return dst; /* TODO: 改掉 */
}

/* ================= TODO 2：字符串复制，连结尾 '\0' 一起拷 ================= */
char *my_strcpy(char *dst, const char *src)
{
    /* 提示：循环直到遇到 '\0'，把它也复制过去，最后返回 dst */
    (void)src;
    return dst; /* TODO: 改掉 */
}

/* ================= TODO 3：交换两个 int ================= */
void swap_int(int *a, int *b)
{
    (void)a;
    (void)b;
    /* TODO: 改掉 */
}

/* ============ TODO 4（加分）：按字节交换任意类型，size 是元素大小 ============ */
void swap_generic(void *a, void *b, size_t size)
{
    /* 提示：用一个临时字节数组（比如 unsigned char tmp[64]），
     *       逐字节交换。想清楚 size > 64 时会发生什么。 */
    (void)a;
    (void)b;
    (void)size;
    /* TODO: 改掉 */
}

/* ============================ 测试用例（不要改） ============================ */

static void test_memcpy(void)
{
    printf("[my_memcpy]\n");

    unsigned char src[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    unsigned char dst[8] = {0};
    void *ret = my_memcpy(dst, src, sizeof(src));
    CHECK(memcmp(dst, src, sizeof(src)) == 0, "复制 8 个字节内容正确");
    CHECK(ret == (void *)dst, "返回值等于 dst");

    unsigned char guard[4] = {0xAA, 0xAA, 0xAA, 0xAA};
    my_memcpy(guard, src, 0);
    CHECK(guard[0] == 0xAA && guard[3] == 0xAA, "n == 0 时不碰目标内存");

    int numbers[4] = {10, 20, 30, 40};
    int copy[4] = {0};
    my_memcpy(copy, numbers, sizeof(numbers));
    CHECK(copy[0] == 10 && copy[3] == 40, "复制 4 个 int 正确");
}

static void test_strcpy(void)
{
    printf("[my_strcpy]\n");

    char dst[32] = {0};
    char *ret = my_strcpy(dst, "flight");
    CHECK(strcmp(dst, "flight") == 0, "复制普通字符串正确");
    CHECK(dst[6] == '\0', "结尾的 \\0 被复制过来了");
    CHECK(ret == dst, "返回值等于 dst");

    char empty[8] = {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'};
    my_strcpy(empty, "");
    CHECK(empty[0] == '\0', "复制空字符串只写一个 \\0");
}

static void test_swap(void)
{
    printf("[swap]\n");

    int a = 1, b = 2;
    swap_int(&a, &b);
    CHECK(a == 2 && b == 1, "swap_int 交换成功");

    int x = 100, y = 200;
    swap_generic(&x, &y, sizeof(int));
    CHECK(x == 200 && y == 100, "swap_generic 交换两个 int");

    double m = 3.5, n = 7.25;
    swap_generic(&m, &n, sizeof(double));
    CHECK(m == 7.25 && n == 3.5, "swap_generic 交换两个 double");
}

int main(void)
{
    printf("=== W1D1 测试 ===\n");
    test_memcpy();
    test_strcpy();
    test_swap();
    printf("\n结果：%d 通过 / %d 失败\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
