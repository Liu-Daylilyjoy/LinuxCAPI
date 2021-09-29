#ifndef __CLASSSUM_H__
#define __CLASSSUM_H__

#include <iostream>
#include <pthread.h>

static pthread_mutex_t mutexId;

template<typename T>
struct arg{
    T a;
    T b;
    int threadId;

    ~arg(){
        std::cout << "arg delete\n";
    }
};

template<typename T>
class Sum{
    public:
        Sum(){
            pthread_mutex_init(&mutexId,nullptr);
        }
        
        static void *get(void* argument){
            pthread_mutex_lock(&mutexId);

            T a = ((arg<T>*)argument)->a;
            T b = ((arg<T>*)argument)->b;
            
            std::cout << "This thread is " << ((arg<T>*)argument)->threadId << '\n';

            std::cout << "thread id is" << pthread_self() << '\n';
            
            printf("%d + %d = %d\n",a,b,a+b);
            
            pthread_mutex_unlock(&mutexId);

            return nullptr;
        }

        ~Sum(){
            std::cout << "Sum delete\n";
        }
};

#endif
