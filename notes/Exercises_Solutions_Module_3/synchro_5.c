// Universidad Complutense
// Operating Systems
// Communication and Synchronitation Exercises Sheet - Exercise 5
// Prof: Jose Manuel Velasco

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <semaphore.h>
#include <string.h>

#define NUM_PHILOSOPHERS 5
#define NUM_ITERATIONS   5

#define LEFT  (i + NUM_PHILOSOPHERS - 1)%NUM_PHILOSOPHERS
#define RIGHT (i + 1)%NUM_PHILOSOPHERS

enum state_type{THINKING, HUNGRY, EATING};
enum state_type state[NUM_PHILOSOPHERS];

const char *philosopher_array[5] = {"Parmenides", "Heraclitus", "Socrates", "Plato", "Aristotle"};
const char *state_array[3] = {"Thinking", "Hungry", "Eating"};

sem_t mutex;
sem_t next;
int next_count = 0;

typedef struct cond_philosopher{
	sem_t sem;
	int count;
};
struct cond_philosopher cond_phil[NUM_PHILOSOPHERS];

int turn[NUM_PHILOSOPHERS];

void wait(int i)
{
	cond_phil[i].count++;
	if(next_count > 0) sem_post(&next);
	else sem_post(&mutex);
	
	sem_wait(&cond_phil[i].sem);
	cond_phil[i].count--;
}

void signal(int i)
{
	if(cond_phil[i].count > 0)
	{
		next_count++;
		
		sem_post(&cond_phil[i].sem);

		sem_wait(&next);

		next_count--;
	}
}
void test(int i)
{
	if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING && turn[i] == i && turn[LEFT] == i)
	{
		state[i] = EATING;

		signal(i);
	}
}

void getChopsticks(int i)
{
	sem_wait(&mutex);
	state[i] = HUNGRY;
	printf("%s is Hungry!!\n", philosopher_array[i]);
	printf("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
		for(int z = 0;z < NUM_PHILOSOPHERS; z++){
			printf("@ %s is %s\n", philosopher_array[z], state_array[state[z]]);
		}
		printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n");
	test(i);
	while(state[i] == HUNGRY) wait(i);
	if(next_count > 0) sem_post(&next);
	else sem_post(&mutex);
	
}

void putChopsticks(int i)
{
	sem_wait(&mutex);
	state[i] = THINKING;
	printf("%s returns to think!\n", philosopher_array[i]);

	printf("\n#####################################################\n");
	for(int z = 0;z < NUM_PHILOSOPHERS; z++){
		printf("# %s is %s\n", philosopher_array[z], state_array[state[z]]);
	}
	printf("#####################################################\n\n");

	turn[i] = RIGHT;
	turn[LEFT] = LEFT;

	test(LEFT);
	test(RIGHT);

	if(next_count > 0) sem_post(&next);
	else sem_post(&mutex);

}

void *philosopher(void *i)
{
	int j = 0;	
	while(j < NUM_ITERATIONS)
	{
		 
		int philosopher_number = *(int *) i;
		int j, k;
		j = rand();
		j = 1 + j % 9;
		printf("%s is going to think for %d secs\n", philosopher_array[philosopher_number], j);
	
		sleep(j);
		
		getChopsticks(philosopher_number);
		k = rand();
		k = 1 + k % 9;
		printf("%s is going to eat for %d secs\n", philosopher_array[philosopher_number], k);
		
		sleep(k);
		
		putChopsticks(philosopher_number);
		j++;
	}

}

int main()
{
	int i, pos[NUM_PHILOSOPHERS];
	pthread_t thread[NUM_PHILOSOPHERS];

	sem_init(&mutex,0,1);
	sem_init(&next,0,0);
	for(i = 0;i < NUM_PHILOSOPHERS; i++)
	{
		state[i] = THINKING;
		sem_init(&cond_phil[i].sem, 0, 0);
		cond_phil[i].count = 0;
		turn[i]    = i;
	}
	turn[1] = 2;
	turn[3] = 4;
	
	for (i = 0; i < NUM_PHILOSOPHERS; i++) 
	{
		pos[i] = i;
		pthread_create(&thread[i], NULL,philosopher, (int *) &pos[i]);
	}
	for (i = 0; i < NUM_PHILOSOPHERS; i++) pthread_join(thread[i], NULL);

	return 0;
}
