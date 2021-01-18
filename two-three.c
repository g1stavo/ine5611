#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#define MAX_THREADS 16 // define a quantidade de workers

int contador_global = 0;

void *work() {
    if (contador_global % 100 == 0) {
        contador_global = contador_global + 2;
    } else {
        contador_global++;
    }
    pthread_exit(NULL);
}

void main() {
    pthread_t worker[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&worker[i], NULL, work, NULL);  // cria as threads
    }
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(worker[i], NULL); // aguarda finalização das execuções de work()
    }
    printf("contador_global = %i\n", contador_global);
    pthread_exit(NULL);
}
