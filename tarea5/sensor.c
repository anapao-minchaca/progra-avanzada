#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>

#define TCP_PORT 8000

// declaracion de métodos
void reporte(int *arreglo, int kant, int datos, int *kantDatos, int dias);

// método para sacar el reporte por sensor
void reporte(int *arreglo, int kant, int datos, int *kantDatos, int dias){
    int * minimo;
    int * maximo;
    double promedio = 0;

    for(int i = 0; i < kant; i++){
        minimo = (arreglo+((datos * i)));
        maximo = (arreglo+((datos * i)));
        promedio = 0;

        for(int j = 0; j < dias; j++){
            for(int k = 0; k < *(kantDatos+(i)); k++){
                if(*(arreglo+(((datos * i) * (dias * 0))+k)) < *minimo){
                    minimo = (arreglo+((datos * i)+k));
                    printf("Min: %d", *minimo);
                }
                if(*maximo < *(arreglo+(((datos * i) * (dias * 0))+k))){
                    maximo = (arreglo+((datos * i)+k));
                    printf("Max: %d", *maximo);
                }
                promedio += *(arreglo+(((datos * i) * (dias * 0))+k));
            }
        }
            printf(" ---- Sensor # %d ---- \n", i);
            printf("Minimo: %d°C\n",*minimo);
            printf("Maximo: %d°C\n",*maximo);
            printf("Promedio: %f°C\n", promedio/ (*(kantDatos+(i))));

    }
}

int main(int argc, const char * argv[]){
    int kantSensores;
    int dias = 1; 
    int kant_datos = 100;
    key_t k1 = 9999;
    key_t k2 = 1812;
    key_t k3 = 2728;
    int *conexion;
    int sh1, sh2, sh3;
    int desconectar = 1;
    int servidor, cliente;
    struct sockaddr_in direccion;
    ssize_t leidos, escritos;
    int siguiente = 1;
    int count = 0;
    pid_t pid;
    char buffer[1000];

    printf("Ingrese el número de sensores a conectar: ");
    scanf("%d", &kantSensores);
    printf("Esperando conexiones.....\n");

    int *arreglo = (int*)malloc(kantSensores * dias * kant_datos * sizeof(int));
    int *total = (int*)malloc(kantSensores * sizeof(int));

    sh1 = shmget(k1, sizeof(total), 0644|IPC_CREAT);
    sh2 = shmget(k2, sizeof(arreglo), 0644|IPC_CREAT);
    sh3 = shmget(k3, sizeof(conexion), 0644|IPC_CREAT);
    total = shmat(sh1, NULL, 0);
    arreglo = shmat(sh2, NULL, 0);
    conexion = shmat(sh3, NULL, 0);

    memcpy(conexion, &desconectar, sizeof(desconectar));

    if (argc != 2){
        printf("Por favor ingresa el IP del servidor \n");
        exit(-1);
    }

    servidor = socket(PF_INET, SOCK_STREAM, 0);

    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    bind(servidor, (struct sockaddr *) &direccion, sizeof(direccion));
    listen(servidor, 10);
    escritos = sizeof(direccion);

    while(siguiente){
        cliente = accept(servidor, (struct sockaddr *) &direccion, &escritos);
        printf("Conexiones en %s:%d \n",inet_ntoa(direccion.sin_addr),ntohs(direccion.sin_port));
        count++;

        pid = fork();

        if (pid == 0){
            siguiente = 0;
        }
        else if(count > kantSensores-1){
            while(1){
                int opcion;
                printf("-------- Menú principal --------\n");
                printf("1.- Generar reporte de sensores\n");
                printf("2.- Salir del programa\n");
                scanf("%d", &opcion);

                switch(opcion){
                    case 1:
                        reporte(arreglo, kantSensores, kant_datos, total, dias);
                        break;
                    case 2:
                        desconectar = 0;
                        memcpy(conexion, &desconectar, sizeof(desconectar));
                        sleep(3);
                        exit(0);
                    default:
                        break;
                }
            }
        }
    }

    if (pid == 0){
        close(servidor);
        int v = 0;
        int contador = 0;
        char * temporal;
        int num_tempo;

        if(cliente >= 0){
            while(leidos = read(cliente, &buffer, sizeof(buffer))){
                v = (((kant_datos * (count-1))) * (dias * 0)) + contador;
                printf("El número es: %s \n", buffer);
                temporal = buffer;
                num_tempo = atoi(temporal);
                
                if(contador < kant_datos){
                    memcpy(((arreglo+(v))), &num_tempo, sizeof(num_tempo));
                } else {
                    printf("Se ha llenado el almacenamiento del sensor\n");
                }
        
                contador++;
                memcpy(((total+(contador-1))), &contador, sizeof(contador));
            }
        }
        close(cliente);
        exit(0);
    } else {
        while(wait(NULL) != -1){
            shmdt(arreglo);
            shmdt(total);
            free(arreglo);
            free(total);
            close(servidor);
        }
    }
    return 0;
}