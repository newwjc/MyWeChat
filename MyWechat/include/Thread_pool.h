#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#include "packdef.h"
//任务块
typedef struct{
    void* (*task)(void*);//任务函数
    void * arg;//任务参数
} task_t;
//线程池
typedef struct STRU_POOL_T
{
    STRU_POOL_T( int max,int min,int que_max );
    int thread_max;//线程最大数量
    int thread_min;//线程最小数量
    int thread_alive;//当前存活线程数
    int thread_busy;//当前忙碌线程数
    int thread_shutdown;//控制开关
    int thread_wait;//需要等待关闭的线程数
    int queue_max;//循环队列最大长度
    int queue_cur;//当前队列内任务数量
    int queue_front;//循环队列头指针
    int queue_rear;//循环队列尾指针
    pthread_cond_t not_full;//队列满，生产者等待
    pthread_cond_t not_empty;//队列空消费者等待
    pthread_mutex_t lock;//互斥锁，避免多线程并发
    task_t * queue_task;//循环队列
    pthread_t *tids;//工作线程id数组
    pthread_t manager_tid;//管理者线程id
} pool_t;

class thread_pool
{
public:
    bool Pool_create(int,int,int);
    void pool_destroy();
    int Producer_add( void *(*)(void*),void*);
    static void * Custom(void*);
    static void * Manager(void*);

    static int if_thread_alive(pthread_t);
private:
    pool_t * m_pool;
};



#endif




