#include "util.h"

void setInventory(int newInventory){

    pthread_mutex_lock(&lock_inventory);
    inventory = newInventory;
    pthread_mutex_unlock(&lock_inventory);
}

int empty(){
    
    int i = inventory;
    if ( i >= 38000 ){
        setInventory(i - 38000);
    }
    return i;
}

void writeInFile(char *string){

    pthread_mutex_lock(&lock_file);
    
    fprintf(file, "%s\n\n", string);

    pthread_mutex_unlock(&lock_file);
}

void *timeHandler(){

    int fill = 10*supply;

    while( executionTime < timeWorking ){
        sleep( 1 );
        executionTime += 10;
        setInventory(inventory + fill);
    }
}