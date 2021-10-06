#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_spinlock_t spin; 

void* block(void*){
    pthread_spin_lock(&spin);

    for(int i = 0;i<10;++i){
        sleep(1);

        printf("block %d seconds\n",i+1);
    }

    pthread_spin_unlock(&spin);

    return nullptr;
}

void* wait_block(void*){
    printf("block begin\n");

    pthread_spin_lock(&spin);

    printf("block end\n");

    pthread_spin_unlock(&spin);

    return nullptr;
}

int main(){//can also use two threads
    pthread_spin_init(&spin,1);//set this lock can be shared by process,if not ,use PTHREAD_PROCESS_PRIVATE

    pthread_t one,two;

    pthread_create(&one,nullptr,block,nullptr);
    pthread_create(&two,nullptr,wait_block,nullptr);

    pthread_join(one,nullptr);
    pthread_join(two,nullptr);

    pthread_spin_destroy(&spin);

    return 0;
}