#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#ifndef timeWorking
#define timeWorking 480
#endif

extern pthread_mutex_t lock_file;
extern pthread_mutex_t lock_inventory;

extern FILE* file;
extern int executionTime;
extern char centerName[42];
extern int maximumCapacity;
extern int inventory;
extern int supply;
extern int timeTruck;

void setInventory(int newInventory);

int empty();

void writeInFile(char *string);

void *timeHandler();