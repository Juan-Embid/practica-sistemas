// Universidad Complutense
// Operating Systems
// Communication and Synchronitation Exercises Sheet - Exercise 7
// Prof: Jose Manuel Velasco

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define M 10 // M = Number of Servings
#define NUMBER_OF_SAVAGES 20 

sem_t sem_pot_empty;
sem_t sem_pot_full;

int pot = 0;
pthread_mutex_t mutex;

void *cook(void * arg){

    while(1){
        sem_wait(&sem_pot_empty);
        printf("Cook awake!!\n");
        printf("Cook cooking %d servings!!\n", M);
        printf("Cook asleep!!\n\n");
        pot = M;
        sem_post(&sem_pot_full);
    }
    return NULL;
}

void *savage(void * arg){

    while(1){
        pthread_mutex_lock(&mutex);
        if (pot==0){
            sem_post(&sem_pot_empty);
            sem_wait(&sem_pot_full); 
        }
        printf("Savage eating!!\n");
        pot--;
        printf("Pot: %d servings!\n\n", pot);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{   
    pthread_t th_savage[NUMBER_OF_SAVAGES], th_cook;
    
    sem_init(&sem_pot_empty, 0, 0);
    sem_init(&sem_pot_full, 0, 0);


    for(int i = 0; i < NUMBER_OF_SAVAGES; i++) {
        pthread_create(&th_savage[i], NULL, (void *)savage, NULL);
    }
    pthread_create(&th_cook, NULL, (void *)cook, NULL);

    for(int i = 0; i < M; i++) {
        pthread_join(th_savage[i], NULL);
    }
    pthread_join(th_cook, NULL);
    

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_pot_empty);
    sem_destroy(&sem_pot_full);

    return 0;
    
}