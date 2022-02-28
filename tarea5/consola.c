#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>

#define TCP_PORT 8000

// ip -> 192.168.0.7

// declaracion de métodos
void reverse (char s[]);
void itoa (unsigned long long int n, char s[]);

void reverse(char s[])
{
    int i, j;
    char c;
    
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(unsigned long long int n, char s[])
{
    int i, sign;
    
    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int main(int argc, const char * argv[]){
    
    //verificación de argumentos
    if (argc < 2) {
        printf("Por favor ingresa el IP del servidor y el ID del sensor \n");
        exit(-1);
    }

    int sh3;
    key_t k3 = 2728;
    int *conexion;
    int cliente;
    struct sockaddr_in direccion;
    ssize_t leidos, escritos;
    char buffer[1000];
    srand(time(NULL));

    // variables para memoria compartida
    sh3 = shmget(k3, sizeof(conexion), 0644|IPC_CREAT);
    conexion = shmat(sh3, NULL, 0);

    // creamos el socket
    cliente = socket(PF_INET, SOCK_STREAM, 0);    
    
    // enlace con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    escritos = connect(cliente, (struct sockaddr *) &direccion, sizeof(direccion));
    
    if (escritos == 0) {
        printf("Conectado a %s:%d \n",inet_ntoa(direccion.sin_addr),ntohs(direccion.sin_port));

        // escribir datos en el socket
        while((leidos = cliente, &buffer, sizeof(buffer)) && (*conexion > 0)){
            int temperatura = rand()%100;
            itoa(temperatura, buffer);
            write(cliente, &buffer, leidos);
            printf("Temperatura = %s\n", buffer);
            sleep(3);
        }
    }
    // cerrar sockets
    close(cliente);
    return 0;
}