#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>

sem_t headLock;                                                                                         // allows tracking of who arrived and goes first.
sem_t northStopline, eastStopline, southStopline, westStopline;                                         // ceclaring semaphores to simulate lines at the stop signs.
sem_t NE, NN, NW, WN, WW, WS, SW, SS, SE, ES, EE, EN;                                                   // declaring semaphores for all possible car paths.
sem_t *SEM_ARRAY[] = {&NE, &NN, &NW, &WN, &WW, &WS, &SW, &SS, &SE, &ES, &EE, &EN};                      // creating an array to make sem_init easier.
int num_NE, num_NN, num_NW, num_WN, num_WW, num_WS, num_SW, num_SS, num_SE, num_ES, num_EE, num_EN;     // tracker for how many blocked (for each direction)
clock_t start, end;                                                                                     // start and end time to find time durations.
float duration;                                                                                         // duration between start and end times.

// declaring struct that stores car direction, ID, and arrival time.
typedef struct _directions{
int carID;
double arrTime;
char dir_original;
char dir_target;
} directions;

// spin function that spins for a given amount of time to simulate stopping at the stop sign or crossing through the intersection
void spin(float secs) {
    clock_t spinStart, spinEnd;
    float spinDuration;
    spinStart = clock();
    while (spinDuration < secs){
        spinEnd = clock();
        spinDuration = (float)(spinEnd - spinStart)/CLOCKS_PER_SEC;
    }
}

// turnType finds what type of turn R, L, S, given an initial and final direction.
char turnType(char origin, char final){
    char direction;
    if (origin == 'N' && final == 'E'){
        direction = '>';
    }
    if (origin == 'N' && final == 'N'){
        direction = '^';
    }
    if (origin == 'N' && final == 'W'){
        direction = '<';
    }
    if (origin == 'E' && final == 'S'){
        direction = '>';
    }
    if (origin == 'E' && final == 'E'){
        direction = '^';
    }
    if (origin == 'E' && final == 'N'){
        direction = '<';
    }
    if (origin == 'S' && final == 'W'){
        direction = '>';
    }
    if (origin == 'S' && final == 'S'){
        direction = '^';
    }
    if (origin == 'S' && final == 'E'){
        direction = '<';
    }
    if (origin == 'W' && final == 'N'){
        direction = '>';
    }
    if (origin == 'W' && final == 'W'){
        direction = '^';
    }
    if (origin == 'W' && final == 'S'){
        direction = '<';
    }
    return direction;
}

// checkPath performs a sem_wait for the path that the car would like to take.
void *checkPath(void *d)
{
    directions *carPtr = (directions *)d;

    if (carPtr->dir_original == 'N')
    {
        if (carPtr->dir_target == 'N')
        {
            sem_wait(&NN);
        }
        else if (carPtr->dir_target == 'E')
        {
            sem_wait(&NE);
        }
        else if (carPtr->dir_target == 'W')
        {
            sem_wait(&NW);
        }
    }
    if (carPtr->dir_original == 'E')
    {
        if (carPtr->dir_target == 'E')
        {
            sem_wait(&EE);
        }
        else if (carPtr->dir_target == 'S')
        {
            sem_wait(&ES);
        }
        else if (carPtr->dir_target == 'N')
        {
            sem_wait(&EN);
        }
    }
    if (carPtr->dir_original == 'S')
    {
        if (carPtr->dir_target == 'S')
        {
            sem_wait(&SS);
        }
        else if (carPtr->dir_target == 'W')
        {
            sem_wait(&SW);
        }
        else if (carPtr->dir_target == 'E')
        {
            sem_wait(&SE);
        }
    }
    if (carPtr->dir_original == 'W')
    {
        if (carPtr->dir_target == 'W')
        {
            sem_wait(&WW);
        }
        else if (carPtr->dir_target == 'N')
        {
            sem_wait(&WN);
        }
        else if (carPtr->dir_target == 'S')
        {
            sem_wait(&WS);
        }
    }

    return (void *)NULL;
}

// blockPath blocks all other paths that other cars could take that would result in a collision.
void *blockPath(void *d)
{
    directions *carPtr = (directions *)d;

    if (carPtr->dir_original == 'N')
    {
        if (carPtr->dir_target == 'N')
        {
            sem_trywait(&WN);
            sem_trywait(&WW);
            sem_trywait(&WS);
            sem_trywait(&SE);
            sem_trywait(&EE);
            sem_trywait(&EN);

            num_WN++;
            num_WW++;
            num_WS++;
            num_SE++;
            num_EE++;
            num_EN++;
        }
        else if (carPtr->dir_target == 'E')
        {
            sem_trywait(&SE);
            sem_trywait(&EE);

            num_SE++;
            num_EE++;
        }
        else if (carPtr->dir_target == 'W')
        {
            sem_trywait(&WW);
            sem_trywait(&WS);
            sem_trywait(&SW);
            sem_trywait(&SS);
            sem_trywait(&EE);
            sem_trywait(&EN);

            num_WW++;
            num_WS++;
            num_SW++;
            num_SS++;
            num_EE++;
            num_EN++;
        }
    }
    if (carPtr->dir_original == 'E')
    {
        if (carPtr->dir_target == 'E')
        {
            sem_trywait(&NE);
            sem_trywait(&NN);
            sem_trywait(&NW);
            sem_trywait(&WS);
            sem_trywait(&SS);
            sem_trywait(&SE);

            num_NE++;
            num_NN++;
            num_NW++;
            num_WS++;
            num_SS++;
            num_SE++;
        }
        else if (carPtr->dir_target == 'S')
        {
            sem_trywait(&WS);
            sem_trywait(&SS);

            num_WS++;
            num_SS++;
        }
        else if (carPtr->dir_target == 'N')
        {
            sem_trywait(&NN);
            sem_trywait(&NW);
            sem_trywait(&WN);
            sem_trywait(&WW);
            sem_trywait(&SS);
            sem_trywait(&SE);

            num_NN++;
            num_NW++;
            num_WN++;
            num_WW++;
            num_SS++;
            num_SE++;
        }
    }
    if (carPtr->dir_original == 'S')
    {
        if (carPtr->dir_target == 'S')
        {
            sem_trywait(&ES);
            sem_trywait(&EE);
            sem_trywait(&EN);
            sem_trywait(&NW);
            sem_trywait(&WW);
            sem_trywait(&WS);

            num_ES++;
            num_EE++;
            num_EN++;
            num_NW++;
            num_WW++;
            num_WS++;
        }
        else if (carPtr->dir_target == 'W')
        {
            sem_trywait(&NW);
            sem_trywait(&WW);

            num_NW++;
            num_WW++;
        }
        else if (carPtr->dir_target == 'E')
        {
            sem_trywait(&EE);
            sem_trywait(&EN);
            sem_trywait(&NE);
            sem_trywait(&NN);
            sem_trywait(&WW);
            sem_trywait(&WS);

            num_EE++;
            num_EN++;
            num_NE++;
            num_NN++;
            num_WW++;
            num_WS++;
        }
    }
    if (carPtr->dir_original == 'W')
    {
        if (carPtr->dir_target == 'W')
        {
            sem_trywait(&SW);
            sem_trywait(&SS);
            sem_trywait(&SE);
            sem_trywait(&EN);
            sem_trywait(&NN);
            sem_trywait(&NW);

            num_SW++;
            num_SS++;
            num_SE++;
            num_EN++;
            num_NN++;
            num_NW++;
        }
        else if (carPtr->dir_target == 'N')
        {
            sem_trywait(&EN);
            sem_trywait(&NN);

            num_EN++;
            num_NN++;
        }
        else if (carPtr->dir_target == 'S')
        {
            sem_trywait(&SS);
            sem_trywait(&SE);
            sem_trywait(&ES);
            sem_trywait(&EE);
            sem_trywait(&NN);
            sem_trywait(&NW);

            num_SS++;
            num_SE++;
            num_ES++;
            num_EE++;
            num_NN++;
            num_NW++;
        }
    }

    return (void *)NULL;
}

// unblockPath performs a sem_post for all the other paths that would result in a collision.
void *unblockPath(void *d)
{
    directions *carPtr = (directions *)d;

    if (carPtr->dir_original == 'N')
    {
        if (carPtr->dir_target == 'N')
        {
            num_WN--;
            if (num_WN == 0)
                sem_post(&WN);

            num_WW--;
            if (num_WW == 0)
                sem_post(&WW);

            num_WS--;
            if (num_WS == 0)
                sem_post(&WS);

            num_SE--;
            if (num_SE == 0)
                sem_post(&SE);

            num_EE--;
            if (num_EE == 0)
                sem_post(&EE);

            num_EN--;
            if (num_EN == 0)
                sem_post(&EN);
        }
        else if (carPtr->dir_target == 'E')
        {
            num_SE--;
            if (num_SE == 0)
                sem_post(&SE);
            num_EE--;
            if (num_EE == 0)
                sem_post(&EE);
        }
        else if (carPtr->dir_target == 'W')
        {
            num_WW--;
            if (num_WW == 0)
                sem_post(&WW);

            num_WS--;
            if (num_WS == 0)
                sem_post(&WS);

            num_SW--;
            if (num_SW == 0)
                sem_post(&SW);

            num_SS--;
            if (num_SS == 0)
                sem_post(&SS);

            num_EE--;
            if (num_EE == 0)
                sem_post(&EE);

            num_EN--;
            if (num_EN == 0)
                sem_post(&EN);
        }
    }
    if (carPtr->dir_original == 'E')
    {
        if (carPtr->dir_target == 'E')
        {
            num_NE--;
            if (num_NE == 0)
                sem_post(&NE);

            num_NN--;
            if (num_NN == 0)
                sem_post(&NN);

            num_NW--;
            if (num_NW == 0)
                sem_post(&NW);

            num_WS--;
            if (num_WS == 0)
                sem_post(&WS);

            num_SS--;
            if (num_SS == 0)
                sem_post(&SS);

            num_SE--;
            if (num_SE == 0)
                sem_post(&SE);
        }
        else if (carPtr->dir_target == 'S')
        {
            num_WS--;
            if (num_WS == 0)
                sem_post(&WS);

            num_SS--;
            if (num_SS == 0)
                sem_post(&SS);
        }
        else if (carPtr->dir_target == 'N')
        {
            num_NN--;
            if (num_NN == 0)
                sem_post(&NN);

            num_NW--;
            if (num_NW == 0)
                sem_post(&NW);

            num_WN--;
            if (num_WN == 0)
                sem_post(&WN);

            num_WW--;
            if (num_WW == 0)
                sem_post(&WW);

            num_SS--;
            if (num_SS == 0)
                sem_post(&SS);

            num_SE--;
            if (num_SE == 0)
                sem_post(&SE);
        }
    }
    if (carPtr->dir_original == 'S')
    {
        if (carPtr->dir_target == 'S')
        {
            num_ES--;
            if (num_ES == 0)
                sem_post(&ES);

            num_EE--;
            if (num_EE == 0)
                sem_post(&EE);

            num_EN--;
            if (num_EN == 0)
                sem_post(&EN);

            num_NW--;
            if (num_NW == 0)
                sem_post(&NW);

            num_WW--;
            if (num_WW == 0)
                sem_post(&WW);

            num_WS--;
            if (num_WS == 0)
                sem_post(&WS);
        }
        else if (carPtr->dir_target == 'W')
        {
            num_NW--;
            if (num_NW == 0)
                sem_post(&NW);

            num_WW--;
            if (num_WW == 0)
                sem_post(&WW);
        }
        else if (carPtr->dir_target == 'E')
        {
            num_EE--;
            if (num_EE == 0)
                sem_post(&EE);

            num_EN--;
            if (num_EN == 0)
                sem_post(&EN);

            num_NE--;
            if (num_NE == 0)
                sem_post(&NE);

            num_NN--;
            if (num_NN == 0)
                sem_post(&NN);

            num_WW--;
            if (num_WW == 0)
                sem_post(&WW);

            num_WS--;
            if (num_WS == 0)
                sem_post(&WS);
        }
    }
    if (carPtr->dir_original == 'W')
    {
        if (carPtr->dir_target == 'W')
        {
            num_SW--;
            if (num_SW == 0)
                sem_post(&SW);

            num_SS--;
            if (num_SS == 0)
                sem_post(&SS);

            num_SE--;
            if (num_SE == 0)
                sem_post(&SE);

            num_EN--;
            if (num_EN == 0)
                sem_post(&EN);

            num_NN--;
            if (num_NN == 0)
                sem_post(&NN);

            num_NW--;
            if (num_NW == 0)
                sem_post(&NW);
        }
        else if (carPtr->dir_target == 'N')
        {
            num_EN--;
            if (num_EN == 0)
                sem_post(&EN);

            num_NN--;
            if (num_NN == 0)
                sem_post(&NN);
        }
        else if (carPtr->dir_target == 'S')
        {
            num_SS--;
            if (num_SS == 0)
                sem_post(&SS);

            num_SE--;
            if (num_SE == 0)
                sem_post(&SE);

            num_ES--;
            if (num_ES == 0)
                sem_post(&ES);

            num_EE--;
            if (num_EE == 0)
                sem_post(&EE);

            num_NN--;
            if (num_NN == 0)
                sem_post(&NN);

            num_NW--;
            if (num_NW == 0)
                sem_post(&NW);
        }
    }

    return (void *)NULL;
}

// unblockCurrentPath sem_posts the car's actual path.
void *unblockCurrentPath(void *d)
{
    directions *carPtr = (directions *)d;

    if (carPtr->dir_original == 'N')
    {
        if (carPtr->dir_target == 'N')
        {
            sem_post(&NN);
        }
        else if (carPtr->dir_target == 'E')
        {
            sem_post(&NE);
        }
        else if (carPtr->dir_target == 'W')
        {
            sem_post(&NW);
        }
    }
    if (carPtr->dir_original == 'E')
    {
        if (carPtr->dir_target == 'E')
        {
            sem_post(&EE);
        }
        else if (carPtr->dir_target == 'S')
        {
            sem_post(&ES);
        }
        else if (carPtr->dir_target == 'N')
        {
            sem_post(&EN);
        }
    }
    if (carPtr->dir_original == 'S')
    {
        if (carPtr->dir_target == 'S')
        {
            sem_post(&SS);
        }
        else if (carPtr->dir_target == 'W')
        {
            sem_post(&SW);
        }
        else if (carPtr->dir_target == 'E')
        {
            sem_post(&SE);
        }
    }
    if (carPtr->dir_original == 'W')
    {
        if (carPtr->dir_target == 'W')
        {
            sem_post(&WW);
        }
        else if (carPtr->dir_target == 'N')
        {
            sem_post(&WN);
        }
        else if (carPtr->dir_target == 'S')
        {
            sem_post(&WS);
        }
    }

    // now = time(NULL);
    // printf("Time: %lds\t", (now - start));
    // printf("Car %d (%c , %c)\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    // printf("\t\t\t\t\tUnblocked current path\n");    //DEBUGGER

    return (void *)NULL;
}

// ArriveIntersection simulates a car arriving at the intersection.
void ArriveIntersection(void* d){
    directions *carPtr = (directions *)d;

    end = clock();
    duration = (float)(end - start)/CLOCKS_PER_SEC;
    printf("Time: %.1f\t", duration);

    printf("Car %d (%c , %c)\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    printf("arriving\n");

    // simulates taking two seconds to stop at the intersection.
    spin(2);

    // if block that performs a sem_wait for the stop signs for all four directions.
    if(carPtr->dir_original == 'N'){
        sem_wait(&southStopline);
    }
    if(carPtr->dir_original == 'E'){
        sem_wait(&westStopline);
    }
    if(carPtr->dir_original == 'S'){
        sem_wait(&northStopline);
    }
    if(carPtr->dir_original == 'W'){
        sem_wait(&eastStopline);
    }

    // performs a wait on the headLock (tracks car arrival order to be executed)
    sem_wait(&headLock);

    // sem_wait on the desired car path.
    checkPath(d);
}

// CrossIntersection simulates a car crossing the intersection.
void CrossIntersection(void* d){
    directions *carPtr = (directions *)d;
    
    end = clock();
    duration = (float)(end - start)/CLOCKS_PER_SEC;
    printf("Time: %.1f\t", duration);
    
    printf("Car %d (%c , %c)\t\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    printf(" crossing\n");

    // sem_waits and blocks all other paths that will collide with this path.
    blockPath(d);

    // sem_posts to this car's path as it is driving through the intersection.
    unblockCurrentPath(d);

    // if block that performs a sem_post for the stop signs for all four directions.
    sem_post(&headLock);
    if(carPtr->dir_original == 'N'){
        sem_post(&southStopline);
    }
    if(carPtr->dir_original == 'E'){
        sem_post(&westStopline);
    }
    if(carPtr->dir_original == 'S'){
        sem_post(&northStopline);
    }
    if(carPtr->dir_original == 'W'){
        sem_post(&eastStopline);
    }

    // simulates the time to go left, right, or straight.
    if (turnType(carPtr->dir_original, carPtr->dir_target) == '<'){
        spin(5);
    }
    if (turnType(carPtr->dir_original, carPtr->dir_target) == '^'){
        spin(4);
    }
    if (turnType(carPtr->dir_original, carPtr->dir_target) == '>'){
        spin(3);
    }
}

// ExitIntersection simulates a car leaving the intersection.
void ExitIntersection(void* d){
    directions *carPtr = (directions *)d;

    end = clock();
    duration = (float)(end - start)/CLOCKS_PER_SEC;
    printf("Time: %.1f\t", duration);

    printf("Car %d (%c , %c)\t\t\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    printf("  exiting\n");

    // sem_posting to unblock all paths that would've caused a collision.
    unblockPath(d);
}

// simulates the car's full course of action.
void * Car(void* d) {
    directions *carPtr = (directions *)d;

    // spinning to simulate car arrival time.
    spin(carPtr->arrTime);

    ArriveIntersection(d);
    CrossIntersection(d);
    ExitIntersection(d);

    return NULL;
}

int main(void) {
    
    sem_init(&headLock, 0, 1);
    sem_init(&northStopline, 0, 1);
    sem_init(&eastStopline, 0, 1);
    sem_init(&westStopline, 0, 1);
    sem_init(&southStopline, 0, 1);
    
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
    car1Ptr->carID = 1;
    car1Ptr->arrTime = 1.1;
    car1Ptr->dir_original = 'N';
    car1Ptr->dir_target = 'N';
 
    directions *car2Ptr;
    car2Ptr = (directions *)malloc(sizeof(directions));
    car2Ptr->carID = 2;
    car2Ptr->arrTime = 2.2;
    car2Ptr->dir_original = 'N';
    car2Ptr->dir_target = 'N';
 
    directions *car3Ptr;
    car3Ptr = (directions *)malloc(sizeof(directions));
    car3Ptr->carID = 3;
    car3Ptr->arrTime = 3.3;
    car3Ptr->dir_original = 'N';
    car3Ptr->dir_target = 'W';
 
    directions *car4Ptr;
    car4Ptr = (directions *)malloc(sizeof(directions));
    car4Ptr->carID = 4;
    car4Ptr->arrTime = 4.4;
    car4Ptr->dir_original = 'S';
    car4Ptr->dir_target = 'S';
 
    directions *car5Ptr;
    car5Ptr = (directions *)malloc(sizeof(directions));
    car5Ptr->carID = 5;
    car5Ptr->arrTime = 5.5;
    car5Ptr->dir_original = 'S';
    car5Ptr->dir_target = 'E';
 
    directions *car6Ptr;
    car6Ptr = (directions *)malloc(sizeof(directions));
    car6Ptr->carID = 6;
    car6Ptr->arrTime = 6.6;
    car6Ptr->dir_original = 'N';
    car6Ptr->dir_target = 'N';
 
    directions *car7Ptr;
    car7Ptr = (directions *)malloc(sizeof(directions));
    car7Ptr->carID = 7;
    car7Ptr->arrTime = 7.7;
    car7Ptr->dir_original = 'E';
    car7Ptr->dir_target = 'N';
 
    directions *car8Ptr;
    car8Ptr = (directions *)malloc(sizeof(directions));
    car8Ptr->carID = 8;
    car8Ptr->arrTime = 8.8;
    car8Ptr->dir_original = 'W';
    car8Ptr->dir_target = 'N';
 
    start = clock();                // starting clock to find time durations.
    
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
   
    printf("\nAll cars are through.\n");
    return 0;
}