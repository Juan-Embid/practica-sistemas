// Universidad Complutense
// Operating Systems
// Communication and Synchronitation Exercises Sheet - Exercise 6
// Prof: Jose Manuel Velasco

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem_tobacco_paper;
sem_t sem_paper_matches;
sem_t sem_matches_tobacco;
sem_t sem_table_ready;

const char *ingredients_array[3] = {"Tobacco and Paper", "Paper and Matches", "Matches and Tobacco"};

void *smoker_matches(void * arg){

    while(1){
        sem_wait(&sem_tobacco_paper); 
        printf("Smoker-Matches: smoking!!\n");
        sem_post(&sem_table_ready);
    }
    return NULL;
}

void *smoker_tobacco(void * arg){

    while(1){
        sem_wait(&sem_paper_matches); 
        printf("Smoker-Tobacco: smoking!!\n");
        sem_post(&sem_table_ready);
    }
    return NULL;
}

void *smoker_paper(void * arg){

    while(1){
        sem_wait(&sem_matches_tobacco); 
        printf("Smoker-Paper: smoking!!\n");
        sem_post(&sem_table_ready);
    }
    return NULL;
}

void *agent(void * arg){

    while(1){
        sem_wait(&sem_table_ready); 
        int r = rand() % 3;
        printf("%s on the table!!\n", ingredients_array[r]);
        switch( r ) {
            case 0: 
                sem_post(&sem_tobacco_paper);
                break;
            case 1: 
                sem_post(&sem_paper_matches);
                break;
            case 2: 
                sem_post(&sem_matches_tobacco);
                break;
        }
    }
    return NULL;
}

int main()
{   

    pthread_t th_smoker_paper, th_smoker_matches, th_smoker_tobacco, th_agent;
  
    sem_init(&sem_tobacco_paper, 0, 0);
    sem_init(&sem_paper_matches, 0, 0);
    sem_init(&sem_matches_tobacco, 0, 0);
    sem_init(&sem_table_ready, 0, 1);
    

    pthread_create(&th_smoker_paper, NULL, (void *)smoker_paper, NULL);
    pthread_create(&th_smoker_matches, NULL, (void *)smoker_matches, NULL);
    pthread_create(&th_smoker_tobacco, NULL, (void *)smoker_tobacco, NULL);
    pthread_create(&th_agent, NULL, (void *)agent, NULL);
   
    pthread_join(th_smoker_paper, NULL);
    pthread_join(th_smoker_matches, NULL);
    pthread_join(th_smoker_tobacco, NULL);
    pthread_join(th_agent, NULL);

    sem_destroy(&sem_tobacco_paper);
    sem_destroy(&sem_paper_matches);
    sem_destroy(&sem_matches_tobacco);
    sem_destroy(&sem_table_ready);

    return 0;
    
}






