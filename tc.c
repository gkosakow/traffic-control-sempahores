#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t headLock;                                             // arrival at intersection
sem_t NE, NN, NW, WN, WW, WS, SW, SS, SE, ES, EE, EN;        // northwest mid, northeast mid, southeast mid, southwest mid
sem_t *SEM_ARRAY[] = {&NE, &NN, &NW, &WN, &WW, &WS, &SW, &SS, &SE, &ES, &EE, &EN};

typedef struct _directions{
int carID;
char dir_original;
char dir_target;
} directions;

void *blockIntersection(void *d){
    directions *carPtr = (directions *)d;
    printf("%c", carPtr->dir_original); //DEBUGGER
    printf("%c\t", carPtr->dir_target);   //DEBUGGER
    printf("Blocking intersection\n");    //DEBUGGER

    if (carPtr->dir_original == 'N'){
        if(carPtr->dir_target == 'N'){
            printf("Car 1 hit correct conditions\n"); //DEBUGGER
            sem_wait(&NN);
            sem_wait(&WN);
            sem_wait(&WW);
            sem_wait(&WS);
            sem_wait(&SE);
            sem_wait(&EE);
            sem_wait(&EN);
        } else if(carPtr->dir_target == 'E'){
            sem_wait(&NE);
            sem_wait(&SE);
            sem_wait(&EE);
        } else if(carPtr->dir_target == 'W'){
            sem_wait(&NW);
            sem_wait(&WW);
            sem_wait(&WS);
            sem_wait(&SW);
            sem_wait(&SS);
            sem_wait(&EE);
            sem_wait(&EN);
        }
    }
    if (carPtr->dir_original == 'E'){
        if(carPtr->dir_target == 'E'){
            sem_wait(&EE);
            sem_wait(&NE);
            sem_wait(&NN);
            sem_wait(&NW);
            sem_wait(&WS);
            sem_wait(&SS);
            sem_wait(&SE);
        } else if(carPtr->dir_target == 'S'){
            sem_wait(&ES);
            sem_wait(&WS);
            sem_wait(&SS);
        } else if(carPtr->dir_target == 'N'){
            sem_wait(&EN);
            sem_wait(&NN);
            sem_wait(&NW);
            sem_wait(&WN);
            sem_wait(&WW);
            sem_wait(&SS);
            sem_wait(&SE);
        }
    }
    if (carPtr->dir_original == 'S'){
        if(carPtr->dir_target == 'S'){
            sem_wait(&SS);
            sem_wait(&ES);
            sem_wait(&EE);
            sem_wait(&EN);
            sem_wait(&NW);
            sem_wait(&WW);
            sem_wait(&WS);
        } else if(carPtr->dir_target == 'W'){
            sem_wait(&SW);
            sem_wait(&NW);
            sem_wait(&WW);
        } else if(carPtr->dir_target == 'E'){
            sem_wait(&SE);
            sem_wait(&EE);
            sem_wait(&EN);
            sem_wait(&NE);
            sem_wait(&NN);
            sem_wait(&WW);
            sem_wait(&WS);
        }
    }
    if (carPtr->dir_original == 'W'){
        if(carPtr->dir_target == 'W'){
            printf("Car 2 hit correct conditional\n"); //DEBUGGER
            sem_wait(&WW);
            sem_wait(&SW);
            sem_wait(&SS);
            sem_wait(&SE);
            sem_wait(&EN);
            sem_wait(&NN);
            sem_wait(&NW);
        } else if(carPtr->dir_target == 'N'){
            sem_wait(&WN);
            sem_wait(&EN);
            sem_wait(&NN);
        } else if(carPtr->dir_target == 'S'){
            sem_wait(&WS);
            sem_wait(&SS);
            sem_wait(&SE);
            sem_wait(&ES);
            sem_wait(&EE);
            sem_wait(&NN);
            sem_wait(&NW);
        }
    }

    return (void *) NULL;
}

void *unblockIntersection(void *d){
    directions *carPtr = (directions *)d;
    printf("%c", carPtr->dir_original); //DEBUGGER
    printf("%c\t", carPtr->dir_target);   //DEBUGGER
    printf("Unblocking intersection\n");    //DEBUGGER

    if (carPtr->dir_original == 'N'){
        if(carPtr->dir_target == 'N'){
            sem_post(&NN);
            sem_post(&WN);
            sem_post(&WW);
            sem_post(&WS);
            sem_post(&SE);
            sem_post(&EE);
            sem_post(&EN);
        } else if(carPtr->dir_target == 'E'){
            sem_post(&NE);
            sem_post(&SE);
            sem_post(&EE);
        } else if(carPtr->dir_target == 'W'){
            sem_post(&NW);
            sem_post(&WW);
            sem_post(&WS);
            sem_post(&SW);
            sem_post(&SS);
            sem_post(&EE);
            sem_post(&EN);
        }
    }
    if (carPtr->dir_original == 'E'){
        if(carPtr->dir_target == 'E'){
            sem_post(&EE);
            sem_post(&NE);
            sem_post(&NN);
            sem_post(&NW);
            sem_post(&WS);
            sem_post(&SS);
            sem_post(&SE);
        } else if(carPtr->dir_target == 'S'){
            sem_post(&ES);
            sem_post(&WS);
            sem_post(&SS);
        } else if(carPtr->dir_target == 'N'){
            sem_post(&EN);
            sem_post(&NN);
            sem_post(&NW);
            sem_post(&WN);
            sem_post(&WW);
            sem_post(&SS);
            sem_post(&SE);
        }
    }
    if (carPtr->dir_original == 'S'){
        if(carPtr->dir_target == 'S'){
            sem_post(&SS);
            sem_post(&ES);
            sem_post(&EE);
            sem_post(&EN);
            sem_post(&NW);
            sem_post(&WW);
            sem_post(&WS);
        } else if(carPtr->dir_target == 'W'){
            sem_post(&SW);
            sem_post(&NW);
            sem_post(&WW);
        } else if(carPtr->dir_target == 'E'){
            sem_post(&SE);
            sem_post(&EE);
            sem_post(&EN);
            sem_post(&NE);
            sem_post(&NN);
            sem_post(&WW);
            sem_post(&WS);
        }
    }
    if (carPtr->dir_original == 'W'){
        if(carPtr->dir_target == 'W'){
            sem_post(&WW);
            sem_post(&SW);
            sem_post(&SS);
            sem_post(&SE);
            sem_post(&EN);
            sem_post(&NN);
            sem_post(&NW);
        } else if(carPtr->dir_target == 'N'){
            sem_post(&WN);
            sem_post(&EN);
            sem_post(&NN);
        } else if(carPtr->dir_target == 'S'){
            sem_post(&WS);
            sem_post(&SS);
            sem_post(&SE);
            sem_post(&ES);
            sem_post(&EE);
            sem_post(&NN);
            sem_post(&NW);
        }
    }
    
    return (void *) NULL;
}

void ArriveIntersection(void* d){
    directions *carPtr = (directions *)d;
    printf("%d\t", carPtr->carID);
    printf("arriving\n");
    sleep(2);
    sem_wait(&headLock);
    blockIntersection(d);
}

void CrossIntersection(void* d){
    directions *carPtr = (directions *)d;
    printf("%d\t", carPtr->carID);
    printf("crossing\n");
    sem_post(&headLock);
    sleep(4); // x= left, right, straight
}

void ExitIntersection(void* d){
    directions *carPtr = (directions *)d;
    printf("%d\t", carPtr->carID);
    printf("exiting\n");
    unblockIntersection(d);
}

void * Car(void* d) {
    directions *carPtr = (directions *)d;
    ArriveIntersection(d);
    CrossIntersection(d);
    ExitIntersection(d);
    printf("Car %d\t", carPtr->carID);
    printf("finished executing\n");
    return NULL;
}

int main(void) {
    
    sem_init(&headLock, 0, 1);
    
    // initialize semaphore, only to be used with threads in this process, set value to 1
    for(int i = 0; i < 12; i++){
        sem_init(SEM_ARRAY[i], 0, 1);
    }

    pthread_t thread1;
    pthread_t thread2;
    directions *car1Ptr;
    car1Ptr = (directions *)malloc(sizeof(directions));
    car1Ptr->carID = 1;
    car1Ptr->dir_original = 'N';
    car1Ptr->dir_target = 'N';
    printf("%c", car1Ptr->dir_original);
    printf("%c\n", car1Ptr->dir_target);

    directions *car2Ptr;
    car2Ptr = (directions *)malloc(sizeof(directions));
    car2Ptr->carID = 2;
    car2Ptr->dir_original = 'W';
    car2Ptr->dir_target = 'W';
    printf("%c", car2Ptr->dir_original);
    printf("%c\n", car2Ptr->dir_target);

    pthread_create(&thread1, NULL, Car, car1Ptr);
    pthread_create(&thread2, NULL, Car, car2Ptr);

    // printf("after pthread_create\n");
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("\n");
    return 0;
}