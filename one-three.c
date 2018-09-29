#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void main() {
  int i = 0;
  while (i < 2) {
    i++;
    if (fork()) {
      wait(NULL);
    } else {
      printf("Processo %i filho de %i\n", getpid(), getppid());
      if (fork()) {
        wait(NULL);
      } else {
        printf("Processo %i filho de %i\n", getpid(), getppid());
        if (fork()) {
          wait(NULL);
        } else {
          printf("Processo %i filho de %i\n", getpid(), getppid());
        }
      }
    return;
    }
  }
}
