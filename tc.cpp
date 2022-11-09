#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

sem_t semaphore;

void * threadfunc(void* arg) {
    while (1) {
        sem_wait(&semaphore);
        printf("Hello from da thread!\n");
        sem_post(&semaphore);
        sleep(1);
    }
    return NULL;
}

int main(void) {
    
    // initialize semaphore, only to be used with threads in this process, set value to 1
    sem_init(&semaphore, 0, 1);
    
    pthread_t *mythread;
    
    mythread = (pthread_t *)malloc(sizeof(*mythread));
    
    // start the thread
    cout << "Starting thread, semaphore is unlocked." << endl;
    pthread_create(mythread, NULL, threadfunc, NULL);
    
    getchar();
    
    sem_wait(&semaphore);
    cout << "Semaphore locked." << endl;
    
    // do stuff with whatever is shared between threads
    getchar();
    
    cout << "Semaphore unlocked." << endl;
    sem_post(&semaphore);
    
    getchar();
    
    return 0;
}