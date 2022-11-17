#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

sem_t headLock;                                       // arrival at intersection
sem_t NE, NN, NW, WN, WW, WS, SW, SS, SE, ES, EE, EN; // northwest mid, northeast mid, southeast mid, southwest mid
sem_t *SEM_ARRAY[] = {&NE, &NN, &NW, &WN, &WW, &WS, &SW, &SS, &SE, &ES, &EE, &EN};
int num_NE, num_NN, num_NW, num_WN, num_WW, num_WS, num_SW, num_SS, num_SE, num_ES, num_EE, num_EN; // tracker for how many blocked (for each direction)
time_t start, now;

typedef struct _directions
{
    int carID;
    int arrTime;
    char dir_original;
    char dir_target;

} directions;

void spin(unsigned int secs)
{
    unsigned int retTime = time(0) + secs; // Get finishing time.
    while (time(0) < retTime)
        ; // Loop until it arrives.
}

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
    // now = time(NULL);
    // printf("Time: %lds\t", (now - start));
    // printf("Car %d (%c , %c)\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    // printf("\t\t\t\t\tUnblocked current path\n");    //DEBUGGER

    return (void *)NULL;
}

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

    // now = time(NULL);
    // printf("Time: %lds\t", (now - start));
    // printf("Car %d (%c , %c)\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    // printf("\t\t\t\t\tBlocked path\n");    //DEBUGGER

    return (void *)NULL;
}

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
            sem_post(&SE);
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
                sem_post(&SE);

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
            sem_post(&NE);
            sem_post(&NN);
            sem_post(&NW);
            sem_post(&WS);
            sem_post(&SS);
            sem_post(&SE);
        }
        else if (carPtr->dir_target == 'S')
        {
            sem_post(&WS);
            sem_post(&SS);
        }
        else if (carPtr->dir_target == 'N')
        {
            sem_post(&NN);
            sem_post(&NW);
            sem_post(&WN);
            sem_post(&WW);
            sem_post(&SS);
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

            sem_post(&WS);
            num_WS--;
            if (num_WS == 0)
                sem_post(&WS);
        }
    }
    if (carPtr->dir_original == 'W')
    {
        if (carPtr->dir_target == 'W')
        {
            sem_post(&SW);
            sem_post(&SS);
            sem_post(&SE);
            sem_post(&EN);
            sem_post(&NN);
            sem_post(&NW);
        }
        else if (carPtr->dir_target == 'N')
        {
            sem_post(&EN);
            sem_post(&NN);
        }
        else if (carPtr->dir_target == 'S')
        {
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

    return (void *)NULL;
}

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

void ArriveIntersection(void *d)
{
    directions *carPtr = (directions *)d;

    now = time(NULL);
    printf("Time: %lds\t", (now - start));
    printf("Car %d (%c , %c)\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target); // DEBUGGER
    printf("arriving\n");
    spin(2);
    sem_wait(&headLock);
    checkPath(d);
}

void CrossIntersection(void *d)
{
    directions *carPtr = (directions *)d;

    now = time(NULL);
    printf("Time: %lds\t", (now - start));
    printf("Car %d (%c , %c)\t\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target); // DEBUGGER
    printf(" crossing\n");
    unblockCurrentPath(d);
    blockPath(d);
    spin(4); // x= left, right, straight
}

void ExitIntersection(void *d)
{
    directions *carPtr = (directions *)d;

    now = time(NULL);
    printf("Time: %lds\t", (now - start));
    printf("Car %d (%c , %c)\t\t\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target); // DEBUGGER
    printf("  exiting\n");
    unblockPath(d);
}

void *Car(void *d)
{
    directions *carPtr = (directions *)d;
    ArriveIntersection(d);
    sem_post(&headLock);
    CrossIntersection(d);
    ExitIntersection(d);

    // now = time(NULL);
    // printf("Time: %lds\t", (now - start));
    // printf("Car %d (%c , %c)\t\t\t\t", carPtr->carID, carPtr->dir_original, carPtr->dir_target);   //DEBUGGER
    // printf("gone \n");
    return NULL;
}

int main(void)
{

    sem_init(&headLock, 0, 1);

    // initialize semaphore, only to be used with threads in this process, set value to 1
    for (int i = 0; i < 12; i++)
    {
        sem_init(SEM_ARRAY[i], 0, 1);
    }

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    directions *car1Ptr;
    car1Ptr = (directions *)malloc(sizeof(directions));
    car1Ptr->carID = 1;
    car1Ptr->dir_original = 'N';
    car1Ptr->dir_target = 'N';

    directions *car2Ptr;
    car2Ptr = (directions *)malloc(sizeof(directions));
    car2Ptr->carID = 2;
    car2Ptr->dir_original = 'S';
    car2Ptr->dir_target = 'S';

    directions *car3Ptr;
    car3Ptr = (directions *)malloc(sizeof(directions));
    car3Ptr->carID = 3;
    car3Ptr->dir_original = 'W';
    car3Ptr->dir_target = 'W';

    start = time(NULL); // starting the timer. (NULL is initial time in int seconds)

    pthread_create(&thread1, NULL, Car, car1Ptr);
    spin(1);
    pthread_create(&thread2, NULL, Car, car2Ptr);
    spin(1);
    pthread_create(&thread3, NULL, Car, car3Ptr);

    // printf("after pthread_create\n");
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("Done with main thread.\n");
    return 0;
}