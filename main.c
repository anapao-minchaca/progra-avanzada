#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// definimos estructura de urna
typedef struct {
    int minimo;
    int maximo;
    int total_archivos;
} urnas;

// variable global 
 int num_urnas = 0;

// declaracion de métodos
void archivos(char *, urnas *);
void imprimir(urnas *);

// metodo para abrir y leer el directorio y obtener su informacion
void archivos(char *dir, urnas *urn){
      DIR * directorio;
      struct dirent * dr;
      struct stat info;
      char * nombre = (char*)malloc(550*sizeof(char));

      directorio = opendir(dir);
      
      // en caso de que no encuentre el directorio manda error y cierra el programa
      if(directorio == NULL){
        printf("ERROR. No se pudo abrir el directorio\n");
        closedir(directorio);
      }

      // conteo de archivos en directorio
      while ((dr=readdir(directorio))) {
          sprintf(nombre, "%s/%s", dir, dr->d_name);
          stat(nombre, &info);
          if(S_ISDIR(info.st_mode)){
              if(strcmp(dr->d_name, ".") != 0 && strcmp(dr->d_name, "..") != 0){
                  archivos(nombre, urn);
                } 
            } else if(strcmp(dr->d_name, ".") != 0 && strcmp(dr->d_name, "..") != 0){
                urnas * aux = urn;
                urnas * final = urn + num_urnas;

                for(; aux<final; aux++){
                    if((info.st_size<aux->maximo && info.st_size>=aux->minimo)){
                        aux->total_archivos = aux->total_archivos + 1;
                    }
                }
            }
      }

    //se cierra el directorio
    closedir(directorio);
    //liberamos nombre
    free(nombre);
}

// metodo para impresion del histograma
void imprimir(urnas *urn){
    urnas * aux = urn;
    urnas * final = urn + num_urnas;
    int estrellitas = 0;

    printf("Urna            Número de Archivos          Histograma\n");
    for(; aux < final; ++aux){

        printf("%d-%d              %d                    ", aux->minimo, aux->maximo, aux->total_archivos);

        // redondeamos para que sea dividido entre 10
        estrellitas = aux->total_archivos/10;
        
        for(int contador = 0; contador<estrellitas; contador++){
            printf("*");
        }
        printf("\n");
    }
}

int main(){
    int rango_urnas = 0;

    printf("Ingrese la cantidad de urnas: ");
    scanf("%d", &num_urnas);

    urnas *urn = (urnas*)malloc(num_urnas*sizeof(urnas));

    printf("Ingrese la anchura de urna (bytes): ");
    scanf("%d", &rango_urnas);

    // asignación de rangos
    urnas * aux = urn;
    urnas * final = urn + num_urnas;
    int i = 0;
     for(; aux < final; ++aux){
        aux->minimo = i*rango_urnas;
        aux->maximo = (i*rango_urnas) + rango_urnas-1;
        i++;
     }

     printf("Ingresa el directorio a recorrer: \n");
     char * dir = (char*)malloc(150*sizeof(char));
     scanf(" %[^\n]", dir);
     
     // llamada a los metodos
     archivos(dir, urn);
     imprimir(urn);
     
     // liberamos la memoria del directorio y urnas
     free(dir);
     free(urn);
     
    // dir de prueba: /Users/Hayoung/Desktop/tec/progra
}


