#include <stdio.h>
#include <unistd.h>

void main() {
    fork();
    printf("Novo processo criado!\n");
}
