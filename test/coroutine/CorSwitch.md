# 协程切换的汇编代码
---

切换函数声明 **```int switch(cpu_ctx* nrw_ctx, cpu_ctx* cur_ctx)```**
从**cur_ctx** 切换到 **new_ctx**

```c
struct cpu_ctx {
  void *rsp; // 栈指针
  void *rbp; // 被调用者保存
  void *
```
