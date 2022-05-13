// Universidad Complutense
// Operating Systems
// Communication and Synchronitation Exercises Sheet - Exercise 2
// Prof: Jose Manuel Velasco

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_NUMBER 2000 

sem_t sem_odd;
sem_t sem_even;
sem_t sem_cons;
int data;

void *producer_odd(void * arg){
  int i;

  for(i=1; i<=MAX_NUMBER; i+=2){
    sem_wait(&sem_odd); 
    data = i;
    printf("Producer_odd: %d\n", data);
    sem_post(&sem_cons);
  }
  return NULL;
}

void *producer_even(void * arg){
  int i;

  for(i=2; i<=MAX_NUMBER; i+=2){
    sem_wait(&sem_even); 
    data = i;
    printf("Producer_even: %d\n", data);
    sem_post(&sem_cons);
  }
  return NULL;
}

void *consumer(void * arg){
  int i;

  for(i=1; i<=MAX_NUMBER; i++){
    sem_wait(&sem_cons); 
    printf("Consumer: %d - %d\n", i, data);
	  if(data%2)
	    sem_post(&sem_even); 
	  else
	    sem_post(&sem_odd); 
  }
  return NULL;
}

int main()
{   

    pthread_t th_odd, th_even, th_consumer;
  
    sem_init(&sem_even, 0, 0);
    sem_init(&sem_odd, 0, 1);
    sem_init(&sem_cons, 0, 0);


    pthread_create(&th_odd, NULL, (void *)producer_odd, NULL);
    pthread_create(&th_even, NULL, (void *)producer_even, NULL);
    pthread_create(&th_consumer, NULL, (void *)consumer, NULL);
   
    pthread_join(th_odd, NULL);
    pthread_join(th_even, NULL);
    pthread_join(th_consumer, NULL);

    sem_destroy(&sem_even);
    sem_destroy(&sem_odd);
    sem_destroy(&sem_cons);

    return 0;
    
}
