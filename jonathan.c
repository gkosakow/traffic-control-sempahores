#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

//sem_t semaphore;
clock_t currentTime;
//pthread_mutex_t lane[8];
//pthread_mutex_t line[4];
int nNN = 0, nEE = 0, nSS = 0, nWW = 0, nNW = 0, nEN = 0, nSE = 0, nWS = 0, nNE = 0, nES = 0, nSW = 0, nWN = 0;
sem_t NN, EE, SS, WW, NW, EN, SE, WS, NE, ES, SW, WN;
sem_t N,E,S,W, line;

struct directions {
    char car_id;
    char dir_original;
    char dir_target;
};

float checkTime(clock_t cur){
    return ((float)(clock() - cur)) / CLOCKS_PER_SEC;
}

int ArriveIntersection(struct directions *dir) {
    printf("Time %.1f Car %d ( %c,  %c ) arriving\n", checkTime(currentTime), dir->car_id, dir->dir_original, dir->dir_target);
    int time = 0;
    clock_t wait;
    wait = clock();
    while (checkTime(wait) < 2);
    
    //if locks available start to cross
    //pthread_mutex_lock(&use);
    //acquire all other locks
    //pthread_mutex_unlock(&use);
    //sem_wait(&line);
    
    
    //sem_post(&line);
    if (dir->dir_original == 'N')
    {
        sem_wait(&N);
        printf("Car %d\n", dir->car_id);
        sem_wait(&line);
        if (dir->dir_target == 'N')
        {
            sem_wait(&NN);
            time = 4;

            sem_trywait(&EE);
            nEE++;
            sem_trywait(&WW);
            nWW++;
            sem_trywait(&EN);
            nEN++;
            sem_trywait(&WN);
            nWN++;
            sem_trywait(&SE);
            nSE++;
            sem_trywait(&WS);
            nWS++;

            sem_post(&NN);
        }
        else if (dir->dir_target == 'W')
        {
            sem_wait(&NW);
            time = 5;

            sem_trywait(&EE);
            nEE++;
            sem_trywait(&WW);
            nWW++;
            sem_trywait(&SS);
            nSS++;
            sem_trywait(&WS);
            nWS++;
            sem_trywait(&SW);
            nSW++;
            sem_trywait(&EN);
            nEN++;

            sem_post(&NW);
        }
        else if (dir->dir_target == 'E')
        {
            sem_wait(&NE);
            time = 3;

            sem_trywait(&SE);
            nSE++;
            sem_trywait(&EE);
            nEE++;

            sem_post(&NE);
        }
        sem_post(&line);
        //printf("test\n");
        sem_post(&N);
    }
    else if (dir->dir_original == 'W')
    {
        sem_wait(&W);
        printf("Car %d\n", dir->car_id);
        sem_wait(&line);
        if (dir->dir_target == 'W')
        {
            sem_wait(&WW);
            time = 4;

            sem_trywait(&NN);
            nNN++;
            sem_trywait(&SS);
            nSS++;
            sem_trywait(&SW);
            nSW++;
            sem_trywait(&SE);
            nSE++;
            sem_trywait(&EN);
            nEN++;
            sem_trywait(&NW);
            nNW++;

            sem_post(&WW);
        }
        else if (dir->dir_target == 'N')
        {
            sem_wait(&WN);
            time = 3;

            sem_trywait(&EN);
            nEN++;
            sem_trywait(&NN);
            nNN++;

            sem_post(&WN);
        }
        else if (dir->dir_target == 'S')
        {
            sem_wait(&WS);
            time = 5;

            sem_trywait(&NN);
            nNN++;
            sem_trywait(&SS);
            nSS++;
            sem_trywait(&EE);
            nEE++;
            sem_trywait(&NW);
            nNW++;
            sem_trywait(&ES);
            nES++;
            sem_trywait(&SE);
            nSE++;

            sem_post(&WS);
        }
        sem_post(&line);
        //printf("test");
        sem_post(&W);
    }
    else if (dir->dir_original == 'E')
    {
        sem_wait(&E);
        printf("Car %d\n", dir->car_id);
        sem_wait(&line);
        if (dir->dir_target == 'E')
        {
            sem_wait(&EE);
            time = 4;

            sem_trywait(&NN);
            nNN++;
            sem_trywait(&SS);
            nSS++;
            sem_trywait(&NE);
            nNE++;
            sem_trywait(&NW);
            nNW++;
            sem_trywait(&WS);
            nWS++;
            sem_trywait(&SE);
            nSE++;

            sem_post(&EE);
        }
        else if (dir->dir_target == 'N')
        {
            sem_wait(&EN);
            time = 5;

            sem_trywait(&NN);
            nNN++;
            sem_trywait(&SS);
            nSS++;
            sem_trywait(&WW);
            nWW++;
            sem_trywait(&NW);
            nNW++;
            sem_trywait(&WN);
            nWN++;
            sem_trywait(&SE);
            nSE++;

            sem_post(&EN);
        }
        else if (dir->dir_target == 'S')
        {
            sem_wait(&ES);
            time = 3;

            sem_trywait(&WS);
            nWS++;
            sem_trywait(&SS);
            nSS++;

            sem_post(&ES);
        }
        sem_post(&line);
        //printf("test");
        sem_post(&E);
    }
    else if (dir->dir_original == 'S')
    {
        sem_wait(&S);
        printf("Car %d\n", dir->car_id);
        sem_wait(&line);
        if (dir->dir_target == 'S')
        {
            sem_wait(&SS);
            time = 4;

            sem_trywait(&EE);
            nEE++;
            sem_trywait(&WW);
            nWW++;
            sem_trywait(&ES);
            nES++;
            sem_trywait(&WS);
            nWS++;
            sem_trywait(&NW);
            nNW++;
            sem_trywait(&EN);
            nEN++;
            sem_post(&SS);
        }
        else if (dir->dir_target == 'E')
        {
            sem_wait(&SE);
            time = 5;

            sem_trywait(&EE);
            nEE++;
            sem_trywait(&WW);
            nWW++;
            sem_trywait(&NN);
            nNN++;
            sem_trywait(&WS);
            nWS++;
            sem_trywait(&NE);
            nNE++;
            sem_trywait(&EN);
            nEN++;

            sem_post(&SE);
        }
        else if (dir->dir_target == 'W')
        {
            sem_wait(&SW);
            time = 3;

            sem_trywait(&NW);
            nNW++;
            sem_trywait(&WW);
            nWW++;

            sem_post(&SW);
        }
        sem_post(&line);
        //printf("test");
        sem_post(&S);
    }
    //sem_post(&line);
    
    return time;
}

void CrossIntersection(struct directions *dir, int time) {
   clock_t wait;
   wait = clock();
   printf("Time %.1f Car %d ( %c,  %c )         crossing\n", checkTime(currentTime),dir->car_id, dir->dir_original, dir->dir_target);
 
   while(checkTime(wait) < time);

}

void ExitIntersection(struct directions *dir) {
    if (dir->dir_original == 'N')
    {
        if (dir->dir_target == 'N')
        {
            //sem_wait(&NN);
            //time = 4;
            nEE--;
            nWW--;
            nEN--;
            nWN--;
            nSE--;
            nWS--;
            if (nEE == 0)
            {
                sem_post(&EE);
            }
            if (nWW == 0)
            {
                sem_post(&WW);
            }
            if (nEN == 0)
            {
                sem_post(&EN);
            }
            if (nWN == 0)
            {
                sem_post(&WN);
            }
            if (nSE == 0)
            {
                sem_post(&SE);
            }
            if (nWS == 0)
            {
                sem_post(&WS);
            }
            
            

            //sem_post(&NN);
        }
        else if (dir->dir_target == 'W')
        {
            //sem_wait(&NW);
            //time = 5;
            nEE--;
            nWW--;
            nSS--;
            nWS--;
            nSW--;
            nEN--;
            if (nEE == 0)
            {
                sem_post(&EE);
            }
            if (nWW == 0)
            {
                sem_post(&WW);
            }
            if (nSS == 0)
            {
                sem_post(&SS);
            }
            if (nWS == 0)
            {
                sem_post(&WS);
            }
            if (nSW == 0)
            {
                sem_post(&SW);
            }
            if (nEN == 0)
            {
                sem_post(&EN);
            }
            

            //sem_post(&NW);
        }
        else if (dir->dir_target == 'E')
        {
            //sem_wait(&NE);
            //time = 3;
            nSE--;
            nEE--;
            if (nSE == 0)
            {
                sem_post(&SE);
            }
            if (nEE == 0)
            {
                sem_post(&EE);
            }
            
            
            

            //sem_post(&NE);
        }
    }
    else if (dir->dir_original == 'W')
    {
        if (dir->dir_target == 'W')
        {
            //sem_wait(&WW);
            //time = 4;

            sem_post(&NN);
            sem_post(&SS);
            sem_post(&SW);
            sem_post(&SE);
            sem_post(&EN);
            sem_post(&NW);

            //sem_post(&WW);
        }
        else if (dir->dir_target == 'N')
        {
            //sem_wait(&WN);
            //time = 3;

            sem_post(&EN);
            sem_post(&NN);

            //sem_post(&WN);
        }
        else if (dir->dir_target == 'S')
        {
            //sem_wait(&WS);
            //time = 5;

            sem_post(&NN);
            sem_post(&SS);
            sem_post(&EE);
            sem_post(&NW);
            sem_post(&ES);
            sem_post(&SE);

            //sem_post(&WS);
        }
    }
    else if (dir->dir_original == 'E')
    {
        if (dir->dir_target == 'E')
        {
            //sem_wait(&EE);
            //time = 4;

            sem_post(&NN);
            sem_post(&SS);
            sem_post(&NE);
            sem_post(&NW);
            sem_post(&WS);
            sem_post(&SE);

            //sem_post(&EE);
        }
        else if (dir->dir_target == 'N')
        {
            //sem_wait(&EN);
            //time = 5;

            sem_post(&NN);
            sem_post(&SS);
            sem_post(&WW);
            sem_post(&NW);
            sem_post(&WS);
            sem_post(&SE);

            //sem_post(&EN);
        }
        else if (dir->dir_target == 'S')
        {
            //sem_wait(&ES);
            //time = 3;

            sem_post(&WS);
            sem_post(&SS);

            //sem_post(&ES);
        }
    }
    else if (dir->dir_original == 'S')
    {
        if (dir->dir_target == 'S')
        {
            //sem_wait(&SS);
            //time = 4;
            nEE--;
            nWW--;
            nES--;
            nWS--;
            nNW--;
            nEN--;
            if (nEE == 0)
            {
                sem_post(&EE);
            }
            if (nWW == 0)
            {
                sem_post(&WW);
            }
            if (nES == 0)
            {
                sem_post(&ES);
            }
            if (nWS == 0)
            {
                sem_post(&WS);
            }
            if (nNW == 0)
            {
                sem_post(&NW);
            }
            if (nEN == 0)
            {
                sem_post(&EE);
            }
            
            
            
            //sem_post(&SS);
        }
        else if (dir->dir_target == 'E')
        {
            //sem_wait(&SE);
            //time = 5;

            sem_post(&EE);
            sem_post(&WW);
            sem_post(&NN);
            sem_post(&WS);
            sem_post(&NE);
            sem_post(&EN);

            //sem_post(&SE);
        }
        else if (dir->dir_target == 'W')
        {
            //sem_wait(&SW);
            //time = 3;

            sem_post(&NW);
            sem_post(&WW);

            //sem_post(&SW);
        }
    }
    printf("Time %.1f Car %d ( %c,  %c )                 exiting\n", checkTime(currentTime),dir->car_id, dir->dir_original, dir->dir_target);
   //print exiting and unlock everything
}

void threadfunc(struct directions *dir) {
    int time = 0;
    time = ArriveIntersection(dir);
    CrossIntersection(dir, time);
    ExitIntersection(dir);
    //struct directions *dir = temp;
    
}

int main(void) {
    
    currentTime = clock();

    // initialize semaphore, only to be used with threads in this process, set value to 1
    //sem_init(&semaphore, 0, 8);
    
    sem_init(&NN , 0, 1);
    sem_init(&EE , 0, 1);
    sem_init(&SS , 0, 1);
    sem_init(&WW , 0, 1);
    sem_init(&NW , 0, 1);
    sem_init(&EN , 0, 1);
    sem_init(&SE , 0, 1);
    sem_init(&WS , 0, 1);
    sem_init(&NE , 0, 1);
    sem_init(&ES , 0, 1);
    sem_init(&SW , 0, 1);
    sem_init(&WN , 0, 1);
    sem_init(&N , 0, 1);
    sem_init(&E , 0, 1);
    sem_init(&S , 0, 1);
    sem_init(&W , 0, 1);
    sem_init(&line , 0, 1);
    pthread_t *one;
    pthread_t *two;
    pthread_t *three;
    pthread_t *four;
    pthread_t *five;
    pthread_t *six;
    pthread_t *seven;
    pthread_t *eight;
    
    one = (pthread_t *)malloc(sizeof(*one));
    two = (pthread_t *)malloc(sizeof(*two));
    three = (pthread_t *)malloc(sizeof(*three));
    four = (pthread_t *)malloc(sizeof(*four));
    five = (pthread_t *)malloc(sizeof(*five));
    six = (pthread_t *)malloc(sizeof(*six));
    seven = (pthread_t *)malloc(sizeof(*seven));
    eight = (pthread_t *)malloc(sizeof(*eight));

    struct directions arg;
    arg.dir_original = 'N';
    arg.dir_target = 'N';
    arg.car_id = 1;
    struct directions* car_one = malloc(sizeof * car_one);
    *car_one = arg;
    
    while(checkTime(currentTime) < 1.1);
    pthread_create(one, NULL, (void*)threadfunc, car_one);
    
    arg.dir_original = 'N';
    arg.dir_target = 'N';
    arg.car_id = 2;
    struct directions* car_two = malloc(sizeof * car_two);
    *car_two = arg;
   
    while(checkTime(currentTime) < 2.2);
    pthread_create(two, NULL, (void*)threadfunc, car_two);

    arg.dir_original = 'N';
    arg.dir_target = 'W';
    arg.car_id = 3;
    struct directions* car_three = malloc(sizeof * car_three);
    *car_three = arg;
    
    while(checkTime(currentTime) < 3.3);
    pthread_create(three, NULL, (void*)threadfunc, car_three);

    arg.dir_original = 'S';
    arg.dir_target = 'S';
    arg.car_id = 4;
    struct directions* car_four = malloc(sizeof * car_four);
    *car_four = arg;
    
    while(checkTime(currentTime) < 4.4);
    pthread_create(four, NULL, (void*)threadfunc, car_four);

    arg.dir_original = 'S';
    arg.dir_target = 'E';
    arg.car_id = 5;
    struct directions* car_five = malloc(sizeof * car_five);
    *car_five = arg;
    
    while(checkTime(currentTime) < 5.5);
    pthread_create(five, NULL, (void*)threadfunc, car_five);

    arg.dir_original = 'N';
    arg.dir_target = 'N';
    arg.car_id = 6;
    struct directions* car_six = malloc(sizeof * car_six);
    *car_six = arg;
    
    while(checkTime(currentTime) < 6.6);
    pthread_create(six, NULL, (void*)threadfunc, car_six);

    arg.dir_original = 'E';
    arg.dir_target = 'N';
    arg.car_id = 7;
    struct directions* car_seven = malloc(sizeof * car_seven);
    *car_seven = arg;
    
    while(checkTime(currentTime) < 7.7);
    pthread_create(seven, NULL, (void*)threadfunc, car_seven);

    arg.dir_original = 'W';
    arg.dir_target = 'N';
    arg.car_id = 8;
    struct directions* car_eight = malloc(sizeof * car_eight);
    *car_eight = arg;
    
    while(checkTime(currentTime) < 8.8);
    pthread_create(eight, NULL, (void*)threadfunc, car_eight);

   // getchar();
    
    //sem_wait(&semaphore);
    //printf("Semaphore locked.\n");
    
    // do stuff with whatever is shared between threads
   // getchar();
    
   // printf("Semaphore unlocked.\n");
   // sem_post(&semaphore);
    
    getchar();
    
    return 0;
}