#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define TCP_PORT 8000

// ip -> 192.168.0.7

// variables globales
int servidor, cliente;
char buffer[1000];

// declaracion de metodos
void rojo(int);
void intermitente(int);

// definimos nuestro método en rojo
void rojo(int signal){
    sprintf(buffer, "%s", "R");
    write(cliente, &buffer, sizeof(buffer));
}

// definimos nuestro método en intermitente
void intermitente(int signal){
    sprintf(buffer, "%s", "I");
    write(cliente, &buffer, sizeof(buffer));
}

int main(int argc, const char * argv[]){
    
    // verificacion de argumentos
    if (argc != 2){
        printf("Por favor ingresa el IP del servidor \n");
        exit(-1);
    }

    struct sockaddr_in direccion;
    ssize_t leidos, escritos;
    pid_t pid;

    // creamos el socket
    servidor = socket(PF_INET, SOCK_STREAM, 0);

    // enlace con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    bind(servidor, (struct sockaddr *) &direccion, sizeof(direccion));
    listen(servidor, 4);
    escritos = sizeof(direccion);

    int siguiente = 1;

    printf("Esperando conexiones.....\n");

    // aceptar conexiones
    while(siguiente){
        cliente = accept(servidor, (struct sockaddr *) &direccion, &escritos);
        printf("Conexiones en %s:%d \n",inet_ntoa(direccion.sin_addr),ntohs(direccion.sin_port));

        signal(SIGINT, intermitente); // declaramos intermitente como control-c
        signal(SIGTSTP, rojo); // declaramos rojo como control-z

        pid = fork();
        
        if(pid == 0){
            siguiente = 0;
        }
    }

    if(pid == 0) {
        close(servidor);

        if(cliente >= 0){
            while(leidos = read(cliente, &buffer, sizeof(buffer))){
                printf("El semáforo %s \n", buffer);
            }
        }
        close(cliente);
        exit(0);
    } else {
        // cerrar sockets
        while(wait(NULL) != -1){
            close(servidor);
        }
    }
    return 0;
}
