// Universidad Complutense
// Operating Systems
// Communication and Synchronitation Exercises Sheet - Exercise 2
// Prof: Jose Manuel Velasco

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


#define MAX_NUMBER 2000 

pthread_mutex_t mutex        = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var_odd  = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_var_even = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_var_cons = PTHREAD_COND_INITIALIZER;

int data;
enum turn_type{odd, even, cons};
enum turn_type turn = odd;

void *producer_odd(void * arg){
  int i;

  for(i=1; i<=MAX_NUMBER; i+=2){
    pthread_mutex_lock(&mutex);
    if (turn != odd)
        pthread_cond_wait(&cond_var_odd, &mutex);
    data = i;
    printf("Producer_odd: %d\n", data);

    turn = cons;
    pthread_cond_signal(&cond_var_cons);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void *producer_even(void * arg){
  int i;

  for(i=2; i<=MAX_NUMBER; i+=2){
    pthread_mutex_lock(&mutex);
    if (turn != even)
        pthread_cond_wait(&cond_var_even, &mutex);
    data = i;
    printf("Producer_even: %d\n", data);
    turn = cons;
    pthread_cond_signal(&cond_var_cons);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void *consumer(void * arg){
  int i;

  for(i=0; i<MAX_NUMBER; i++){
    pthread_mutex_lock(&mutex);
    if (turn != cons)
        pthread_cond_wait(&cond_var_cons, &mutex);
    
    printf("Consumer: %d\n", data);
     
	  if(data%2){
        turn = even;
        pthread_cond_signal(&cond_var_even);
    } else{
        turn = odd;
        pthread_cond_signal(&cond_var_odd);
    }
    pthread_mutex_unlock(&mutex);
    
  }
  return NULL;
}

int main()
{   

    pthread_t th_odd, th_even, th_consumer;

    pthread_create(&th_odd, NULL, producer_odd, NULL);
    pthread_create(&th_even, NULL, producer_even, NULL);
    pthread_create(&th_consumer, NULL, consumer, NULL);

    pthread_join(th_odd, NULL);
    pthread_join(th_even, NULL);
    pthread_join(th_consumer, NULL);

    pthread_mutex_destroy(&mutex);

    pthread_cond_destroy(&cond_var_odd);
    pthread_cond_destroy(&cond_var_even);
    pthread_cond_destroy(&cond_var_cons);

    return 0;
    
}
