# Actividad 1: Aritmética de Apuntadores

## Integrantes
* *Karen Isabel Morgado Castillo A01027446 [CSF]*
* *Ana Paola Minchaca García A01026744 [CSF]*

## Puerto pesquero
Un puerto pesquero desea llevar un registro de los barcos de pesca, sus propietarios y los tripulantes de cada embarcación. Para lo anterior, se desea almacenar de cada persona su nombre, apellidos, edad, y rol que desempeña dentro de la tripulación (arponero, cocinero, vigía, capitán, etc.). De las embarcaciones se requiere conocer el nombre, la eslora, la manga, el número máximo de tripulantes y su tripulación (información de los tripulantes).

## ¿Qué incluye?
### Archivos
- [x] act1v2.c (Archivo principal)

## Funciones
1. Incorporar barcos
        
        Se le pide al usuario toda la información necesaria para incorporar un barco al puerto. Si la memoria reservada ya está llena, se hace un realloc para agregar más espacio. 
2. Incorporar tripulantes
        
        Dependiendo del id del barco se revisará si existe espacio disponible en ese barco para poder proceder a agregar al tripulante con todos sus datos correspondientes.
3. Información de barcos y sus tripulantes

        Se muestra en pantalla la información de todos los barcos registrados: el nombre, la eslora, manga, propietario, lugares disponibles y la tripulación a bordo.  
0. Salir del programa
        
        Se termina el programa y libera la memoria.


## ¿Cómo correrlo?
Simplemente hay que compilar el archivo que viene incluido como en la siguiente línea de código. 

    gcc -o outputv2 act1v2.c

Ya hecho esto no debería marcar ningún error por lo que pasaremos a correrlo con el archivo recién creado.

    ./outputv2    
