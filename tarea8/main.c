#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// cajeros 
#define CAJEROS_G 5
#define CAJEROS_E 3

// llegada de usuarios 
#define USUARIOS_G 100
#define USUARIOS_E 50

#define BUFFER_G 10
#define BUFFER_E 10

// variables globales
int g_in, g_out = 0;
int e_in, e_out = 0;
int total_g, total_e = 0;
int producidos_g, producidos_e = 0;
int consumidos_g, consumidos_e = 0;
int elem_g[BUFFER_G], elem_e[BUFFER_E];

pthread_mutex_t mutex_g = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_e = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t consume_g = PTHREAD_COND_INITIALIZER;
pthread_cond_t consume_e = PTHREAD_COND_INITIALIZER;
pthread_cond_t produce_g = PTHREAD_COND_INITIALIZER;
pthread_cond_t produce_e = PTHREAD_COND_INITIALIZER;


// declaracion de metodos
void * atm_general(void *);
void * atm_empresarial(void *);
void * cliente_general(void *);
void * cliente_empresarial(void *);

// metodo para el cajero general
void * atm_general(void * arg){
    int id = (int) arg;
    int tiempo_op, descanso = 0;

    printf("(A) Inicia el ATM general %d\n", id);

    while(consumidos_g < USUARIOS_G){
        pthread_mutex_lock(&mutex_g);

        if(total_g > 0 && consumidos_g < USUARIOS_G) {
            // consumimos elementos 
            tiempo_op = rand() % 3 + 3; // duracion de operacion entre 3 y 5 segundos
            printf("--- (ATM General %d) realiza la operación general al (Cliente General %d) con una duración de %d segundos\n", id, elem_g[g_out], tiempo_op);
            
            ++consumidos_g;
            ++g_out;
            g_out %= BUFFER_G;
            --total_g;
            descanso++;
            
            if(total_g == (BUFFER_G - 1)){
                pthread_cond_broadcast(&produce_g);
            }

            if(descanso == 5){
                descanso = 0;
                printf("<--- (ATM General %d) ha detenido sus actividades durante 3 segundos\n", id);
                usleep(3);
                printf("---> (ATM General %d) ha resumido sus actividades\n", id);
            }
        }
        else if(consumidos_g < USUARIOS_G){
            pthread_cond_wait(&consume_g, &mutex_g);
        }

        pthread_mutex_unlock(&mutex_g);
        usleep(tiempo_op);
    }
    pthread_exit(NULL);
}

// metodo para el cajero empresarial
void * atm_empresarial(void * arg){
    int id = (int) arg;
    int tiempo_op, descanso = 0;

    printf("(A) Inicia el ATM empresarial %d\n", id);

    while(consumidos_e < USUARIOS_E || consumidos_g < USUARIOS_G){
        if(total_e > 0 && consumidos_e < USUARIOS_E){
            // consumimos elementos 
            pthread_mutex_lock(&mutex_e);

            tiempo_op = rand() % 3 + 3; // duracion de operacion entre 3 y 5 segundos
            printf("--- (ATM Empresarial %d) realiza la operación empresarial al (Cliente Empresarial %d) con una duración de %d segundos\n", id, elem_e[e_out], tiempo_op);

            ++consumidos_e;
            ++e_out;
            e_out %= BUFFER_E;
            --total_e;
            descanso++;

            if(total_e == (BUFFER_E - 1)){
                pthread_cond_broadcast(&produce_e);
            }

            if(descanso == 5){
                descanso = 0;
                printf("<--- (ATM Empresarial %d) ha detenido sus actividades durante 3 segundos\n", id);
                usleep(3);
                printf("---> (ATM Empresarial %d) ha resumido sus actividades\n", id);
            }
        }
        else if(consumidos_e < USUARIOS_E){
            pthread_cond_wait(&consume_e, &mutex_e);
        }

        pthread_mutex_unlock(&mutex_e);
        usleep(tiempo_op);
    }
    pthread_exit(NULL);
}

// proceso del cliente general
void * cliente_general(void * arg){
    int id = (int) arg;
    int tiempo;

    while(producidos_g < USUARIOS_G){
        tiempo = rand() % 18 + 5; // tiempo de llegada entre 5 y 22 segundos
        pthread_mutex_lock(&mutex_g);

        if(total_g < BUFFER_G && producidos_g < USUARIOS_G){
            // producimos los elementos
            elem_g[g_in] = producidos_g;
            printf("+++ (Cliente General %d) llega a la fila y va a realizar una operación general\n", elem_g[g_in]);
            
            producidos_g++;
            ++g_in;
            g_in %= BUFFER_G;
            ++total_g;

            if(total_g == 1){
                pthread_cond_broadcast(&consume_g);
                pthread_cond_broadcast(&consume_e);
            }
        }

        else if(producidos_g < USUARIOS_G){
            printf("---------- Se han dejado de agregar clientes a la fila general ----------\n");
            pthread_cond_wait(&produce_g, &mutex_g);
            printf("---------- Se han vuelto a agregar clientes a la fila general ----------\n");
        }

        pthread_mutex_unlock(&mutex_g);
        usleep(tiempo);
    }

    pthread_exit(NULL);
}

// proceso del cliente empresarial
void * cliente_empresarial(void * arg){
    int id = (int) arg;
    int tiempo;
    
    while(producidos_e < USUARIOS_E){
        tiempo = rand() % 26 + 9; // tiempo de llegada entre 9 y 34 segundos
        
        pthread_mutex_lock(&mutex_e);

        if(total_e < BUFFER_E && producidos_e < USUARIOS_E){
            // producimos los elementos
            elem_e[e_in] = producidos_e;
            printf("+++ (Cliente Empresarial %d) llega a la fila y va a realizar una operación empresarial\n", elem_e[e_in]);
            
            producidos_e++;
            ++e_in;
            e_in %= BUFFER_E;
            ++total_e;

            if(total_e == 1){
                pthread_cond_broadcast(&consume_e);
            }
        }

        else if(producidos_e < USUARIOS_E){
            printf("---------- Se han dejado de agregar clientes a la fila empresarial ----------\n");
            pthread_cond_wait(&produce_e, &mutex_e);
            printf("---------- Se han vuelto a agregar clientes a la fila empresarial ----------\n");
        }

        pthread_mutex_unlock(&mutex_e);
        usleep(tiempo);
    }

    pthread_exit(NULL);
}

int main(int argc, const char *argv[]){
    srand((int) time(NULL));

    int index = 0;
    
    pthread_t * atm = (pthread_t *) malloc (sizeof(pthread_t) * 10);
    pthread_t * aux = atm;

    printf("EL BANCO ESTÁ ABIERTO\n");
    
    printf("(P) Creando el productor para usuarios generales %d\n", 1);
    pthread_create(aux, NULL, cliente_general, (void *) 1);
    aux++;
    printf("(P) Creando el productor para usuarios empresariales %d\n", 1);
    pthread_create(aux, NULL, cliente_empresarial, (void *) 1);
    aux++;

    for(; aux < atm + CAJEROS_G + 2; ++aux){
        printf("--- Creando el ATM General %d ---\n", ++index);
        pthread_create(aux, NULL, atm_general, (void *) index);
    }

    for(aux = atm; aux < atm + CAJEROS_E; ++aux){
        printf("--- Creando el ATM Empresarial %d ---\n", ++index);
        pthread_create(aux, NULL, atm_empresarial, (void *) index);
    }

    for(aux = atm; aux < (atm + 9); ++aux){
        pthread_join(*aux, NULL);
    }

    free(atm);
    return 0;
}