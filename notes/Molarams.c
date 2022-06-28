#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>

#define N_INDIANOS 10 // indianos al inicio de la ejecucion
#define MAX_USUARIOS 4 // capacidad del puente
#define N_MOLA_RAMS 4 // numero de molaRams al inicio de la ejecución


int IndianosEnPuente = 0; //indianos en puente actualmente
int MolaRamsEnPuente = 0;       // mola rams en el puente actualmente
int Indianos = N_INDIANOS; //indianos restantes //molaRmans restantes
int MolaRams = N_MOLA_RAMS;
int IdIndiano = 1, IdMolaRam = 1;//indices

pthread_mutex_t mtx; //mutex
pthread_cond_t esperando/*soy un indiano y quiero cruzar el puente*/, indianospuente/*soy un mola esperando a que un indiano entre al puente*/;



void* indiano(int id) {
    int index = id;
    /* comprobar si puede cruzar el puente */
   pthread_mutex_lock(&mtx); // bloqueamos el mutex
    while (MolaRamsEnPuente > 0 || IndianosEnPuente == MAX_USUARIOS) {
        printf("Indiano %d se queda a la espera\n", index);
        pthread_cond_wait(&esperando, &mtx);
       // pthread_mutex_unlock(&mtx);
    }
    IndianosEnPuente++;
    printf("Indiano %d entra en el puente\n", index);
    pthread_mutex_unlock(&mtx);
    pthread_cond_broadcast(&indianospuente); //si entra 1 indiano hace un broadcast para los molarams que estaban esoerando
    sleep(2);
    IndianosEnPuente--;
    Indianos--;
    printf("Indiano %d sale del puente\n", index);
    if(MolaRamsEnPuente + IndianosEnPuente == 0)
        pthread_cond_broadcast(&esperando);
    pthread_mutex_unlock(&mtx);
	
}



void* molaRams(int id) {
    int index = id;
    /* comprobar si puede cruzar el puente */
	pthread_mutex_lock(&mtx);
    while(MolaRamsEnPuente > 0 || IndianosEnPuente == 4 || IndianosEnPuente == 0) {
        printf("molRam %d se queda a la espera\n", index);
         pthread_cond_wait(&indianospuente, &mtx);
    }
    /* Otras acciones (COMPLETAR) */
    MolaRamsEnPuente++;
    printf("molRam %d se entra en el puente\n", index);
    sleep(3);
    /* Acciones de salida del puente (COMPLETAR) */
    MolaRamsEnPuente--;
  MolaRams--;
    printf("molRam %d sale del puente\n", index);
    if (MolaRamsEnPuente + IndianosEnPuente == 0)
        pthread_cond_broadcast(&esperando);
		
         pthread_mutex_unlock(&mtx);
	
}

void* thread_indiano(void* arg){
    printf("creando Indiano numero %d\n", IdIndiano);
     IdIndiano++;
	indiano(IdIndiano-1);
   
}
void* thread_molaRams(void* arg){
    printf("creabdo molaRam numero %d\n", IdMolaRam);
    IdMolaRam++;
	molaRams(IdMolaRam-1);
     
}

void* thread_log(void* argv) {
//<< abrir fichero log.bin. Abrir para escritura, truncando si ya existe >>
  FILE *f1;
  char buffer[] = "Timestamp		Indianos		molaRams\n";
  f1 = fopen ("indianos.log.bin", "wb");
  if(f1 == NULL)
  printf("ERROR AL ABRIR ARCHIVO");
  else 
  printf("  NO ERROR AL ABRIR ARCHIVO");
  fwrite(buffer, 1, sizeof(buffer), f1);
	while(Indianos > 0){
//		<< escribir ts (en binario) >>
  /* char buffer2[] = "%t", (intmax_t)time(NULL);
    fwrite(buffer2, 1, sizeof(buffer2), f1);*/
//		<< escribir indianosEnPuente >>
     char buffer3[] = "         %d", Indianos;
    fwrite(buffer3, 1, sizeof(buffer3), f1);
//		<< escribir molaRamsEnPuente >>
      char buffer4[] = "		%d\n", MolaRams;
    fwrite(buffer4, 1, sizeof(buffer4), f1);
		sleep(1);
	}
//	<< Cerrar fichero >>
    fclose(f1);
	return NULL;
}

int main(int argc, char* argv[]) {
    // Definicion de variables locales a main
	int i;
    // inicializamos todas las condiciones y el mutex
  pthread_mutex_init(&mtx, NULL);
  pthread_cond_init(&esperando, NULL);
  pthread_cond_init(&indianospuente, NULL);
    // creamos un array de usuarios
  pthread_t indianos[N_INDIANOS];   //vector de hilos de indianos
  pthread_t molaRams[N_MOLA_RAMS];  //vector de hilos de molaRams
  pthread_t log;
  
  pthread_create(&log, NULL, thread_log, NULL);
  
    for (i = 0; i < N_INDIANOS; i++) {
        // Crear thread para el indiano 
        pthread_create(&indianos[i], NULL, thread_indiano, NULL);
    }
    for (i = 0; i < N_MOLA_RAMS; i++) {
        // Crear thread para el molaRam i
        pthread_create(&molaRams[i], NULL, thread_molaRams, NULL);
    }
    for (i = 0; i < N_INDIANOS; i++) {
        // Crear thread para el indiano i
        pthread_join(indianos[i], NULL);
      	
    }
     pthread_join(log, NULL);
    //limpieza
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&esperando);
    pthread_cond_destroy(&indianospuente);
    //destruir molarams sobrantes si los hubiese

    return 0;
}