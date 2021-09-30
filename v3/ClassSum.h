#ifndef __CLASSSUM_H__
#define __CLASSSUM_H__

#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

struct argu{
    int a = 0;
    int b = 0;
};

static int result = 0;
sem_t semaphore;
pthread_mutex_t mutexId;
static int count = 0;

template<typename T = int>
class Sum{
    public:
        Sum(){
            sem_init(&semaphore,0,5);
            pthread_mutex_init(&mutexId,nullptr);
        }

        static void* get(void* temp){

            sem_wait(&semaphore);

            ++count;

            printf("%d threads are on work\n",count);

            std::cout << "thread " << pthread_self() << "do this job\n";

            T a = ((argu*)temp)->a;
            T b = ((argu*)temp)->b;

            pthread_mutex_lock(&mutexId);

            result += (a+b);

            pthread_mutex_unlock(&mutexId);

            sleep(5);

            sem_post(&semaphore);

            --count;

            return nullptr;
        }

        ~Sum(){
            std::cout << "Sum delete\n";
        }
};

#endif