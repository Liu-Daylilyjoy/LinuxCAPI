#ifndef __CLASSSUM_H__
#define __CLASSSUM_H__

#include <iostream>
#include <semaphore.h>
#include <pthread.h>

struct argu{
    int a = 0;
    int b = 0;
};

static int result = 0;
sem_t semaphore;
pthread_mutex_t mutexId;

template<typename T = int>
class Sum{
    public:
        Sum(){
            sem_init(&semaphore,0,6);//6 so thread id will no more than 6
            pthread_mutex_init(&mutexId,nullptr);
        }

        static void* get(void* temp){

            sem_wait(&semaphore);

            int count;

            sem_getvalue(&semaphore,&count);

            printf("%d threads are on work\n",6-count);

            std::cout << "thread " << pthread_self() << "do this job\n";

            T a = ((argu*)temp)->a;
            T b = ((argu*)temp)->b;

            pthread_mutex_lock(&mutexId);//avoid several threads change result at the same time

            result += (a+b);

            pthread_mutex_unlock(&mutexId);

            sem_post(&semaphore);

            return nullptr;
        }

        ~Sum(){
            std::cout << "Sum delete\n";
        }
};

#endif