# 通过POSIX实现的线程池
模拟具有5个线程的线程池处理10个任务
主要基于POSIX实现的线程同步锁和线程同步条件变量
# 启动方式
### 方式一
使用g++直接编译src目录下的文件

```g++ -o threadpool ./main.cpp ./thread_pool.cpp-pthread ```
### 方式二
使用Cmake 进入build目录并在终端输入命令 

```cmake ..```

然后在终端执行

```make ```

最后就可以执行bin目录下的可执行文件了