#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// declaramos el final de la carrera
#define META 70

// declaracion de métodos
void movimiento_tortuga(int * tortuga);
void movimiento_liebre(int * liebre);
void imprimir(const int* const tortuga, const int* const liebre);

int main(){

    int tortuga = 1;
    int liebre = 1;
    
    pid_t pid_tortuga;
    pid_t pid_liebre;
    
    int tiempo = 0;
    srand(time(0));

    // inicio de la carrera
    printf("BANG!!!!!\n");
    printf("Y ARRANCAN!!!!!\n");

    pid_tortuga = fork();
    pid_liebre = fork();

    while(tortuga != META && liebre != META){
        
        if (pid_tortuga == 0 && pid_liebre == 0){
            movimiento_liebre(&liebre);
            movimiento_tortuga(&tortuga);
            imprimir(&tortuga, &liebre);
            tiempo++;
        }

    }

    if (tortuga > liebre){
        printf("GANO LA TORTUGA!!!!! VIVA!!!!!");
    }  else if (tortuga < liebre){
        printf("GANO LA LIEBRE!!!!! YUPI!!!!!");
    } 

    return 0;
    /*imprimir(&tortuga, &liebre);
    tiempo++;*/
}

// método para el movimiento de la tortuga
void movimiento_tortuga(int * tortuga){
    int pos = rand() % 10;

    if(pos >= 1 && pos <= 5){ // paso rapido
        *tortuga += 3;
    } else if(pos >= 6 && pos <= 7){ // deslizamiento
        *tortuga -= 6;
    } else { // paso lento
        *tortuga += 1;
    }
    
    if(*tortuga < 1){
       *tortuga = 1;
    } else if(*tortuga > META){
       *tortuga = META;
    }
   //printf("Se esta moviendo la tortuga");
}

// método para el movimiento de la liebre
void movimiento_liebre(int * liebre){
    int pos = rand() % 10;

    if(pos >= 1 && pos <= 2){ // dormir
        *liebre = *liebre;
    } else if(pos >= 3 && pos <= 4){ // salto grande
        *liebre += 9;
    } else if(pos == 5) { // deslizamiento grande
        *liebre -= 12;
    } else if(pos >= 6 && pos <= 8){ // salto pequeño
        *liebre += 1;
    } else { // deslizamiento pequeño
        *liebre -= 2;
    }
    
    if(*liebre < 1){
       *liebre = 1;
    } else if(*liebre > META){
       *liebre = META;
    }
    //printf("Se esta moviendo la liebre");
}

// método para imprimir las posiciones de los competidores
void imprimir(const int* const tortuga, const int* const liebre){
    int pos;

    if(*tortuga == *liebre){
        for(pos = 1; pos < *tortuga; pos++){
            printf("%s", " ");
        }
        printf("OUCH!!!!");
    } else if(*tortuga < *liebre){
        for(pos = 1; pos < *tortuga; pos++){
            printf("%s", " ");
        }
        printf("T");

        for(pos = 1; pos < (*liebre - *tortuga); pos++){
            printf("%s", " ");
        }
        printf("L");
    }else{
        for(pos = 1; pos < *liebre; pos++){
            printf("%s", " ");
        }
        printf("L");

        for(pos = 0; pos < (*tortuga - *liebre); pos++){
            printf("%s", " ");
        }
        printf("T");
    }
    printf("\n");
}
