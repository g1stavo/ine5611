#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void main() {
  int i = 0;
  while (i < 4) {
    i++;
    if (fork()) {
      printf("Processo pai %i criou %i\n", getpid(), pid);
      wait(NULL);
    } else {
      printf("Processo filho %i\n", getpid());
      return;
    }
  }
}
