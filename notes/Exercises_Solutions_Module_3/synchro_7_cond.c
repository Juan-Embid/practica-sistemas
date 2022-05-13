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

pthread_mutex_t mutex           = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var_empty   = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_var_full    = PTHREAD_COND_INITIALIZER;


int pot_servings = 0;
pthread_mutex_t mutex;

void *cook(void * arg){

    while(1){
        pthread_mutex_lock(&mutex);
        if (pot_servings != 0)
            pthread_cond_wait(&cond_var_empty, &mutex);

        printf("Cook awake!!\n");
        printf("Cook cooking %d servings!!\n", M);
        printf("Cook asleep!!\n\n");
        pot_servings = M;
        pthread_cond_signal(&cond_var_full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *savage(void * arg){

    while(1){
        pthread_mutex_lock(&mutex);
        if (pot_servings == 0){
            pthread_cond_signal(&cond_var_empty);
            pthread_cond_wait(&cond_var_full, &mutex);

        }

        printf("Savage eating!!\n");
        pot_servings--;
        printf("Pot: %d servings!\n\n", pot_servings);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{   
    pthread_t th_savage[NUMBER_OF_SAVAGES], th_cook;

    for(int i = 0; i < NUMBER_OF_SAVAGES; i++) {
        pthread_create(&th_savage[i], NULL, (void *)savage, NULL);
    }
    pthread_create(&th_cook, NULL, (void *)cook, NULL);

    for(int i = 0; i < M; i++) {
        pthread_join(th_savage[i], NULL);
    }
    pthread_join(th_cook, NULL);
    

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var_empty);
    pthread_cond_destroy(&cond_var_full);

    return 0;
    
}