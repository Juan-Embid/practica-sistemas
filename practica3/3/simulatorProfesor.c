#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>

#define N_PARADAS 5     // número de paradas de la ruta
#define MAX_USUARIOS 40 // capacidad del autobús
#define USUARIOS 4      // numero de usuarios

// Estados del autobus
#define EN_RUTA 0       // en ruta
#define EN_PARADA 1     // en la parada

int estado        = EN_RUTA;
int parada_actual = 0;       // parada en la que se encuentra el autobus
int n_ocupantes   = 0;       // ocupantes que tiene el autobús
int capacidad     = MAX_USUARIOS;
int usuarios      = USUARIOS;
int paradas       = N_PARADAS;

// personas que desean subir/bajar en cada parada
int *esperando_parada = NULL;
int *esperando_bajar = NULL;

pthread_mutex_t mtx;
pthread_cond_t enparada, esperando;

void Autobus_En_Parada()
{
	pthread_mutex_lock(&mtx);
	while ((esperando_parada[parada_actual] && (n_ocupantes < capacidad))
			|| esperando_bajar[parada_actual]) {
		printf("En parada %d: esperando subir %d esperando bajar %d\n",
				parada_actual, esperando_parada[parada_actual],
				esperando_bajar[parada_actual]);

		pthread_cond_broadcast(&enparada);
		pthread_cond_wait(&esperando, &mtx);
	}
	pthread_mutex_unlock(&mtx);
}

void Conducir_Hasta_Siguiente_Parada()
{
	int n;
	pthread_mutex_lock(&mtx);
	estado = EN_RUTA;
	n = 1 + (random() % 9);
	printf("Autobús se va de la parada %d (trayecto %d seg, Ocupantes %d)\n",
			parada_actual, n, n_ocupantes);
	pthread_mutex_unlock(&mtx);

    sleep(n);

	pthread_mutex_lock(&mtx);
	estado = EN_PARADA;
	parada_actual = (parada_actual + 1) % paradas;
	printf("Autobús llega a la parada %d\n", parada_actual);
	pthread_mutex_unlock(&mtx);
}

void Subir_Autobus(int id_usuario, int origen)
{
	pthread_mutex_lock(&mtx);
	printf("usuario %d esperando en parada %d\n", id_usuario, origen);
	esperando_parada[origen]++;
	while (n_ocupantes >= capacidad || estado != EN_PARADA || parada_actual != origen)
		pthread_cond_wait(&enparada, &mtx);

	n_ocupantes++;
	esperando_parada[origen]--;
	printf("usuario %d sube al autobus en parada %d\n", id_usuario, origen);

	if (esperando_parada[origen] == 0)
		pthread_cond_signal(&esperando);
	pthread_mutex_unlock(&mtx);
}

void Bajar_Autobus(int id_usuario, int destino)
{
	pthread_mutex_lock(&mtx);
	printf("usuario %d esparando a bajar en %d\n", id_usuario, destino);
	esperando_bajar[destino]++;
	while (estado != EN_PARADA || parada_actual != destino)
		pthread_cond_wait(&enparada, &mtx);

	esperando_bajar[destino]--;
	n_ocupantes--;
	printf("usuario %d baja del autobus en parada %d\n", id_usuario, destino);

	if (esperando_bajar[destino] == 0)
		pthread_cond_signal(&esperando);
	pthread_mutex_unlock(&mtx);
}