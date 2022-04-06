#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#define N_PARADAS 5 // numero de paradas de la ruta
#define EN_RUTA 0 // autobus en ruta
#define EN_PARADA 1 // autobus en la parada
#define MAX_USUARIOS 40 // capacidad del autobus
#define USUARIOS 4 // numero de usuarios

// estado inicial
int estado = EN_RUTA;
int parada_actual = 0; // parada en la que se encuentra el autobus
int n_ocupantes = 0; // ocupantes que tiene el autobus
// personas que desean subir en cada parada
int esperando_parada[N_PARADAS]; //= {0,0,...0};
// personas que desean bajar en cada parada
int esperando_bajar[N_PARADAS]; //= {0,0,...0};
// Otras definiciones globales (comunicacion y sincronizacion)
int id_actual = 1;

pthread_mutex_t mutex;
pthread_cond_t cond_bajar, cond_subir, cond_arrancar; // variables condicionables

void Autobus_En_Parada(){
    /* Ajustar el estado y bloquear al autobus hasta que no haya pasajeros que
    quieran bajar y/o subir la parada actual. Despues se pone en marcha */
    
    pthread_mutex_lock(&mutex); //bloqueamos el mutex
    estado = EN_PARADA;

    // esperamos a que todos se suban y bajen
    printf("Autobús parado en la %d parada \n", parada_actual);
    while(esperando_parada[parada_actual] > 0) {
        pthread_cond_broadcast(&cond_subir);
        pthread_cond_wait(&cond_arrancar, &mutex);
    }
    while(esperando_bajar[parada_actual] > 0) {
        pthread_cond_broadcast(&cond_bajar);
        pthread_cond_wait(&cond_arrancar, &mutex);
    }

    estado = EN_RUTA;
    printf("Autobús saliendo de la %d parada \n", parada_actual);
    pthread_mutex_unlock(&mutex); //desbloqueamos el mutex
}
void Conducir_Hasta_Siguiente_Parada(){
    /* Establecer un Retardo que simule el trayecto y actualizar numero de parada */

    // conduce hasta la siguiente parada y tarda entre 1 y 10 segundos
    pthread_mutex_lock(&mutex);
    int tiempo = rand() % 10;
    printf("Autobús en ruta desde %d parada \n", parada_actual);
    sleep(tiempo);
    parada_actual = ((parada_actual + 1) % N_PARADAS);
    printf("BEEP BEEP!! Autobús llegando a la parada %d \n", parada_actual);
    pthread_mutex_unlock(&mutex);
}
void Subir_Autobus(int id_usuario, int origen){
    /* El usuario indicara que quiere subir en la parada ’origen’, esperara a que
    el autobus se pare en dicha parada y subira. El id_usuario puede utilizarse para
    proporcionar informacion de depuracion */
    pthread_mutex_lock(&mutex); // bloqueamos el mutex

    printf("Usuario %d esperando a subirse en parada %d \n", id_usuario, origen);
    esperando_parada[origen]++; // una persona más que quiere subir a la parada
    while (estado != EN_PARADA || parada_actual != origen)
        pthread_cond_wait(&cond_subir, &mutex); // va a estar esperando hasta que llegue el bus

    n_ocupantes++;
    esperando_parada[origen]--;
    printf("Usuario %d se ha subido en la parada %d \n", id_usuario, origen);

    if(esperando_parada[origen] == 0)
        pthread_cond_signal(&cond_arrancar); // si no hay nadie esperando parada (ya han subido todos) entonces arranca

    pthread_mutex_unlock(&mutex); // desbloqueamos el mutex
}
void Bajar_Autobus(int id_usuario, int destino){
    /* El usuario indicara que quiere bajar en la parada ’destino’, esperara a que
    el autobus se pare en dicha parada y bajara. El id_usuario puede utilizarse para
    proporcionar informacion de depuracion */
    pthread_mutex_lock(&mutex); // bloqueamos el mutex

    printf("Usuario %d esperando a bajarse en la parada %d \n", id_usuario, destino);
    esperando_bajar[destino]++; // una persona más que quiere bajar del autobús
    while (estado != EN_PARADA || parada_actual != destino)
        pthread_cond_wait(&cond_bajar, &mutex); // va a estar esperando hasta que llegue el bus al destino

    n_ocupantes--;
    esperando_parada[destino]--;
    printf("Usuario %d se acaba de bajar en la parada %d \n", id_usuario, destino);

    if(esperando_parada[destino] == 0)
        pthread_cond_signal(&cond_arrancar); // si nadie más se quiere bajar entonces arranca

    pthread_mutex_unlock(&mutex); // desbloqueamos el mutex
}

void * thread_autobus(void * args) {
    printf("Creando Autobús \n");
    while (1) {
        // esperar a que los viajeros suban y bajen
        Autobus_En_Parada();
        // conducir hasta siguiente parada
        Conducir_Hasta_Siguiente_Parada();
    }
}

void Usuario(int id_usuario, int origen, int destino) {
    // Esperar a que el autobus este en parada origen para subir
    Subir_Autobus(id_usuario, origen);
    // Bajarme en estacion destino
    Bajar_Autobus(id_usuario, destino);
}
void * thread_usuario(void * arg) {
    int id_usuario = id_actual, a, b;
    id_actual++;
    printf("Creando usuario %d \n", id_usuario); //atoi en caso de que usemos arg (?)
    // obtener el id del usario
    while (1) {
        a = rand() % N_PARADAS; // parada origen
        do{
            b = rand() % N_PARADAS; // parada destino
        }while(a == b);
        Usuario(id_usuario,a,b);
    }
}


int main(int argc, char *argv[]) {
    // Definicion de variables locales a main
    int i, j;

    // Opcional: obtener de los argumentos del programa la capacidad del
    // autobus, el numero de usuarios y el numero de paradas

    // Crear el thread Autobus
    pthread_t bus;

    // inicializamos todas las condiciones y el mutex
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_arrancar, NULL);
    pthread_cond_init(&cond_bajar, NULL);
    pthread_cond_init(&cond_subir, NULL);

    // creamos un array de usuarios
    pthread_t usuarios[USUARIOS];
    // creamos el autobús
    pthread_create(&bus, NULL, thread_autobus, NULL);

    for (i = 0; i < USUARIOS; i++){
        // Crear thread para el usuario i
        pthread_create(&usuarios[i], NULL, thread_usuario, NULL);
    }
    for (j = 0; j < USUARIOS; j++) {
        // Esperar terminacion de los hilos
        pthread_join(usuarios[j], NULL);
    }
    // Esperamos la terminación de los hilos
    pthread_join(bus, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_arrancar);
    pthread_cond_destroy(&cond_bajar);
    pthread_cond_destroy(&cond_subir);

    return 0;
}