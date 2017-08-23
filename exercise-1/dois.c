#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void main() {
  int i = 0;
  while (i < 4) {
    int pid = fork();
    i++;
    if (pid >= 0) {
      if (pid > 0) {
        printf("Processo pai %i criou %i\n", getpid(), pid);
        wait(NULL);
      }
      if (pid == 0) {
        printf("Processo filho %i\n", getpid());
        return;
      }
    }
  }
}
