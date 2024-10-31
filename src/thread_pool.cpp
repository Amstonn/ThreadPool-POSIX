#include "thread_pool.h"
#include<cstdio>

void CTask::setData(void *data){
    ptr_data = data;
}

//CThreadPool中的静态成员初始化
vector<CTask *> CThreadPool::task_list;
bool CThreadPool::shutdown = false;
pthread_mutex_t CThreadPool::pthread_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::pthread_cond = PTHREAD_COND_INITIALIZER;

//线程池构造函数
CThreadPool::CThreadPool(int num){
    thread_num = num;
    printf("I will create %d threads.\n", thread_num);
    Create();
}

//创建线程函数
int CThreadPool::Create(){
    pthread_id = new pthread_t[thread_num];
    for(int i=0;i<thread_num;i++){
        pthread_create(&pthread_id[i],NULL,ThreadFunc,NULL);
    }
    return 0;
}

//线程函数
void* CThreadPool::ThreadFunc(void * threadData){
    pthread_t tid = pthread_self();
    while(1){
        pthread_mutex_lock(&pthread_mutex);
        //如果任务队列为空，则等待新任务进入
        while(task_list.empty() && !shutdown){  //之所以循环是为了确保线程被唤醒后检查条件的有效性
            pthread_cond_wait(&pthread_cond,&pthread_mutex);
        }
        //如果线程池提示关闭所有线程
        if(shutdown){
            pthread_mutex_unlock(&pthread_mutex);
            printf("[tid:%lu] exit\n", tid);
            pthread_exit(NULL);
        }

        //有任务 未关闭线程池
        printf("[tid:%lu] turn:", tid);
        vector<CTask*>::iterator iter = task_list.begin();
        CTask* task = *iter;
        if(iter != task_list.end()){ //取任务队列第一个任务 并将其移除任务队列
            task = *iter;
            task_list.erase(iter);
        }
        pthread_mutex_unlock(&pthread_mutex);  //任务队列获取任务完成，临界代码段完成
        //执行被调度的任务
        task->Run();
        printf("[tid:%lu] idle\n", tid);
    }
    return (void*)0;
}
/*
添加任务到任务队列
*/
int CThreadPool::AddTask(CTask *task){
    pthread_mutex_lock(&pthread_mutex); //互斥访问任务队列
    task_list.push_back(task);
    pthread_mutex_unlock(&pthread_mutex);
    pthread_cond_signal(&pthread_cond); //有新任务到底释放条件变量 让被阻塞的进程被唤醒
    return 0;
}

/*
停止所有线程
*/
int CThreadPool::StopAll(){
    if(shutdown) return -1;//已经关闭则直接返回
    printf("Now I will shutdown all threads.\n");
    shutdown = true;
    pthread_cond_broadcast(&pthread_cond); //唤醒所有等待线程 
    //等待所有线程完成退出操作
    for(int i=0;i<thread_num;i++){
        pthread_join(pthread_id[i], NULL);
    }
    //删除线程数组
    delete[] pthread_id;
    pthread_id = NULL;

    //销毁互斥锁和条件变量
    pthread_mutex_destroy(&pthread_mutex);
    pthread_cond_destroy(&pthread_cond);

    return 0;
}

/*
获取任务队列中的任务数
*/
int CThreadPool::getTaskSize(){
    return task_list.size();
}

