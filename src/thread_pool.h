#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include<vector>
#include<string>
#include<pthread.h>

using namespace std;
/*
任务基类，定义任务及任务数据，并执行任务
*/
class CTask{
protected:
    string task_name;
    void * ptr_data;
public:
    CTask() = default;
    CTask(string &name):task_name(name),ptr_data(NULL){} //列表初始化
    void setData(void *data); //设置任务参数
    virtual int Run() = 0; //基类中没有实现的纯虚构函数  任务的执行内容

    virtual ~CTask() {}
};


/*
线程池管理类
*/
class CThreadPool{
private:
    static vector<CTask *> task_list;  //任务列表
    static bool shutdown;  //线程退出标志
    int thread_num;  //线程池中线程数量
    pthread_t *pthread_id;  //线程池  线程数组
    static pthread_mutex_t pthread_mutex;  //线程同步锁 
    static pthread_cond_t pthread_cond;  //线程同步条件变量
protected:
    static void * ThreadFunc(void *threadData);  //新线程的线程回调函数
    static int MoveToIdle(pthread_t tid); //线程执行结束后将自己放入空闲线程中
    static int MoveToBusy(pthread_t tid); //移动到忙碌线程中
    int Create();//批量创建thread_num个线程
public:
    CThreadPool(int num);
    int AddTask(CTask *task); //将任务添加到任务队列中
    int StopAll();  //退出线程池中所有的线程
    int getTaskSize();  //当前任务队列中任务数量
};

#endif