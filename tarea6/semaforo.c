#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define TCP_PORT 8000

// ip -> 192.168.0.7

// variables globales
int cliente, nextpid;
char buffer[1000];
int block = 0;
char color;

// declaracion de metodos
void control_semaforo(int);
void cambio_semaforo(int);

// metodo para que los semáforos empiecen en verde y duren 30 segundos
void control_semaforo(int signal){
    color = 'V';
    sprintf(buffer, "%d %s", getpid(), "está en Verde");
    write(cliente, &buffer, sizeof(buffer));
    alarm(30);
}

// metodo para que los semáforos cambien a rojo y que el siguiente cambie a verde 
void cambio_semaforo(int signal){
    if(block == 0){
        color = 'R';
        sprintf(buffer, "%d %s", getpid(), "está en Rojo");
        write(cliente, &buffer, sizeof(buffer));
        kill(nextpid, SIGUSR1);
    }
}

int main(int argc, const char * argv[]){
    
    // verificacion de argumentos
    if (argc != 2){
        printf("Por favor ingresa el IP del servidor \n");
        exit(-1);
    }

    struct sockaddr_in direccion;
    ssize_t leidos, escritos;
    char color_ant;
    int conexion;

    // creamos el socket
    cliente = socket(PF_INET, SOCK_STREAM, 0);

    // enlace con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
 
    conexion = connect(cliente, (struct sockaddr *) &direccion, sizeof(direccion));

    signal(SIGUSR1, control_semaforo); // declaramos control_semaforo como SIGUSR1
    signal(SIGALRM, cambio_semaforo); // declaramos cambio_semaforo con SIGALRM cuando el timer expira

    // con esto sabemos que PID tiene cada semáforo
    printf("Soy el PID con el ID: %d\n", getpid());

    // ingresamos el color inicial de los semáforos, se debe usar V o R
    printf("Ingresa el color inicial: \n");
    scanf("%c", &color);
    
    // aquí ingresamos los PID de los semáforos para que sepa el proceso
    printf("Ingresa el PID del siguiente semáforo: \n");
    scanf("%d", &nextpid);

    // si esta en verde se inicia el proceso
    if(color == 'V'){
        sprintf(buffer, "%d %s", getpid(), "está en Verde");
        write(cliente, &buffer, sizeof(buffer));
        alarm(30);
    }

    if(conexion == 0){

        printf("Conectado a %s:%d \n", inet_ntoa(direccion.sin_addr), ntohs(direccion.sin_port));

        while(leidos = read(cliente, &buffer, sizeof(buffer))){
            printf("Recibimos señal: %s \n", buffer);

            // si recibimos rojo o intermitente escribe el nuevo valor y se detiene la alarma
            if(buffer[0] == 'R' || buffer[0] == 'I'){
                if(block == 0){
                    block = 1;
                    color_ant = color;
                    sprintf(buffer, "%d %s %c", getpid(), "recibió señal", buffer[0]);
                    write(cliente, &buffer, sizeof(buffer));
                    alarm(0);
                } else {
                    block = 0;
                    color = color_ant;
                    if(color == 'V'){
                        sprintf(buffer, "%d %s", getpid(), "está en Verde");
                        write(cliente, &buffer, sizeof(buffer));
                        alarm(30);
                    }
                }
            }
        } 
    }
    // cerrar sockets
    close(cliente);
    return 0;
}