// Universidade Federal de Santa Catarina
// Departamento de Informática e Estatística
// INE5611-04238A/B (20172) Sistemas Operacionais
// Prof. Cristian Koliver
// Trabalho 1 (T1)
// Copyright (c) 2018 Gustavo de Castro Salvador and Silvia Laurentino

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

// mutex lock
pthread_mutex_t lock1;
// NIM coins
int coins = 10;

// ternary tree node
struct Node {
  int value, guard;
  struct Node *left, *center, *right;
};

struct Node *root = NULL;

// creates a ternary tree node
struct Node* newNode(int value) {
    struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
    temp->value = value;
    temp->guard = 2;
    temp->left = temp->center = temp->right = NULL;
    return temp;
}

// build minimax tree. why a ternary tree? well, in NIM you can only remove 1, 2 or 3 objects
// which son would receive the value was an arbitrary decision
// uses pthreads to paralellize the construction of different branches
void build(struct Node* *node) {
    if ((*node)->value - 1 > 0) {
        (*node)->left = newNode((*node)->value - 1);
        pthread_t worker;
        pthread_create(&worker, NULL, build, &( (*node)->left )); 
        pthread_join(worker, NULL);
    }
    if ((*node)->value - 2 > 0) {
        (*node)->center = newNode((*node)->value - 2);
        build(&( (*node)->center ));
        pthread_t worker;
        pthread_create(&worker, NULL, build, &( (*node)->center )); 
        pthread_join(worker, NULL);
    }
    if ((*node)->value - 3 > 0) {
        (*node)->right = newNode((*node)->value - 3);
        build(&( (*node)->right ));
        pthread_t worker;
        pthread_create(&worker, NULL, build, &( (*node)->right )); 
        pthread_join(worker, NULL);
    }
}

// function to fill guards of minimax tree
// if it's a leaf, it receives a 1 if it's on a max depth or -1 if it's on a min depth
// if it's not a leaf, its guard become the sum of the node next depth sons guards 
// every time it calls itself, it swaps max/min
void fillGuards(struct Node* *node, bool max) {
    int guards = 0;
    if ((*node)->left) {
        if (max) {
            fillGuards((&( (*node)->left )), false);
        } else {
            fillGuards((&( (*node)->left )), true);
        }
        guards += ((*node)->left)->guard;
    }
    if ((*node)->center) {
        if (max) {
            fillGuards((&( (*node)->center )), false);
        } else {
            fillGuards((&( (*node)->center )), true);
        }
        guards += ((*node)->center)->guard;
    } 
    if ((*node)->right) {
        if (max) {
            fillGuards((&( (*node)->right )), false);
        } else {
            fillGuards((&( (*node)->right )), true);
        }
        guards += ((*node)->right)->guard;
    }
    (*node)->guard = guards;
    if (!((*node)->left) && !((*node)->center) && !((*node)->right)) {
        if (max) {
            (*node)->guard = 1;
        } else {
            (*node)->guard = -1;
        }
    }
}

// function to get the best computer move
// it does the choice based on the root three sons
// chooses the largest guard if it's max
// chooses the smallest guard if it's min
int bestMove(bool max) {
    int guard1, guard2, guard3 = 0;
    if ((root)->left) {
        guard1 = ((root)->left)->guard;
    }
    if ((root)->center) {
        guard2 = ((root)->center)->guard;
    }
    if ((root)->right) {
        guard3 = ((root)->right)->guard; 
    }
    if (max) {
        if (guard1 > guard2 && guard1 > guard3) {
            return (root)->value - ((root)->left)->value;
        }
        if (guard2 > guard1 && guard2 > guard3) {
            return (root)->value - ((root)->center)->value;
        }
        if (guard3 > guard1 && guard3 > guard2) {
            return (root)->value - ((root)->right)->value;
        }
        if (guard1 == guard2 && guard1 > guard3) {
            return (root)->value - ((root)->left)->value;
        }
        if (guard1 == guard3 && guard1 > guard2) {
            return (root)->value - ((root)->left)->value;
        }
        if (guard2 == guard3 && guard2 > guard1) {
            return (root)->value - ((root)->center)->value;
        }
    } else {
        if (guard1 < guard2 && guard1 < guard3) {
            return (root)->value - ((root)->left)->value;
        }
        if (guard2 < guard1 && guard2 < guard3) { 
            return (root)->value - ((root)->center)->value;
        }
        if (guard3 < guard1 && guard3 < guard2) {
            return (root)->value - ((root)->right)->value;
        }
        if (guard1 == guard2 && guard1 < guard3) {
            return (root)->value - ((root)->left)->value;
        }
        if (guard1 == guard3 && guard1 < guard2) {
            return (root)->value - ((root)->left)->value;
        }
        if (guard2 == guard3 && guard2 < guard1) {
            return (root)->value - ((root)->center)->value;
        }
    }
}

// lock mutex, deallocate memory
// creates root based on the new coins number and build minimax tree again
void *updateRoot() {
    pthread_mutex_lock(&lock1);	
    free(root);
    root = newNode(coins);
    build(&root);
    fillGuards(&root, true);
    pthread_mutex_unlock(&lock1);	
    pthread_exit(NULL);
}

// function that handles the game
// pthreads for running updateRoot()
int game () {
    if (coins == 1) {
        printf("Coins on the table: %i\n", coins);
        printf("Thanks for playing!\n");
        exit(0);
    }
    int move = 0;
    printf("Coins on the table: %i\n", coins);
    printf("Remove: ");
    scanf("%i", &move); 
    if (move > 4) {
        printf("You can't remove that much!\n");
        game();
    }
    if (move > 0 && move < 4) {
        coins -= move;
        if (coins == 1) {
            printf("You beat me!\n");
            exit(0);
        }
        pthread_t worker;
        pthread_create(&worker, NULL, updateRoot, NULL); 
        pthread_join(worker, NULL);
        int computerMove = bestMove(false);
        printf("Computer removed %i coins\n", computerMove);
        coins -= computerMove;
        pthread_t worker2;
        pthread_create(&worker2, NULL, updateRoot, NULL); 
        pthread_join(worker2, NULL);
        game();
    }
    return 0;
}

// main function
// start root, explain the rules and calls game
int main(void) {
    root = newNode(coins);	
    printf("Welcome to NIM. The game starts with imaginary 5 coins on the table.\n");
    printf("You can type 1, 2 or 3 for removing the coins. Your main objective is\n");
    printf("to leave the computer with just one coin. Can you? Let's play!\n");
    game();
    return 0;
}
