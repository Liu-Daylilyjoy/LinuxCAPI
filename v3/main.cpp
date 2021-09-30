#include "ClassSum.h"
#include <array>

int main(){
    Sum<int> worker;
    std::array<int,20> a;
    pthread_t temp[10];
    argu argv[10];

    a.fill(1);

    for(int i = 0;i<10;++i){
        argv[i].a = a[i];
        argv[i].b = a[i+1];

        pthread_create(&temp[i],nullptr,worker.get,(void*)&argv[i]);
    }

    for(int i = 0;i<10;++i){
        pthread_join(temp[i],nullptr);
    }

    printf("sum is %d\n",result);

    return 0;
}