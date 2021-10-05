#include <pthread.h>
#include <iostream>
#include <unistd.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;//it's the same as pthread_rwlock_init(rwlock,nullptr)

int count = 0;

void* reader(void*){
    printf("reader %lu coming\n",pthread_self());

    pthread_rwlock_rdlock(&rwlock);//several reader can read a same file,when there are readers,the file can't be written

    printf("reader %lu get the lock\n",pthread_self());

    printf("reader:count %d\n",count);

    pthread_rwlock_unlock(&rwlock);

    return nullptr;
}

void* writer(void*){
    printf("writer %lu coming\n",pthread_self());

    pthread_rwlock_wrlock(&rwlock);//only one writer can write the file in the same time,when there is writer,the file can't be read

    printf("writer %lu get the lock\n",pthread_self());

    printf("writer:count+1\n");

    ++count;

    pthread_rwlock_unlock(&rwlock);

    return nullptr;
}

int main(){
    pthread_t readerThreadOne,readerThreadTwo,writerThread;

    while(count<6){
        for(int i = 0;i<2;++i){
            sleep(1);

            pthread_create(&writerThread,nullptr,writer,nullptr);

            pthread_join(writerThread,nullptr);
        }

        for(int i = 0;i<3;++i){
            sleep(1);

            pthread_create(&readerThreadOne,nullptr,reader,nullptr);
            pthread_create(&readerThreadTwo,nullptr,reader,nullptr);

            pthread_join(readerThreadOne,nullptr);
            pthread_join(readerThreadTwo,nullptr);
        }
    }

    return 0;
}