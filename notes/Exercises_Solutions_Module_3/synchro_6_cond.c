// Universidad Complutense
// Operating Systems
// Communication and Synchronitation Exercises Sheet - Exercise 6
// Prof: Jose Manuel Velasco

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex           = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var_paper   = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_var_matches = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_var_tobacco = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_var_agent   = PTHREAD_COND_INITIALIZER;

enum turn_type{matches, tobacco, paper, agent};
enum turn_type turn = agent;

const char *ingredients_array[3] = {"Tobacco and Paper", "Paper and Matches", "Matches and Tobacco"};

void *smoker_matches(void * arg){

    while(1){
        pthread_mutex_lock(&mutex);
        if (turn != matches)
            pthread_cond_wait(&cond_var_matches, &mutex);

        printf("Smoker-Matches: smoking!!\n");
        turn = agent;
        pthread_cond_signal(&cond_var_agent);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *smoker_tobacco(void * arg){

    while(1){
        pthread_mutex_lock(&mutex);
        if (turn != tobacco)
            pthread_cond_wait(&cond_var_tobacco, &mutex);

        printf("Smoker-Tobacco: smoking!!\n");
        turn = agent;
        pthread_cond_signal(&cond_var_agent);
        pthread_mutex_unlock(&mutex);    }
    return NULL;
}

void *smoker_paper(void * arg){

    while(1){
        pthread_mutex_lock(&mutex);
        if (turn != paper)
            pthread_cond_wait(&cond_var_paper, &mutex);

        printf("Smoker-Paper: smoking!!\n");
        turn = agent;
        pthread_cond_signal(&cond_var_agent);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *agent_table(void * arg){

    while(1){
        pthread_mutex_lock(&mutex);
        if (turn != agent)
            pthread_cond_wait(&cond_var_agent, &mutex);
        int r = rand() % 3;
        printf("\n%s on the table!!\n", ingredients_array[r]);
        switch( r ) {
            case 0: 
                turn = matches;
                pthread_cond_signal(&cond_var_matches);
                break;
            case 1: 
                turn = tobacco;
                pthread_cond_signal(&cond_var_tobacco);
                break;
            case 2: 
                turn = paper;
                pthread_cond_signal(&cond_var_paper);
                break;
        }
        pthread_mutex_unlock(&mutex); 
    }
    return NULL;
}

int main()
{   

    pthread_t th_smoker_paper, th_smoker_matches, th_smoker_tobacco, th_agent;
    
    pthread_create(&th_smoker_paper, NULL, (void *)smoker_paper, NULL);
    pthread_create(&th_smoker_matches, NULL, (void *)smoker_matches, NULL);
    pthread_create(&th_smoker_tobacco, NULL, (void *)smoker_tobacco, NULL);
    pthread_create(&th_agent, NULL, (void *)agent_table, NULL);
   
    pthread_join(th_smoker_paper, NULL);
    pthread_join(th_smoker_matches, NULL);
    pthread_join(th_smoker_tobacco, NULL);
    pthread_join(th_agent, NULL);

    pthread_cond_destroy(&cond_var_paper);
    pthread_cond_destroy(&cond_var_tobacco);
    pthread_cond_destroy(&cond_var_matches);
    pthread_cond_destroy(&cond_var_agent);

    return 0;
    
}






