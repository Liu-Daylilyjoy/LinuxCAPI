#include "ClassSum.h"
#include <pthread.h>
#include <unistd.h>

int main(){
    Sum<int> worker;

    int a = 1;
    int b = 1;
    pthread_t temp[10];
    arg<int> argu[10];

    for(int i = 0;i<10;++i){
        argu[i].a = a++;
        argu[i].b = b++;
        argu[i].threadId = i;
    }

    for(int i = 0;i<10;++i){
        sleep(2);//I want to make every thread work in right order,in fact this is a way that decrease much efficience

        pthread_create(&temp[i],nullptr,worker.get,(void*)&argu[i]);
    }

    for(int i = 0;i<10;++i){
        pthread_join(temp[i],nullptr);//wait every thread over
    }

    return 0;
}