#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#define MAX_THREADS 5 // define a quantidade de workers

void *work() {
  printf("Nova thread criada. TID = %u!\n", (unsigned int)pthread_self());
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
  pthread_exit(NULL);
}
