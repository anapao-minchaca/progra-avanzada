#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 4;

// definimos nuestra estructura de vector
typedef struct
{
    void **info;
    int tam;
    int total;
} vector;

typedef struct
{
    char *titulo;
    int paginas;
} Libro;

// declaración de métodos para llamarlos
void vector_init(vector *);
void vector_copias(vector *, int, void *);
static void vector_aumentar(vector *, int);
void vector_insertar(vector *, void *, int);
void insertar_copias(vector *, void *, int, int);
void push_back(vector *, void *);
void pop_back(vector *);
void ref_front(vector *);
void ref_back(vector *);
void vector_borrar(vector *, int);
void borrar_medio(vector *);
void clear(vector *);
bool empty(vector *);

// 1
void vectorV()
{
    int *intVectorV = (int *)malloc(sizeof(int));
    char *charVectorV = (char *)malloc(sizeof(char));
    Libro *libroVectorV = (Libro *)malloc(sizeof(Libro));
}

// inicializar el vector -> 2
void vector_init(vector *vectorcito)
{
    vectorcito->tam = N;
    vectorcito->total = 0;
    vectorcito->info = malloc(sizeof(void *) * vectorcito->tam);
}

//crea un vector con n copias de t -> 3
void vector_copias(vector *vectorcito_copia, int n, void *info)
{
    for (int i = 0; i < n; i++)
    {
        vectorcito_copia->info[i] = info;
        vectorcito_copia->total++;
    }
}

// función para aumentar el tamaño del vector
static void vector_aumentar(vector *vectorcito, int tam)
{
    void **info = realloc(vectorcito->info, sizeof(void *) * tam);

    if (info)
    {
        vectorcito->info = info;
        vectorcito->tam = tam;
    }
}

// insertar un valor en una posicion en específico -> 4
void vector_insertar(vector *vectorcito, void *info, int pos)
{

    for (int i = vectorcito->total - 1; i >= pos; i--)
    {
        vectorcito->info[i + 1] = vectorcito->info[i];
    }
    vectorcito->info[pos] = info;
    vectorcito->total++;
}

//inserta n copias de x en pos -> 5
void insertar_copias(vector *vectorcito, void *info, int pos, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector_insertar(vectorcito, info, pos);
    }
}

// Inserta un elemento al final -> 6
void push_back(vector *vectorcito, void *info)
{
    if (vectorcito->tam == vectorcito->total)
    {
        vector_aumentar(vectorcito, vectorcito->tam * 2);
        // si el tamaño es igual al total se hace un realloc para aumentar el tamaño del vector
    }
    vectorcito->info[vectorcito->total++] = info;
}

//elimina el ultimo elemento -> 7
void pop_back(vector *vectorcito)
{
    vectorcito->info[vectorcito->total--] = NULL;
}

// regresamos la informacion guardada en el vector
void *obtener_info(vector *vectorcito, int indice)
{
    if (indice >= 0 && indice < vectorcito->total)
    {
        return vectorcito->info[indice];
    }
    return NULL;
}

//referencia al primer elemento -> 8
void ref_front(vector *vectorcito)
{
    printf("El primer elemento del vector es: %s\n", obtener_info(vectorcito, 0));
}

//referencia al ultimo elemento -> 9
void ref_back(vector *vectorcito)
{
    printf("El último elemento del vector es: %s\n", vectorcito->info[vectorcito->total - 1]);
}

//elimina elemento que se encuentra en pos ->10
void vector_borrar(vector *vectorcito, int pos)
{
    vectorcito->info[pos] = NULL;

    for (int i = pos; i < vectorcito->total - 1; i++)
    {
        vectorcito->info[i] = vectorcito->info[i + 1];
        vectorcito->info[i + 1] = NULL;
    }

    vectorcito->total--;

    if (vectorcito->total > 0 && vectorcito->total == vectorcito->tam / 4)
    {
        vector_aumentar(vectorcito, vectorcito->tam / 2);
    }
}

//elimina los elementos que se encuentran entre first y last -> 11
void borrar_medio(vector *vectorcito)
{
    while (vectorcito->total > 2)
    {
        for (int i = 1; i < vectorcito->total - 1; i++)
        {
            vectorcito->info[i] = vectorcito->info[i + 1];
        }
        vectorcito->total--;
    }
}

//elimina todos los elementos -> 12
void clear(vector *vectorcito)
{
    for (int i = 0; i <= vectorcito->total; i++)
    {
        vectorcito->info[i] = NULL;
    }
    vectorcito->total--;
    free(vectorcito->info);
}

//regresa t o f si el vector esta vacio -> 13
bool empty(vector *vectorcito)
{
    for (int i = 0; i <= vectorcito->total; i++)
    {
        if (vectorcito->info[i] == NULL)
        {
            printf("El vector está vacío\n");
            return true;
        }
        printf("El vector no está vacio\n");
        return false;
    }
}

//regresa un iterador al primer elemento -> 14
void *begin(vector *vectorcito) 
{
    void *aux = vectorcito->info;

    return aux;
}

//regresa un iterador al ultimo elemento -> 15
void *end(vector *vectorcito, int type)
{
    return (vectorcito->info + ((vectorcito->total - 1) * type));
}

//regresa un iterador al siguiente elemento -> 16
void *next(void *arr, int type, void *final)
{
    void *aux = (arr + type);
    if (aux - type == final)
    { // Verífica si llegó al final del arreglo y regresa un NULL
        return NULL;
    }
    else
    {
        return aux;
    }
}

// función para saber el final "total" de nuestro vector
int vector_final(vector *vectorcito)
{
    return vectorcito->total;
}

int main()
{
    vector vectorcito;
    vector vectorcito_copia;
    Libro libro1;
    libro1.titulo = "Harry Potter";
    libro1.paginas = 133;

    // -> pruebas para inicializar vector <-
    vector_init(&vectorcito);
    vector_init(&vectorcito_copia);

    // -> pruebas con push_backs <-
    push_back(&vectorcito, "Ana");
    push_back(&vectorcito, "Karen");
    push_back(&vectorcito, "Programación");
    push_back(&vectorcito, "Prueba");
    push_back(&vectorcito, "Alo");
    push_back(&vectorcito, "Policia");
    push_back(&vectorcito, "Send");
    push_back(&vectorcito, "Help");

    /* push_back(&vectorcito, &libro1);
    push_back(&vectorcito, 1);
    push_back(&vectorcito, 2);
    push_back(&vectorcito, 3);
    push_back(&vectorcito, 4);
    push_back(&vectorcito, 5);
    push_back(&vectorcito, 6);
    push_back(&vectorcito, 7);
    push_back(&vectorcito, 8);*/

    printf("Antes de modificar vector:\n");
    for (int i = 0; i < vector_final(&vectorcito); i++)
    {
        printf(" %s, ", (char *)obtener_info(&vectorcito, i)); 
        // en los prints es necesario typecastear el tipo de dato que vamos a estar usando para el vector
    }
    printf("\n");

    // -> pruebas para eliminar elementos <-
    pop_back(&vectorcito);
    //pop_back(&vectorcito);
    vector_borrar(&vectorcito, 0);

    // -> pruebas para insertar en x posicion <-
    vector_insertar(&vectorcito, "Peng", 5);
    //vector_insertar(&vectorcito, 8, 3);
    
    // -> pruebas del vector_copia <- 
    vector_copias(&vectorcito_copia, 3, "Abijora");
    insertar_copias(&vectorcito, "Greta", 1, 3);

    printf("Vector copia:\n");
    for (int i = 0; i < vector_final(&vectorcito_copia); i++)
    {
        printf(" %s, ", (char *)obtener_info(&vectorcito_copia, i));
    }
    printf("\n");

    printf("Después de modificar vector:\n");
    for (int i = 0; i < vector_final(&vectorcito); i++)
    {
        printf(" %s, ", (char *)obtener_info(&vectorcito, i));
    }
    printf("\n");


    // -> pruebas con los iteradores <-
    void *init = begin(&vectorcito);

    void *final = end(&vectorcito, 1);

    printf("Iterador: \n");
    for (; init != NULL; init = next(init, sizeof(init), final))
    {
        printf(" %s, ", *((char **)init));
    }
    printf("\n");

    // -> pruebas de primer y ultimo elemento <- 
    ref_front(&vectorcito);
    ref_back(&vectorcito);

    // -> pruebas para eliminar valores entre first y last <- 
    borrar_medio(&vectorcito);

    printf("Después de modificar vector #2:\n");
    for (int i = 0; i < vector_final(&vectorcito); i++)
    {
        printf(" %s, ", (char *)obtener_info(&vectorcito, i));
    }
    printf("\n");

    // -> pruebas de liberación y eliminación <-
    clear(&vectorcito);
    empty(&vectorcito);
    //clear(&vectorcito_copia);
    empty(&vectorcito_copia);
}