#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define SECCIONES 4
#define ROBOT 6
#define MAX_SECCION 40

// variables globales
int * peso_seccion;
int * peso_actual;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t centro_comercial [SECCIONES];

// declaracion de metodos
void * robots(void *);

// metodo para el proceso del robot
void * robots(void * robotcito){
    int peso_producto = 0;
    int peso_check = 0;
    int * compras;
    compras = (int *) malloc (sizeof(int) * SECCIONES);
    int id_robot = (int) robotcito;
    int peso_robot = rand()%20+1;
    int robot_producto = 0;
    
    // llenamos la lista de compras de forma aleatoria
    for(int i = 0; i<SECCIONES; i++){
        peso_producto = rand() % 6;
        if((peso_check + peso_producto) < MAX_SECCION){
            *(compras + i) = peso_producto;
            peso_check += peso_producto;
        } else{
            *(compras + i) = 0;
        }
    }

    printf("+++ (Robot %d) con peso %d\n", id_robot, peso_robot);
    
    // robot recorre las secciones necesarias y va haciendo compras
    for(int j = 0; j < SECCIONES; j++){
        robot_producto = 0;
        while(robot_producto == 0){
            pthread_mutex_lock(&mutex);
            // condicion para revisar si el robot puede entrar a seccion y comprar algo
            if((*(peso_actual + j) + peso_robot + *(compras + j)) < *(peso_seccion + j) && *(compras + j) > 0){
                if(j > 0){
                    printf("--- (Robot %d) salió de la sección %d\n", id_robot, j-1);
                    // restamos el peso
                    *(peso_actual + j -1) -= peso_robot;
                    pthread_cond_broadcast(&centro_comercial[j-1]);
                }
                // sumamos los pesos respectivamente
                *(peso_actual + j) += *(compras + j);
                *(peso_actual + j) += peso_robot;
                peso_robot += *(compras + j);
                printf("+++ (Robot %d) (peso = %d) ingresó a la sección %d y compró peso %d (peso actual de sección = %d)\n", id_robot, peso_robot, j, *(compras + j), *(peso_actual+j));
                robot_producto = 1;
                
                if(j == SECCIONES - 1){
                    printf("--- (Robot %d) salió de la sección %d\n", id_robot, j);
                    *(peso_actual + j) -= peso_robot;
                    pthread_cond_broadcast(&centro_comercial[j]);
                }
            } else if((*(peso_actual + j) + peso_robot) < *(peso_seccion + j) && *(compras + j) == 0){
                if(j > 0){
                    printf("--- (Robot %d) salió de la sección %d\n", id_robot, j-1);
                    *(peso_actual + j -1) -= peso_robot;
                    pthread_cond_broadcast(&centro_comercial[j-1]);
                }
                *(peso_actual + j) += peso_robot;
                robot_producto = 1; 
                printf("+++ (Robot %d) (peso = %d) ingresó a la sección %d para tránsito (peso actual de sección = %d)\n", id_robot, peso_robot, j, *(peso_actual+j));
                if (j == SECCIONES - 1){
                    printf("--- (Robot %d) salió de la sección %d\n", id_robot, j);
                    *(peso_actual + j) -= peso_robot;
                    pthread_cond_broadcast(&centro_comercial[j]);
                }
            } else {
                printf("--- (Robot %d) está esperando para ingresar a la sección %d (peso actual de sección = %d) ---\n", id_robot, j, *(peso_actual+j));
                pthread_cond_wait(&centro_comercial[j], &mutex);
            }
            pthread_mutex_unlock(&mutex);
        }
        sleep(rand()%10);
    }
    pthread_exit(NULL);
}

int main(int argc, const char *argv[]){
    srand((int) time(NULL));

    peso_seccion = (int *) malloc (sizeof(int) * SECCIONES);
    int * aux_peso_seccion = peso_seccion;
    int * fin_peso_seccion = peso_seccion + SECCIONES;

    peso_actual = (int *) malloc (sizeof(int) * SECCIONES);
    int * aux_peso_actual = peso_actual;
    int * fin_peso_actual = peso_actual + SECCIONES;

    pthread_t * robotcitos = (pthread_t *) malloc (sizeof(pthread_t) * ROBOT);
    pthread_t * aux_robot = robotcitos;
    pthread_t * fin_robot = robotcitos + ROBOT;

    // variables de condicion
    for(int i = 0; i < SECCIONES; i++){
        pthread_cond_init(&centro_comercial[i], NULL);
    }

    int id_robot = 0;
    int n = 0;

    // le asignamos peso a las secciones
    for(; aux_peso_seccion < fin_peso_seccion; aux_peso_seccion++){
        * aux_peso_seccion = rand() % 70 + MAX_SECCION;
        printf("--- El peso máximo por la sección %d: %d\n", n, *aux_peso_seccion);
        n++;
    }
    
    printf("\n");

    // inicializamos el peso actual en 0
    for(; aux_peso_actual < fin_peso_actual; aux_peso_actual ++){
        *aux_peso_seccion = 0;
    }

    // creacion de threads
    for(; aux_robot < fin_robot; aux_robot++){
        pthread_create(aux_robot, NULL, robots, (void *) id_robot);
        id_robot++;
    }

    for(aux_robot = robotcitos; aux_robot < (robotcitos + ROBOT); ++aux_robot){
        pthread_join(*aux_robot, NULL);
    }

    // liberamos memoria 
    free(peso_seccion);
    free(peso_actual);
    free(robotcitos);
    
    return 0;
}