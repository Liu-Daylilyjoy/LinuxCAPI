#include "ClassSum.h"

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
        pthread_create(&temp[i],nullptr,worker.get,(void*)&argu[i]);
    }

    for(int i = 0;i<10;++i){
        pthread_join(temp[i],nullptr);
    }

    pthread_mutex_destroy(&mutexId);

    return 0;
}
//everything works quite well,but it have no advantage than the none mutex program