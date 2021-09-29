#ifndef __CLASSSUM_H__
#define __CLASSSUM_H__

#include <iostream>

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
        static void *get(void* argument){
            T a = ((arg<T>*)argument)->a;
            T b = ((arg<T>*)argument)->b;
            
            std::cout << "This thread is " << ((arg<T>*)argument)->threadId << '\n';

            std::cout << "thread id is" << pthread_self() << '\n';
            
            printf("%d + %d = %d\n",a,b,a+b);
            
            return nullptr;
        }

        ~Sum(){
            std::cout << "Sum delete\n";
        }
};

#endif
