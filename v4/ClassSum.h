#ifndef __CLASSSUM_H__
#define __CLASSSUM_H__

#include <iostream>
#include <semaphore.h>
#include <pthread.h>

struct argu{
    int a = 0;
    int b = 0;
};

const int MAX_THREAD_NUMBER = 6;
bool isCreate = false;//judge whether has one instance of Sum
bool isPrintf = false;//judge whether has one thread is printf
static int result = 0;
sem_t semaphore;
pthread_mutex_t resultMutex;
pthread_mutex_t initMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition;

template<typename T = int>
class Sum{
    public:
        Sum(){
            if(!isCreate){//avoid lock each time when call Sum(),this called singleton mode(my code maybe not standard)
                pthread_mutex_lock(&initMutex);
                
                if(!isCreate){
                    sem_init(&semaphore,0,MAX_THREAD_NUMBER);
                    pthread_mutex_init(&resultMutex,nullptr);
                    pthread_cond_init(&condition,nullptr);

                    isCreate = true;
                }

                pthread_mutex_unlock(&initMutex);
            }
        }

        static void* get(void* temp){
            T a = ((argu*)temp)->a;
            T b = ((argu*)temp)->b;
            T c = a+b;

            sem_wait(&semaphore);

            int count;

            sem_getvalue(&semaphore,&count);

            pthread_mutex_lock(&printMutex);

            while(isPrintf){
                pthread_cond_wait(&condition,&printMutex);//cond_wait will auto unlock the mutex,so we should lock it before while()
            }

            isPrintf = true;

            pthread_mutex_unlock(&printMutex);

            //if a condition is waken up,it will get the mutex(if possible),we use while to ensure other thread is still wait

            printf("%d threads are on the work\n",MAX_THREAD_NUMBER-count);
            printf("thread %lu do this job:",pthread_self());
            printf("%d+%d\n",a,b);

            isPrintf = false;

            pthread_cond_signal(&condition);

            pthread_mutex_lock(&resultMutex);
            result += c;
            pthread_mutex_unlock(&resultMutex);

            sem_post(&semaphore);

            return nullptr;
        }

        ~Sum(){
            std::cout << "Sum delete\n";
        }
};

#endif