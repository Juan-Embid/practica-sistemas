#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>

#define NR_INDIANOS 10
#define NR_MOLRAMS 4

int molRamsEnPuente;
int indianosEnPuente;
// int indianos = 0, molRam = 0;

pthread_t th_indiano[NR_INDIANOS], th_molRam[NR_MOLRAMS];
pthread_mutex_t mtx[NR_INDIANOS + NR_MOLRAMS];
pthread_cond_t esperando_ind, esperando_mol;

void* indiano(void * arg) {
	int index = (int) arg;

	pthread_mutex_lock(&mtx[index]);
	// comprobar si cruza el puente
	while (molRamsEnPuente != 0 || (indianosEnPuente + molRamsEnPuente) >= 4) {
		printf("Indiano %d se queda a la espera\n", index);
		// esperar
		//pthread_cond_broadcast(&esperando_mol);
		pthread_cond_wait(&esperando_ind, &mtx[index]);
	}

	printf("Indiano %d entra en el puente\n", index);
	indianosEnPuente++;
	sleep(2);

	printf("Indiano %d sale del puente\n", index);
	indianosEnPuente--;

	if(indianosEnPuente + molRamsEnPuente == 1) {
		// avisar a todos los que esperan
		// pthread_cond_signal(&esperando[index]);
		pthread_cond_broadcast(&esperando_ind);
	}

	pthread_mutex_unlock(&mtx[index]);
	//indianos++;
}

void* molRams(void * arg) {
	int index = (int) arg;

	pthread_mutex_lock(&mtx[index]);
	// comprobar si puede cruzar el puente
	while(molRamsEnPuente != 0 || (indianosEnPuente + molRamsEnPuente) >= 4 || indianosEnPuente != 0) {
		printf("MolRam %d se queda a la espera\n", index);
		// esperar
		//pthread_cond_broadcast(&esperando_ind);
		pthread_cond_wait(&esperando_mol, &mtx[index]);
	}

	printf("MolRam %d entra en el puente\n", index);
	molRamsEnPuente++;
	sleep(3);

	printf("MolRam %d sale del puente\n", index);
	molRamsEnPuente--;

	if(indianosEnPuente + molRamsEnPuente == 1) {
		// avisar a todos los que esperan
		// pthread_cond_signal(&esperando[index]);
		pthread_cond_broadcast(&esperando_mol);	
	}

	pthread_mutex_unlock(&mtx[index]);
	//molRam++;
}

int main() {

	unsigned long i;

	for(i=0; i<NR_INDIANOS + NR_MOLRAMS; i++)
        pthread_mutex_init(&mtx[i],NULL);

	// create
	for(i=0; i<NR_INDIANOS; i++)
        pthread_create(&th_indiano[i], NULL, indiano, (void*)i);
    for(i=0; i<NR_MOLRAMS; i++)
    	pthread_create(&th_molRam[i], NULL, molRams, (void*)i);

    // joins
	for(i=0; i<NR_INDIANOS; i++)
		pthread_join(th_indiano[i], NULL);
    for(i=0; i<NR_MOLRAMS; i++)
		pthread_join(th_molRam[i], NULL);

	pthread_cond_destroy(&esperando_ind);
	pthread_cond_destroy(&esperando_mol);

	return 0;
}