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

// método para el movimiento de la tortuga
void movimiento_tortuga(int * tortuga){
    /*int tubo_tortuga[2];
    pipe(tubo_tortuga);*/
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

    /*close(tubo_tortuga[0]);
    write(tubo_tortuga[1], &tortuga, sizeof(int));
    close(tubo_tortuga[1]);*/
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

int main(){

    pid_t pid_tortuga;
    pid_t pid_liebre;
    
    int tortuga = 1;
    int liebre = 1;

    int tiempo = 0;
    srand(time(0));

    int tubo_tortuga[2];
    int tubo_liebre[2];

    pipe(tubo_tortuga);
    pipe(tubo_liebre);

    // inicio de la carrera
    printf("BANG!!!!!\n");
    printf("Y ARRANCAN!!!!!\n");

    while(tortuga != META && liebre != META){
        //printf("Tortuga: %d\n", tortuga);
        //printf("Liebre: %d\n", liebre);
        pid_tortuga = fork();
        if (pid_tortuga == -1)
            //printf("No se pudo crear el proceso hijo tortuga\n");
            //exit(-1);
            ;
         else if (pid_tortuga == 0){
            close(tubo_tortuga[0]);
            movimiento_tortuga(&tubo_tortuga[1]);
            write(tubo_tortuga[1], &tubo_tortuga[1], sizeof(int));
            //escriba;
        } else {
            pid_liebre = fork();
            if (pid_liebre == -1)
                //printf("No se pudo crear el proceso hijo liebre\n");
                //exit(-1);
                ;
             else if (pid_liebre == 0){
                close(tubo_liebre[0]);
                movimiento_liebre(&tubo_liebre[1]);
                write(tubo_liebre[1], &tubo_liebre[1], sizeof(int));
                //escriba;
            } else {
                close(tubo_tortuga[1]);
                close(tubo_liebre[1]);
                read(tubo_tortuga[0], tubo_tortuga, sizeof(int));
                read(tubo_liebre[0], tubo_liebre, sizeof(int));
                imprimir(tubo_tortuga, tubo_liebre);
                //leer;
                //imprimir(&tortuga, &liebre);
                //tiempo++;
            }    
        }
        //imprimir(tubo_tortuga, tubo_liebre);
        tiempo++;
    }

    if (tortuga > liebre){
        printf("GANO LA TORTUGA!!!!! VIVA!!!!!");
    } else if (tortuga < liebre){
        printf("GANO LA LIEBRE!!!!! YUPI!!!!!");
    }

    return 0;
}



