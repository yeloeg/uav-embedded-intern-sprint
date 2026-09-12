/*
 * W1D2 · 内存布局 · 位操作宏 · 环形缓冲区
 * ------------------------------------------------------------------
 * 今天三件事：
 *   1. 打印四类内存地址，看懂栈 / 堆 / 全局区 / 常量区在哪
 *   2. 补完 4 个位操作宏（每个参数都要加括号，想想为什么）
 *   3. 实现环形缓冲区 —— 这是本周最值钱的部分
 *
 * 编译运行：
 *   gcc -std=c11 -Wall -Wextra -g main.c -o w1d2.exe
 *   .\w1d2.exe
 *
 * 收工前回答：
 *   1. 环形缓冲区怎么区分空和满？你用的是哪种方案，为什么？
 *   2. 为什么 head / tail 要加 volatile？
 *   3. 位操作宏少写一对括号会出什么事？举一个具体的错误调用。
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

/* ===================== 一、内存布局：先读再跑 ===================== */

int       g_global_int   = 42;          /* 已初始化全局变量 → .data  */
int       g_global_zero;                /* 未初始化全局变量 → .bss    */
static int g_static_int  = 7;           /* 静态全局变量     → .data  */

static void show_memory_layout(void)
{
    int       local      = 1;                       /* 栈 */
    int      *heap       = malloc(sizeof(int));     /* 堆 */
    const char *literal  = "flight controller";     /* 只读数据段 */
    static int s_counter = 0;                       /* 静态局部变量 */

    s_counter++;
    if (heap) {
        *heap = 123;
    }

    printf("[内存布局] 先看每行的地址，再判断：谁和谁挨在一起？堆和栈各在哪一端？\n");
    printf("           提示：不同平台、不同编译器的排布不一样，嵌入式上由链接脚本决定。\n");
    printf("  函数地址      : %p\n", (void *)(uintptr_t)&show_memory_layout);
    printf("  字符串常量    : %p  \"%s\"\n", (const void *)literal, literal);
    printf("  已初始化全局  : %p  value=%d\n", (void *)&g_global_int, g_global_int);
    printf("  未初始化全局  : %p  value=%d\n", (void *)&g_global_zero, g_global_zero);
    printf("  静态全局      : %p  value=%d\n", (void *)&g_static_int, g_static_int);
    printf("  静态局部      : %p  value=%d\n", (void *)&s_counter, s_counter);
    printf("  堆 (malloc)   : %p\n", (void *)heap);
    printf("  栈 (局部变量) : %p\n", (void *)&local);
    printf("  → 把上面的地址和实际大小填进 notes.md，手画一张分布图。\n");

    free(heap);
}

/* ===================== 二、位操作宏（TODO 5-7） ===================== */

/* 已经给你写好的示范：SET_BIT
 * 注意每个参数都加了括号，而且用 1u 避免有符号位移的坑。 */
#define SET_BIT(reg, bit)    ((reg) |=  (1u << (bit)))

/* TODO 5：清掉第 bit 位（其他位不变）
 * 下面这个占位写法能编译，但什么也不做，所以测试会 FAIL —— 改成真正的实现 */
#define CLR_BIT(reg, bit)    ((reg) |= 0u)

/* TODO 6：翻转第 bit 位（占位写法同样是什么都不做） */
#define TOGGLE_BIT(reg, bit) ((reg) |= 0u)

/* TODO 7：读出第 bit 位的值（0 或 1），占位写法永远返回 0 */
#define READ_BIT(reg, bit)   (0u)

/* ===================== 三、环形缓冲区（TODO 8-12） ===================== */

#define RB_SIZE 8

typedef struct {
    uint8_t        buf[RB_SIZE];
    volatile size_t head;   /* 写入位置：下一个要写进去的下标 */
    volatile size_t tail;   /* 读取位置：下一个要读出来的下标 */
} ring_buffer_t;

/* TODO 8：初始化为空 */
void rb_init(ring_buffer_t *rb)
{
    (void)rb;
    /* TODO: 把 head / tail 归零，把 buf 清干净 */
}

/* TODO 9：空返回 1，否则返回 0 */
int rb_is_empty(const ring_buffer_t *rb)
{
    (void)rb;
    return 1; /* TODO: 改掉 */
}

/* TODO 10：满返回 1，否则返回 0 */
int rb_is_full(const ring_buffer_t *rb)
{
    (void)rb;
    return 0; /* TODO: 改掉 */
}

/* TODO 11：当前存了多少个字节 */
size_t rb_count(const ring_buffer_t *rb)
{
    (void)rb;
    return 0; /* TODO: 改掉 */
}

/* TODO 12：写入一个字节。成功返回 0，缓冲区满返回 -1 */
int rb_put(ring_buffer_t *rb, uint8_t data)
{
    (void)rb;
    (void)data;
    return -1; /* TODO: 改掉 */
}

/* TODO 13：读出一个字节到 *out。成功返回 0，缓冲区空返回 -1 */
int rb_get(ring_buffer_t *rb, uint8_t *out)
{
    (void)rb;
    (void)out;
    return -1; /* TODO: 改掉 */
}

/* ============================ 测试用例（不要改） ============================ */

static void test_bit_macros(void)
{
    printf("[位操作宏]\n");

    uint32_t reg = 0x00000000u;
    SET_BIT(reg, 3);
    CHECK(reg == 0x00000008u, "SET_BIT 把第 3 位置 1");
    CHECK(READ_BIT(reg, 3) == 1u, "READ_BIT 读到 1");
    CHECK(READ_BIT(reg, 4) == 0u, "READ_BIT 读到 0");
    CLR_BIT(reg, 3);
    CHECK(reg == 0x00000000u, "CLR_BIT 把第 3 位清 0");
    TOGGLE_BIT(reg, 3);
    CHECK(reg == 0x00000008u, "TOGGLE_BIT 翻转第 3 位");
    TOGGLE_BIT(reg, 3);
    CHECK(reg == 0x00000000u, "TOGGLE_BIT 再翻转回来");

    uint32_t multi = 0xF0F0F0F0u;
    SET_BIT(multi, 0);
    CLR_BIT(multi, 31);
    CHECK(multi == 0x70F0F0F1u, "只动目标位，其他位不受影响");
}

static void test_ring_buffer(void)
{
    printf("[环形缓冲区]\n");

    ring_buffer_t rb;
    uint8_t       v = 0;

    rb_init(&rb);
    CHECK(rb_is_empty(&rb) == 1, "初始化后为空");
    CHECK(rb_count(&rb) == 0, "初始化后计数为 0");
    CHECK(rb_get(&rb, &v) == -1, "空缓冲区读取返回 -1");

    CHECK(rb_put(&rb, 'A') == 0, "写入 A 成功");
    CHECK(rb_put(&rb, 'B') == 0, "写入 B 成功");
    CHECK(rb_put(&rb, 'C') == 0, "写入 C 成功");
    CHECK(rb_is_empty(&rb) == 0, "写入后不为空");
    CHECK(rb_count(&rb) == 3, "计数为 3");

    CHECK(rb_get(&rb, &v) == 0 && v == 'A', "先出 A（先进先出）");
    CHECK(rb_get(&rb, &v) == 0 && v == 'B', "再出 B");
    CHECK(rb_get(&rb, &v) == 0 && v == 'C', "再出 C");
    CHECK(rb_is_empty(&rb) == 1, "取完后为空");

    /* 写满：不管用"浪费一个槽位"还是"计数器"方案，这条都该通过 */
    size_t written = 0;
    while (rb_put(&rb, (uint8_t)written) == 0) {
        written++;
        if (written > RB_SIZE + 1) {
            break; /* 防死循环：说明你的 rb_put 满了还在返回成功 */
        }
    }
    CHECK(written >= RB_SIZE - 1, "写满时至少能写进 RB_SIZE-1 个字节");
    CHECK(rb_is_full(&rb) == 1, "写满后 is_full 为真");
    CHECK(rb_count(&rb) == written, "计数与写入个数一致");
    CHECK(rb_put(&rb, 0xEE) == -1, "满的时候写入返回 -1");

    /* 读空并检查顺序没乱 */
    size_t read_back = 0;
    int    order_ok  = 1;
    while (rb_get(&rb, &v) == 0) {
        if (v != (uint8_t)read_back) {
            order_ok = 0;
        }
        read_back++;
    }
    CHECK(read_back == written, "读出的个数与写入一致");
    CHECK(order_ok == 1, "环形回绕后顺序依然是先进先出");
    CHECK(rb_is_empty(&rb) == 1, "读空后 is_empty 为真");

    /* 再写一轮，验证回绕之后还能继续用 */
    for (int i = 0; i < 3; i++) {
        CHECK(rb_put(&rb, (uint8_t)(0x10 + i)) == 0, "回绕后仍可写入");
    }
    CHECK(rb_count(&rb) == 3, "回绕后计数正确");
}

int main(void)
{
    printf("=== W1D2 测试 ===\n");
    show_memory_layout();
    printf("\n");
    test_bit_macros();
    printf("\n");
    test_ring_buffer();
    printf("\n结果：%d 通过 / %d 失败\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
