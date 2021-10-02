#include "ClassSum.h"
#include <array>

int main(){
    Sum<int> worker;
    std::array<int,20> a = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    pthread_t temp[10];
    argu argv[10];

    for(int i = 0;i<10;++i){
        argv[i].a = a[2*i];
        argv[i].b = a[2*i+1];

        pthread_create(&temp[i],nullptr,worker.get,(void*)&argv[i]);
    }

    for(int i = 0;i<10;++i){
        pthread_join(temp[i],nullptr);
    }

    printf("sum is %d\n",result);

    return 0;
}