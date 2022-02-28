# Actividad 3: Administración de un SO

## Integrantes
* *Karen Isabel Morgado Castillo A01027446 [CSF]*
* *Ana Paola Minchaca García A01026744 [CSF]*

## Histograma
Escriba un programa en C que parta de un directorio dado y descienda por el árbol de archivos registrando los tamaños de todos los archivos que encuentre. Cuando termine, deberá imprimir un histograma de los tamaños de archivo, utilizando una anchura de urna especificada como parámetro ( por ejemplo, para un valor de urna de 1024, todos los archivos con tamaño entre 0 y 1023 se colocan en una urna, los de 1024 a 2047 en otra y así sucesivamente).

## ¿Qué incluye?
### Archivos
- [x] main.c (Archivo principal)

## Funciones
1. Archivos
        
        En este método se hace un conteo de los archivos que se encuentran en un rango de tamaño de un directorio, aquí hacemos uso de las funciones que vienen incluidas en la libreria stat para poder saber el tamaño de estos archivos. Finalmente si el directorio no es encontrado marca un error.
        Aquí liberamos memoria y cerramos el directorio una vez el reccorrido fue acabado.
2. Imprimir
        
        Imprime el histograma con los rangos de urnas, el número de archivos y la división entre 10 del total en asteríscos.

## ¿Cómo correrlo?
Simplemente hay que compilar el archivo que viene incluido como en la siguiente línea de código. 

    gcc -o output main.c

Ya hecho esto no debería marcar ningún error por lo que pasaremos a correrlo con el archivo recién creado.

    ./output 
    
Al ingresar el directorio se debe escribir algo así:

    /Users/Hayoung/Desktop/tec/progra