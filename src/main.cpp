#include "thread_pool.h"
#include <cstdio>
#include<stdlib.h>
#include<unistd.h>

class CMyTask:public CTask{
public:
    CMyTask() = default;
    int Run(){
        printf("%s\n", (char*)ptr_data);
        int x = rand() % 4 + 1;
        sleep(x);
        return 0;
    }
    ~CMyTask(){}
};

int main(){
    CMyTask taskobj;
    char data_buff[] = "hello!";
    taskobj.setData((void *)data_buff);
    CThreadPool threadpool(5); //有5个线程的线程池
    for(int i=0;i<10;i++){
        threadpool.AddTask(&taskobj); //像线程池添加10个相同的任务
    }
    while(1){
        printf("There are still %d tasks need to handle\n", threadpool.getTaskSize());
        if(threadpool.getTaskSize() == 0){
            if(threadpool.StopAll() == -1){
                printf("Thread pool clear, exit.\nPress any key to exit.\n");
                getchar();
                exit(0);
            }
        }
        sleep(2);
        printf("Sleep 2 seconds...\n");
    }
    getchar();
    return 0;
}