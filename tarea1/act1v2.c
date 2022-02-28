#include <stdio.h>
#include <stdlib.h>

#define Num_init 2

// definimos nuestras estructuras

typedef struct {
    char *nombre;
    char *apellidos;
    int edad;
    char *rol; // arponero, cocinero, vigía, capitán 

} tripulantes;

typedef struct {
    int id;
    char *nombreb;
    int eslora; // eslora = longitud
    int manga; // manga = anchura
    int nummax;
    int tripActuales;
    char *propietario;
    tripulantes *tripulacion;

} barco;

typedef struct {
    int tam;
    int registro;
    barco *barcos;

} puerto;

typedef void (*opciones) (puerto*);

// declaracion de metodos para llamarlos 
void incorporar_b(puerto*);
void incorporar_tp(puerto*); 
void mostrar_barcos(puerto*);
void mostrar_tripulantes(barco);


// incorporar barcos
void incorporar_b (puerto *port){

    barco b;

    // registramos al barco con su respectiva información
    printf("Introduce el nombre del barco:\n");
    b.nombreb = (char*)malloc(30*sizeof(char));
    scanf(" %[^\n]", b.nombreb);

    printf("Introduce el id del barco:\n");
    scanf("%d", &b.id);

    printf("Introduce el tamaño de la eslora:\n");
    scanf("%d", &b.eslora);

    printf("Introduce el tamaño de la manga:\n");
    scanf("%d",&b.manga);

    printf("Introduce el numero máximo de tripulantes:\n");
    scanf("%d",&b.nummax);

    b.tripActuales = 0; // inicializamos a la tripulación actual en 0
    b.tripulacion = (tripulantes*)malloc(b.nummax*sizeof(tripulantes));

    printf("Introduce el nombre del propietario:\n");
    b.propietario = (char*)malloc((sizeof(char)*30));
    scanf(" %[^\n]", b.propietario);

    
    if(port->registro < port->tam){
        *(port->barcos + port->registro) = b; //registra el barco como nueva estructura/objeto barco
		port->registro++;
   
    } else {
		port->barcos = (barco*)realloc(port->barcos,port->tam*2*sizeof(barco)); // agregar 1 espacio para otro barco, si la memoria reservada ya está llena
		port->tam = port->tam*2;
		*(port->barcos + port->registro) = b;
		port->registro++;
	}

}

//incorporar tripulantes
void incorporar_tp(puerto *port){
    int id_b;
    printf("Por favor escriba el codigo del barco al que le quiera agregar tripulantes: \n");
    scanf("%d", &id_b);

    tripulantes trip;

    // registramos al tripulante con su respectiva información
    printf("Introduce el nombre del tripulante:\n");
    trip.nombre = (char*)malloc((sizeof(char)*30));
    scanf(" %[^\n]", trip.nombre);

    printf("Introduce el apellido del tripulante:\n");
    trip.apellidos = (char*)malloc((sizeof(char)*30));
    scanf(" %[^\n]", trip.apellidos);

    printf("Introduce la edad del tripulante:\n");
    scanf("%d", &trip.edad);

    printf("Introduce el rol del tripulante:\n");
    trip.rol = (char*)malloc((sizeof(char)*30));
    scanf(" %[^\n]", trip.rol);

    barco *aux = port->barcos;

    while (aux < port->barcos + port->registro && aux->id != id_b){
		++aux;
	}

	if (aux == port->barcos + port->registro){
		printf("ERROR! El barco con ese ID no existe \n");
	} else {
		if (aux->tripActuales < aux->nummax){
			*(aux->tripulacion + aux->tripActuales) = trip; // agrega a la persona como una struct de tripulante
			aux->tripActuales++;
		} else {
			printf("ERROR! No hay plazas disponibles en el barco \n");
		}
	}
}

// mostrar información de barcos y tripulantes
void mostrar_tripulantes(barco b){
    printf("-----------\n");
    printf("Barco %s\n", b.nombreb);
    printf("Eslora: %d\n", b.eslora);
    printf("Manga: %d\n",b.manga);
    printf("Propietario: %s\n", b.propietario);
    printf("Plazas disponibles: %d\n", b.nummax - b.tripActuales);
    printf("Tripulacion:\n");
    tripulantes *aux;
    for(aux = b.tripulacion; aux < b.tripulacion + b.tripActuales; aux++){
        printf("Nombre: %s, Apellidos: %s, Edad: %d, Rol: %s\n", aux->nombre, aux->apellidos, aux->edad, aux->rol);
    }
    printf("-----------\n");
}

// mostrar la lista de todos los barcos en el puerto con sus tripulantes 
void mostrar_barcos(puerto *port){
    barco * aux = port->barcos;
    for(; aux < port->barcos + port->registro; aux++){
        mostrar_tripulantes(*aux);
    }
}

// liberar la memoria
void limpiar_memoria(puerto *port){
    barco * aux = port->barcos;
    while(aux < port->barcos + port->registro){
        tripulantes * auxTrip = aux->tripulacion;
        while(auxTrip < aux->tripulacion + aux->tripActuales){
            free(auxTrip->nombre);
            free(auxTrip->apellidos);
            free(auxTrip->rol);
            auxTrip++;
        }
        free(aux->nombreb);
        free(aux->tripulacion);
        free(aux->propietario);
        aux++;
    }
    free(port->barcos);
    free(port);
}

int main(){

    opciones *menu = (opciones*)malloc(4 * sizeof(opciones));

    puerto *port = (puerto*)malloc(sizeof(puerto));
    port->barcos = (barco*)malloc(Num_init * sizeof(barco));
    port->registro = 0;
    port->tam = Num_init;

    *menu = incorporar_b;
    *(menu+1) = incorporar_tp;
    *(menu+2) = mostrar_barcos;

    int op=1;
    while(op)
    {
        printf("-------- Menú principal --------\n");
        printf("1.- Incorporar barcos\n");
        printf("2.- Incorporar tripulantes\n");
        printf("3.- Información de barcos y sus tripulantes\n");
        printf("Presiona 0 para salir\n");
        scanf("%i",&op);

         if(op>0 && op<4){
            (*(menu+op-1))(port); // apuntador de funciones
        }
        else if(op!=0){
            continue;
        } else {
            limpiar_memoria(port);
            break;
        }
    }  

}

