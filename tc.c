#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semaphore;

sem_t NSI, NSO, NSM;            // north-side in, north-side out, north-side mid
sem_t SSI, SSO, SSM;            // south-side in, south-side out, north-side mid
sem_t ESI, ESO, ESM;            // east-side in, east-side out, east-side mid
sem_t WSI, WSO, WSM;            // west-side in, west-side out, west-side mid
sem_t NWM, NEM, SEM, SWM;       // northwest mid, northeast mid, southeast mid, southwest mid
sem_t* SEM_ARRAY[] = {&NSI, &NSO, &NSM, &SSI, &SSO, &SSM, &ESI, &ESO, &ESM, &WSI, &WSO, &WSM, &NWM, &NEM, &SEM, &SWM};

typedef struct _directions{
char dir_original;
char dir_target;
} directions;

// void threadfunc() {
//     while (1) {
//         sem_wait(&semaphore);
//         printf("Hello from da thread!\n");
//         sem_post(&semaphore);
//         sleep(1);
//     }
// }

void blockIntersection(directions d){
    printf("%c", d.dir_original);
    printf("%c", d.dir_target);
    if (d.dir_original == 'N'){
        // RIGHT TURN IN NORTH DIRECTION
        if (d.dir_target == "E"){
            sem_wait(&SSI);
            sem_wait(&ESO);
            return;
        
        // STRAIGHT IN NORTH DIRECTION
        } else if (d.dir_target == 'N'){
            printf("Working correctly");
            sem_wait(&SSI);
            sem_wait(&SEM);
            sem_wait(&ESM);
            sem_wait(&NEM);
            sem_wait(&NSO);
            return;

        // LEFT TURN IN NORTH DIRECTION
        } else if (d.dir_target == "W"){
            sem_wait(&SSI);
            sem_wait(&SSM);
            sem_wait(&WSM);
            sem_wait(&WSO);
            return;
        }
    } else if (d.dir_original == "E"){
        if (d.dir_target == "S"){
            sem_wait(&WSI);
            sem_wait(&SSO);
            return;

        } else if (d.dir_target == "E"){
            sem_wait(&WSI);
            sem_wait(&SWM);
            sem_wait(&SSM);
            sem_wait(&SEM);
            sem_wait(&ESO);
            return;

        } else if (d.dir_target == "N"){
            sem_wait(&WSI);
            sem_wait(&WSM);
            sem_wait(&NSM);
            sem_wait(&NSO);
            return;
        }
    } else if (d.dir_original == "S"){
        if (d.dir_target == "W"){
            sem_wait(&NSI);
            sem_wait(&WSO);
            return;

        } else if (d.dir_target == "S"){
            sem_wait(&NSI);
            sem_wait(&NWM);
            sem_wait(&WSM);
            sem_wait(&SWM);
            sem_wait(&SSO);
            return;

        } else if (d.dir_target == "E"){
            sem_wait(&NSI);
            sem_wait(&NSM);
            sem_wait(&ESM);
            sem_wait(&ESO);
            return;
        }
    } else if (d.dir_original == "W"){
        if (d.dir_target == "S"){
            sem_wait(&ESI);
            sem_wait(&ESM);
            sem_wait(&SSM);
            sem_wait(&SSO);
            return;

        } else if (d.dir_target == "W"){
            sem_wait(&ESI);
            sem_wait(&NEM);
            sem_wait(&NSM);
            sem_wait(&NWM);
            sem_wait(&WSO);
            return;

        } else if (d.dir_target == "N"){
            sem_wait(&ESI);
            sem_wait(&NSO);
            return;

        } 
    } else {
        printf("\nYou passed everything, something is broken...........");
    }
}

void unblockIntersection(directions d){
        if (d.dir_original == "N"){
        // RIGHT TURN IN NORTH DIRECTION
        if (d.dir_target == "E"){
            sem_post(&SSI);
            sem_post(&ESO);
            return;
        
        // STRAIGHT IN NORTH DIRECTION
        } else if (d.dir_target == "N"){
            sem_post(&SSI);
            sem_post(&SEM);
            sem_post(&ESM);
            sem_post(&NEM);
            sem_post(&NSO);
            return;

        // LEFT TURN IN NORTH DIRECTION
        } else if (d.dir_target == "W"){
            sem_post(&SSI);
            sem_post(&SSM);
            sem_post(&WSM);
            sem_post(&WSO);
            return;
        }
    } else if (d.dir_original == "E"){
        if (d.dir_target == "S"){
            sem_post(&WSI);
            sem_post(&SSO);
            return;

        } else if (d.dir_target == "E"){
            sem_post(&WSI);
            sem_post(&SWM);
            sem_post(&SSM);
            sem_post(&SEM);
            sem_post(&ESO);
            return;

        } else if (d.dir_target == "N"){
            sem_post(&WSI);
            sem_post(&WSM);
            sem_post(&NSM);
            sem_post(&NSO);
            return;
        }
    } else if (d.dir_original == "S"){
        if (d.dir_target == "W"){
            sem_post(&NSI);
            sem_post(&WSO);
            return;

        } else if (d.dir_target == "S"){
            sem_post(&NSI);
            sem_post(&NWM);
            sem_post(&WSM);
            sem_post(&SWM);
            sem_post(&SSO);
            return;

        } else if (d.dir_target == "E"){
            sem_post(&NSI);
            sem_post(&NSM);
            sem_post(&ESM);
            sem_post(&ESO);
            return;
        }
    } else if (d.dir_original == "W"){
        if (d.dir_target == "S"){
            sem_post(&ESI);
            sem_post(&ESM);
            sem_post(&SSM);
            sem_post(&SSO);
            return;

        } else if (d.dir_target == "W"){
            sem_post(&ESI);
            sem_post(&NEM);
            sem_post(&NSM);
            sem_post(&NWM);
            sem_post(&WSO);
            return;

        } else if (d.dir_target == "N"){
            sem_post(&ESI);
            sem_post(&NSO);
            return;

        } 
    } else {
        printf("\nYou passed everything, something is broken...........");
    }
}

void ArriveIntersection(directions d){
    printf("arriving");
    sleep(2);
    // wait head_of_line lock
    // Lock dir_target;
    // lock  dir_original;
    blockIntersection(d);
}

void CrossIntersection(directions d){
    //Unlock head lock
    if (d.dir_original == "N"){
        sem_post(&SSI);
    } else if (d.dir_original == "E"){
        sem_post(&WSI);
    } else if (d.dir_original == "S"){
        sem_post(&NSI);
    } else if (d.dir_original = "W"){
        sem_post(&ESI);
    } else {
        printf("This is fucked");
    }
    printf("crossing");
    sleep(4); // x= left, right, straight

}

void ExitIntersection(directions d){
    printf("exiting");
    // Unlock dir_target
    unblockIntersection(d);
}

void * Car(directions dir) {
    ArriveIntersection(dir);
    CrossIntersection(dir);
    ExitIntersection(dir);
}

int main(void) {
    
    // initialize semaphore, only to be used with threads in this process, set value to 1
    for(int i = 0; i < 16; i++){
        sem_init(SEM_ARRAY[i], 0, 1);
    }

    directions p1;
    p1.dir_original = 'N';
    p1.dir_target = 'N';

    pthread_t *mythread;
    
    mythread = (pthread_t *)malloc(sizeof(*mythread));
    
    pthread_create(mythread, NULL, (void*)Car, &p1);
    
    getchar();
    
    return 0;
}