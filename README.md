<!--
 * @Date: 2022-01-26 12:21:33
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-03-03 10:45:42
 * @FilePath: /mengsen/README.md
 * @Description: file content
-->
# mengSen -- 一个基于```C++17```实现的协程网络库
---
## 1. 基础模块
* ```AsyncLogging```一个异步的Log类
* ```BlockingQueue```提供加锁的双端队列模板
* ```BoundedBlocking```提供加锁的循环队列
* ```Condition``` 包装条件变量
* ```Copyable```提供了一个值语义的基类和标志
* ```CountDownLatch```可计数的互斥量
* ```CurrentThread```记录进程信息
* ```Exception```继承自标准库的```std::exception```提供一些额外的错误信息
* ```FileUtil```封装为一个文件管理类，负责文件的写入更新等功能的实际实现
* ```LogFile```专门负责log文件的管理，上承```Logging```下接```FileUtil```，将```FileUtil```扩展为```LogFile```，提高后端处理日志的速度
* ```Logging```实际的log管理类，设置日志等级，设置日志输出函数等
* ```LogStream```用于Log的更加快速的Stream
* ```Mutex```包装互斥锁
* ```NonCopyable```提供了一个对象语义的基类和标记
* ```ProcessInfo```负责一些进程信息函数的封装
* ```Singleton```提供了集中单例类的封装
* ```SpinLock```使用```std::atomic_flag```封装了自旋锁
* ```StringPiece```代理模式，用于对```string```和```char*```做兼容，防止由于```const string&```形参，导致传入一个很长的```char*```时，会生成一个较大的```string```对象，开销比较大
* ```Thread```封装了线程调用，调用了一部分```CurrentThread```，用于维护各个线程
* ```ThreadPool```封装了线程池，调用了```Thread```，维护线程池
* ```Timestamp```时间戳系统，直接用```namespace```进行封装，采用全模板形式，通过特化实现多态，对于未特化的部分，默认不允许使用，链接器会发出错误，对于所有的时间按纳秒传递为```uint64_t```类型，适应寄存器
* ```TimeZone```时区系统，默认采用国际时区，若要调换直接对```uint64_t```进行更改
* ```Types```类型转换的封装
* ```WeakCallback```封装了弱回调，既可以保证对象生命周期也不会导致生命周期过长

## 2. 协程模块
* 采用汇编对其重构

## 3. 网络模块
* ```Acceptor```处理每个接受请求，绑定监听套接字，当有链接进入时给```Channel```回调```handleRead```，接受链接套接字，分配文件描述符
* ```Buffer```专用于网络部分，以提高IO效率，保留脏数据
* ```Callbacks```设置了网络部分的可见回调
* ```Channel```IO事件回调的分发器，根据事件的具体类型进行回调
* ```Endian```封装了本机序转换为大端序的方法，以供内部使用
* ```Channel```管理fd回调函数和fd事件，用于连接fd，```EventLoop```，```Poller```部分
* ```Sockets```管理系统sockfd，提供系统sockfd对应函数的封装
* ```EventLoop```专负责管理IO线程，维护一个```Channel```列表和```Poller```，每次循环过程中```Poller```会把```Channel```列表中活动的```Channel```传递给```EventLoop```，并由```EventLoop```进行处理
* ```EventLoopThread```控制```EventLoop```的线程
* ```EventLoopThreadPool```控制```EventLoopThread```的线程池
* ```InetAddress```这个类的作用现在还不知道
* ```EventLoop```控制事件循环
* ```Poller```所有```Multiplexing I/O```的基类，为```EventLoop```提供可以操作的```Channel```
* ```SocketsOps```封装了系统调用和错误处理，这是一个内部空间，前端不应该使用此接口
* ```TcpServer```管理```Acceptor```获得```TcpConnection```，直接供用户使用
* ```Timer```定时器事件的内部类，储存定时器的时间信息，例如是否重复，间隔事件，过期时间，定时器序号```TimeId```等
* ```TimerId```定时器计数类
* ```TimerQueue```定时器事件的内部类
