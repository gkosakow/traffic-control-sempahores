#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>

sem_t headLock;                                             // arrival at intersection
sem_t NE, NN, NW, WN, WW, WS, SW, SS, SE, ES, EE, EN;        // northwest mid, northeast mid, southeast mid, southwest mid
sem_t *SEM_ARRAY[] = {&NE, &NN, &NW, &WN, &WW, &WS, &SW, &SS, &SE, &ES, &EE, &EN};
time_t start, now;
struct timeval begin, end;

typedef struct _directions{
int carID;
double arrTime;
char dir_original;
char dir_target;
} directions;

void spin (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

void *checkPath(void *d){
    directions *carPtr = (directions *)d;

    if (carPtr->dir_original == 'N'){
        if(carPtr->dir_target == 'N'){
            sem_wait(&NN);
        } else if(carPtr->dir_target == 'E'){
            sem_wait(&NE);
        } else if(carPtr->dir_target == 'W'){
            sem_wait(&NW);
        }
    }
    if (carPtr->dir_original == 'E'){
        if(carPtr->dir_target == 'E'){
            sem_wait(&EE);
        } else if(carPtr->dir_target == 'S'){
            sem_wait(&ES);
        } else if(carPtr->dir_target == 'N'){
            sem_wait(&EN);
        }
    }
    if (carPtr->dir_original == 'S'){
        if(carPtr->dir_target == 'S'){
            sem_wait(&SS);
        } else if(carPtr->dir_target == 'W'){
            sem_wait(&SW);
        } else if(carPtr->dir_target == 'E'){
            sem_wait(&SE);
        }
    }
    if (carPtr->dir_original == 'W'){
        if(carPtr->dir_target == 'W'){
            sem_wait(&WW);
        } else if(carPtr->dir_target == 'N'){
            sem_wait(&WN);
        } else if(carPtr->dir_target == 'S'){
            sem_wait(&WS);
        }
    }
    // now = time(NULL);
    // printf("Time: %lds\t", (now - start));
    // printf("Car %d (%c , %c)\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    // printf("\t\t\t\t\tUnblocked current path\n");    //DEBUGGER

    return (void *) NULL;
}

void *blockPath(void *d){
    directions *carPtr = (directions *)d;

    if (carPtr->dir_original == 'N'){
        if(carPtr->dir_target == 'N'){
            sem_trywait(&WN);
            sem_trywait(&WW);
            sem_trywait(&WS);
            sem_trywait(&SE);
            sem_trywait(&EE);
            sem_trywait(&EN);
        } else if(carPtr->dir_target == 'E'){
            sem_trywait(&SE);
            sem_trywait(&EE);
        } else if(carPtr->dir_target == 'W'){
            sem_trywait(&WW);
            sem_trywait(&WS);
            sem_trywait(&SW);
            sem_trywait(&SS);
            sem_trywait(&EE);
            sem_trywait(&EN);
        }
    }
    if (carPtr->dir_original == 'E'){
        if(carPtr->dir_target == 'E'){
            sem_trywait(&NE);
            sem_trywait(&NN);
            sem_trywait(&NW);
            sem_trywait(&WS);
            sem_trywait(&SS);
            sem_trywait(&SE);
        } else if(carPtr->dir_target == 'S'){
            sem_trywait(&WS);
            sem_trywait(&SS);
        } else if(carPtr->dir_target == 'N'){
            sem_trywait(&NN);
            sem_trywait(&NW);
            sem_trywait(&WN);
            sem_trywait(&WW);
            sem_trywait(&SS);
            sem_trywait(&SE);
        }
    }
    if (carPtr->dir_original == 'S'){
        if(carPtr->dir_target == 'S'){
            sem_trywait(&ES);
            sem_trywait(&EE);
            sem_trywait(&EN);
            sem_trywait(&NW);
            sem_trywait(&WW);
            sem_trywait(&WS);
        } else if(carPtr->dir_target == 'W'){
            sem_trywait(&NW);
            sem_trywait(&WW);
        } else if(carPtr->dir_target == 'E'){
            sem_trywait(&EE);
            sem_trywait(&EN);
            sem_trywait(&NE);
            sem_trywait(&NN);
            sem_trywait(&WW);
            sem_trywait(&WS);
        }
    }
    if (carPtr->dir_original == 'W'){
        if(carPtr->dir_target == 'W'){
            sem_trywait(&SW);
            sem_trywait(&SS);
            sem_trywait(&SE);
            sem_trywait(&EN);
            sem_trywait(&NN);
            sem_trywait(&NW);
        } else if(carPtr->dir_target == 'N'){
            sem_trywait(&EN);
            sem_trywait(&NN);
        } else if(carPtr->dir_target == 'S'){
            sem_trywait(&SS);
            sem_trywait(&SE);
            sem_trywait(&ES);
            sem_trywait(&EE);
            sem_trywait(&NN);
            sem_trywait(&NW);
        }
    }

    // now = time(NULL);
    // printf("Time: %lds\t", (now - start));
    // printf("Car %d (%c , %c)\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    // printf("\t\t\t\t\tBlocked path\n");    //DEBUGGER

    return (void *) NULL;
}

void *unblockPath(void *d){
    directions *carPtr = (directions *)d;

    if (carPtr->dir_original == 'N'){
        if(carPtr->dir_target == 'N'){
            sem_post(&WN);
            sem_post(&WW);
            sem_post(&WS);
            sem_post(&SE);
            sem_post(&EE);
            sem_post(&EN);
        } else if(carPtr->dir_target == 'E'){
            sem_post(&SE);
            sem_post(&EE);
        } else if(carPtr->dir_target == 'W'){
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
            sem_post(&NE);
            sem_post(&NN);
            sem_post(&NW);
            sem_post(&WS);
            sem_post(&SS);
            sem_post(&SE);
        } else if(carPtr->dir_target == 'S'){
            sem_post(&WS);
            sem_post(&SS);
        } else if(carPtr->dir_target == 'N'){
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
            sem_post(&ES);
            sem_post(&EE);
            sem_post(&EN);
            sem_post(&NW);
            sem_post(&WW);
            sem_post(&WS);
        } else if(carPtr->dir_target == 'W'){
            sem_post(&NW);
            sem_post(&WW);
        } else if(carPtr->dir_target == 'E'){
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
            sem_post(&SW);
            sem_post(&SS);
            sem_post(&SE);
            sem_post(&EN);
            sem_post(&NN);
            sem_post(&NW);
        } else if(carPtr->dir_target == 'N'){
            sem_post(&EN);
            sem_post(&NN);
        } else if(carPtr->dir_target == 'S'){
            sem_post(&SS);
            sem_post(&SE);
            sem_post(&ES);
            sem_post(&EE);
            sem_post(&NN);
            sem_post(&NW);
        }
    }

    // now = time(NULL);
    // printf("Time: %lds\t", (now - start));
    // printf("Car %d (%c , %c)\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    // printf("\t\t\t\t\tUnblocked path\n");    //DEBUGGER
    
    return (void *) NULL;
}

void *unblockCurrentPath(void *d){
    directions *carPtr = (directions *)d;

    if (carPtr->dir_original == 'N'){
        if(carPtr->dir_target == 'N'){
            sem_post(&NN);
        } else if(carPtr->dir_target == 'E'){
            sem_post(&NE);
        } else if(carPtr->dir_target == 'W'){
            sem_post(&NW);
        }
    }
    if (carPtr->dir_original == 'E'){
        if(carPtr->dir_target == 'E'){
            sem_post(&EE);
        } else if(carPtr->dir_target == 'S'){
            sem_post(&ES);
        } else if(carPtr->dir_target == 'N'){
            sem_post(&EN);
        }
    }
    if (carPtr->dir_original == 'S'){
        if(carPtr->dir_target == 'S'){
            sem_post(&SS);
        } else if(carPtr->dir_target == 'W'){
            sem_post(&SW);
        } else if(carPtr->dir_target == 'E'){
            sem_post(&SE);
        }
    }
    if (carPtr->dir_original == 'W'){
        if(carPtr->dir_target == 'W'){
            sem_post(&WW);
        } else if(carPtr->dir_target == 'N'){
            sem_post(&WN);
        } else if(carPtr->dir_target == 'S'){
            sem_post(&WS);
        }
    }

    // now = time(NULL);
    // printf("Time: %lds\t", (now - start));
    // printf("Car %d (%c , %c)\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    // printf("\t\t\t\t\tUnblocked current path\n");    //DEBUGGER

    return (void *) NULL;
}

void ArriveIntersection(void* d){
    directions *carPtr = (directions *)d;

    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    printf("Time: %.2fs\t", elapsed);

    printf("Car %d (%c , %c)\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    printf("arriving\n");
    sleep(2);
    sem_wait(&headLock);
    checkPath(d);
}

void CrossIntersection(void* d){
    directions *carPtr = (directions *)d;

    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    printf("Time: %.2fs\t", elapsed);

    printf("Car %d (%c , %c)\t\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    printf(" crossing\n");
    unblockCurrentPath(d);
    blockPath(d);
    sleep(4); // x= left, right, straight
}

void ExitIntersection(void* d){
    directions *carPtr = (directions *)d;

    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    printf("Time: %.2fs\t", elapsed);

    printf("Car %d (%c , %c)\t\t\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    printf("  exiting\n");
    unblockPath(d);
}

void * Car(void* d) {
    directions *carPtr = (directions *)d;

    sleep(1.1);

    ArriveIntersection(d);
    sem_post(&headLock);
    CrossIntersection(d);
    ExitIntersection(d);
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
    pthread_t thread3;
    pthread_t thread4;
    pthread_t thread5;
    pthread_t thread6;
    pthread_t thread7;
    pthread_t thread8;
 
    directions *car1Ptr;
    car1Ptr = (directions *)malloc(sizeof(directions));
    car1Ptr->carID = 0;
    car1Ptr->arrTime = 1.1;
    car1Ptr->dir_original = 'N';
    car1Ptr->dir_target = 'N';
 
    directions *car2Ptr;
    car2Ptr = (directions *)malloc(sizeof(directions));
    car2Ptr->carID = 1;
    car2Ptr->arrTime = 2.0;
    car2Ptr->dir_original = 'N';
    car2Ptr->dir_target = 'N';
 
    directions *car3Ptr;
    car3Ptr = (directions *)malloc(sizeof(directions));
    car3Ptr->carID = 2;
    car3Ptr->arrTime = 3.3;
    car3Ptr->dir_original = 'N';
    car3Ptr->dir_target = 'W';
 
    directions *car4Ptr;
    car4Ptr = (directions *)malloc(sizeof(directions));
    car4Ptr->carID = 3;
    car4Ptr->arrTime = 3.5;
    car4Ptr->dir_original = 'S';
    car4Ptr->dir_target = 'S';
 
    directions *car5Ptr;
    car5Ptr = (directions *)malloc(sizeof(directions));
    car5Ptr->carID = 4;
    car5Ptr->arrTime = 4.2;
    car5Ptr->dir_original = 'S';
    car5Ptr->dir_target = 'E';
 
    directions *car6Ptr;
    car6Ptr = (directions *)malloc(sizeof(directions));
    car6Ptr->carID = 5;
    car6Ptr->arrTime = 4.4;
    car6Ptr->dir_original = 'N';
    car6Ptr->dir_target = 'N';
 
    directions *car7Ptr;
    car7Ptr = (directions *)malloc(sizeof(directions));
    car7Ptr->carID = 6;
    car7Ptr->arrTime = 5.7;
    car7Ptr->dir_original = 'E';
    car7Ptr->dir_target = 'N';
 
    directions *car8Ptr;
    car8Ptr = (directions *)malloc(sizeof(directions));
    car8Ptr->carID = 7;
    car8Ptr->arrTime = 5.9;
    car8Ptr->dir_original = 'W';
    car8Ptr->dir_target = 'N';
 
 
    gettimeofday(&begin, 0);
    
    pthread_create(&thread1, NULL, Car, car1Ptr);
    pthread_create(&thread2, NULL, Car, car2Ptr);
    pthread_create(&thread3, NULL, Car, car3Ptr);
    pthread_create(&thread4, NULL, Car, car4Ptr);
    pthread_create(&thread5, NULL, Car, car5Ptr);
    pthread_create(&thread6, NULL, Car, car6Ptr);
    pthread_create(&thread7, NULL, Car, car7Ptr);
    pthread_create(&thread8, NULL, Car, car8Ptr);
 
    // printf("after pthread_create\n");
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);
    pthread_join(thread7, NULL);
    pthread_join(thread8, NULL);
   
    printf("Done with main thread.\n");
    return 0;
}