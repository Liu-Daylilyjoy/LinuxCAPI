#include <iostream>
#include <pthread.h>
#include <queue>
#include <vector>
#include <functional>
#include <ctime>
#include <unistd.h>


static int count = 0;
const int MAXTHREADS = 4;
pthread_condattr_t condattr;
pthread_cond_t condition;
pthread_mutex_t mutex;
pthread_mutex_t queueMutex;
pthread_mutex_t countMutex;
timespec waitTime;

void* f(void* t){
    pthread_mutex_lock(&countMutex);

    printf("This thread count %d\n",count++);

    pthread_mutex_unlock(&countMutex);

    return nullptr;
};

class Producer{
    protected:
        std::queue<std::function<void*(void*)>> producerQueue;//this is C++11,a package for function
    
    public:
        static void* product(void* target){
            Producer* pro = (Producer*)target;

            while(count<12){
                if(pro->producerQueue.size()<10){
                    pro->addJob(f);
                }

                sleep(2);//don't add job too fast
            }

            return nullptr;
        }

        void addJob(std::function<void*(void*)> job){
            printf("add job\n");

            pthread_mutex_lock(&mutex);

            pthread_mutex_lock(&queueMutex);
            producerQueue.push(job);
            pthread_mutex_unlock(&queueMutex);
            
            printf("add job finished\n");
            
            pthread_mutex_unlock(&mutex);   

            pthread_cond_broadcast(&condition);
        }
};

class Consumer:public Producer{
    private:
        std::vector<pthread_t> consumerQueue;

    public:
        Consumer(int n = MAXTHREADS):Producer(){
            pthread_t temp[MAXTHREADS];
            consumerQueue.assign(temp[0],temp[MAXTHREADS]);
        }

        void* start(void*){
            int i;
            
            while(count<12){
                i = 0;

                while(i<MAXTHREADS){
                    pthread_create(&consumerQueue[i++],nullptr,consume,this);//static member function can be used as argument

                    sleep(2);
                }

                i = 0;

                while(i<MAXTHREADS){
                    pthread_join(consumerQueue[i++],nullptr);
                }
            }

            return nullptr;
        }

        static void* consume(void* target){
            Consumer* where = (Consumer*)target;

            pthread_mutex_lock(&mutex);

            printf("thread %ld start\n",pthread_self());

            while(where->producerQueue.empty()){
                ushort ret = pthread_cond_timedwait(&condition,&mutex,&waitTime);

                if(ret==ETIMEDOUT){
                    printf("timeout!,thread %ld is awaken\n",pthread_self());

                    pthread_mutex_unlock(&mutex);//if condition timeout,is will still block when get the mutex,so we should unlock it

                    pthread_exit(nullptr);

                    return nullptr;
                }
            }

            pthread_mutex_unlock(&mutex);

            pthread_mutex_lock(&queueMutex);
            auto f = where->producerQueue.front();
            f(nullptr);
            where->producerQueue.pop();
            pthread_mutex_unlock(&queueMutex);

            printf("thread %ld leave\n",pthread_self());

            pthread_exit(nullptr);

            return nullptr;
        }
};

int main(int argc,char** argv){//I hear that int main() not work on some os,so I write a standard main function
    printf("test start\n");

    clock_gettime(CLOCK_MONOTONIC,&waitTime);
    waitTime.tv_sec+=10;//set waittime 10s

    pthread_condattr_setclock(&condattr,CLOCK_MONOTONIC);
    pthread_cond_init(&condition,&condattr);

    pthread_mutex_init(&mutex,nullptr);
    pthread_mutex_init(&queueMutex,nullptr);
    pthread_mutex_init(&countMutex,nullptr);

    Consumer test;

    printf("job begin\n");
    
    pthread_t producer;

    pthread_create(&producer,nullptr,test.product,&test);//there can use process,but I don't how to communicate in processes...

    test.start(nullptr);

    pthread_join(producer,nullptr);

    printf("job finishesd\n");

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&queueMutex);
    pthread_mutex_destroy(&countMutex);
    pthread_cond_destroy(&condition);
    pthread_condattr_destroy(&condattr);
    
    exit(0);//in paralell threads,'exit' can quit the program directly,'return' just back to upper level
}