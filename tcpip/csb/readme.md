# 第十章
[reference](https://github.com/riba2534/TCP-IP-NetworkNote/tree/master/ch10)

## 典型的并发服务器
1. 多进程服务器
2. 多线程服务器
3. 多路复用服务器

## 进程
### 概念
占用内存空间的正在运行的程序
### pid
 进程ID为大于2的整数。1分配给了操作系统启动后的（用于协助操作系统）的首个进程。
### fork
1. 父进程返回子进程的 pid
2. 子进程返回 0
3. getpid()

### 僵尸(Zombie)进程
与正常进程不同，kill命令对僵尸进程无效
### wait
两个函数：
1. wait
2. waitpid

两个宏：
1. WIFEEXITED()：wait if exited，子进程正常终结返回 true
2. WEXITSTATUS()：wait exit status，返回子进程的返回值

## 孤儿进程
僵尸进程是子进程结束，父进程没有回收，子进程是在父进程之前结束的，孤儿进程是在回收僵尸进程之前，父进程退出了，则僵尸进程变为“孤儿进程”，进而被init进程接管、回收。


## 信号
子进程终止的识别主题是操作系统，因此，若操作系统能把子进程结束的信息告诉正忙于工作的父进程，将有助于构建更高效的程序
为了实现上述的功能，引入信号处理机制（Signal Handing）。此处「信号」是在特定事件发生时由操作系统向进程发送的消息。另外，为了响应该消息，执行与消息相关的自定义操作的过程被称为「处理」或「信号处理」。
常见信号：
1. SIGINT:  signal interrupt，即ctrl+c
2. SIGCHLD: signal child exit，子进程结束的时候发出

信号处理函数：
1. signal
2. sigaction

signal 函数在 Unix 系列的不同操作系统可能存在区别，但 
sigaction 函数完全相同



