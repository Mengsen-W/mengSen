# mengSen -- 一个基于```C++17```实现的多线程协程服务端网络库
---
## 1. 基础模块
- ```AsyncLogging```一个异步的Log类
- ```BlockingQueue```提供加锁的双端队列模板
- ```BoundedBlocking```提供加锁的循环队列
- ```copyable```提供了一个值语义的基类和标志
- ```CountDownLatch```可计数的互斥量
- ```CurrentThread```记录进程信息
- ```Exception```继承自标准库的```std::exception```提供一些额外的错误信息
- ```FileUtil```封装为一个文件管理类，负责文件的写入更新等功能的实际实现
- ```LogFile```专门负责log文件的管理，上承```Logging```下接```FileUtil```
- ```Logging```实际的log管理类，设置日志等级，

## 2. 日志模块
- 重写了```stream```库，提高了程序性能
- 增加了```AsyncLogging```将日志前端也变为异步多线程操作，提高了日志速度
- 将```FileUtil```扩展为```LogFile```，提高后端处理日志的速度

## 3. 时间部分
- 直接用```namespace```进行封装
- 采用全模板形式，通过特化实现多态
- 对于未特化的部分，默认不允许使用，链接器会发出错误
- 对于所有的时间按纳秒传递为```uint64_t```类型，适应寄存器
- 时区系统，默认采用国际时区，若要调换直接对```uint64_t```进行更改

## 4. 线程模块
- 使用系统调用，封装了部分API
- 记录线程信息的```CurrentThread```，采用```namespace```封装
- 线程控制类```Thread```，维护线程状态控制线程执行
- 线程池```ThreadPool```，管理线程类```Thread```，维护并发线程池状态

## 5. 协程部分
- 采用汇编对其重构

## 6. net部分
- ```Buffer```专用于```net```部分，以提高IO效率
- ```Callbacks```设置了```net```部分的可见回调
- ```Endian```封装了本机序转换为大端序的方法，以供内部使用
- ```Channel```管理fd回调函数和fd事件，用于连接fd，```EventLoop```，```Poller```部分
- ```Sockets```管理系统sockfd，提供系统sockfd对应函数的封装
- ```EventLoop class```专负责管理IO线程，对于每个```EventLoop class```分配一个```Channel class```
