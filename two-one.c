#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

void *work() {
    printf("Nova thread criada. TID = %u!\n", (unsigned int)pthread_self());
    pthread_exit(NULL);
}

void main() {
    pthread_t worker;
    pthread_create(&worker, NULL, work, NULL);  // cria thread
    pthread_join(worker, NULL); // aguarda finalização da execução de work()
    pthread_exit(NULL);
}
