/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-27 08:30:43
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-27 18:43:40
 */

#include <cstdio>
#include <cstring>
#include <iostream>

// extern "C" int test_asm(int a);

// __asm__(
//     "  .text;"
//     "     .p2align 4,,15;"
//     ".globl test_asm;"
//     "test_asm:;"
//     "     movq %rdi, %rax;"
//     "     ret;");

// typedef struct _nty_cpu_ctx {
//   // 栈顶指针
//   void *rsp;
//   // 被调用者保存，在过去用来保存栈底指针
//   void *rbp;
//   // 下一指令地址
//   void *rip;
//   // 第一个参数
//   void *edi;
//   // 第二个参数
//   void *esi;
//   // 被调用者保存
//   void *rbx;
//   // 被调用者保存
//   void *r1;
//   void *r2;
//   void *r3;
//   void *r4;
//   void *r5;
// } nty_cpu_ctx;

// extern "C" int _switch(nty_cpu_ctx *new_ctx, nty_cpu_ctx *cur_ctx);

// // %rdi 是第一个参数 %rsi 是第二个参数
// __asm__(
//     "    .text   \n"
//     // 字节对齐为 2^4 共 16 个, 最大到 15 序号
//     "       .p2align 4,,15  \n"
//     // 记录全局符号表
//     ".globl _switch \n"
//     // 指令跳转
//     "_switch:\n"
//     // 保存栈顶指针
//     "       movq %rsp, 0(%rsi) \n"
//     // 保存栈底指针
//     "       movq %rbp, 8(%rsi) \n"
//     // 借助 %rax 保存 (%rsp)
//     "       movq (%rsp), %rax  \n"
//     "       movq %rax, 16(%rsi)\n"
//     // 保存 IO指针
//     "       movq %rbx, 24(%rsi)\n"
//     // 其他数据
//     "       movq %r12, 32(%rsi)\n"
//     "       movq %r13, 40(%rsi)\n"
//     "       movq %r14, 48(%rsi)\n"
//     "       movq %r15, 56(%rsi)\n"
//     // 恢复栈状态
//     "       movq 56(%rdi), %r15\n"
//     "       movq 48(%rdi), %r14\n"
//     "       movq 40(%rdi), %r13\n"
//     "       movq 32(%rdi), %r12\n"
//     "       movq 24(%rdi), %rbx\n"
//     "       movq 8(%rdi), %rbp \n"
//     "       movq 0(%rdi), %rsp \n"
//     "       movq 16(%rdi), %rax\n"
// call 会把返回地址push到栈上，这里跟新栈顶的值就是更改返回地址
//     "       movq %rax, (%rsp)  \n"
//     // 返回
//     "       ret \n");

struct change_func {
  void *func1;
  void *func2;
};

extern "C" void *change(change_func *f, void * = nullptr);
__asm__(
    "    .text\n"
    "       .p2align 4,,15\n"
    ".globl change\n"
    "change:\n"
    "       movq 8(%rdi), %rax \n"
    "       movq %rax, (%rsp) \n"
    "       ret \n");

static void fun1(void) { std::cout << "func1" << std::endl; }
static void fun2(void) { std::cout << "func2" << std::endl; }

/* void testasm() {
  int a = 16;
  int b = 1;
  std::printf("The return value = %d\n", &a);
  std::printf("The return value = %d\n", &b);
  b = test_asm(25);
  std::printf("The return value = %d\n", b);
  b = test_asm(a);
  std::printf("The input value = %d\n", a);
  std::printf("The return value = %d\n", b);
  std::printf("The input value address = %x\n", &a);
  std::printf("The return value address = %x\n", &b);
} */

void test_changed() {
  change_func f;
  f.func1 = (void *)&fun1;
  f.func2 = (void *)&fun2;

  int a = 0;

  change(&f);
}

int main() {
  test_changed();
  return 0;
}
