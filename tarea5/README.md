# Actividad 5: IPC

## Integrantes
* *Karen Isabel Morgado Castillo A01027446 [CSF]*
* *Ana Paola Minchaca García A01026744 [CSF]*

## IPC
Se quiere implementar un sistema de adquisición de datos para llevar un registro de la velocidad del viento en  diferentes puntos de una ciudad. Para lo anterior se tiene en cuenta lo siguiente:

En cada punto de la ciudad se ha instalado un sensor
Cada sensor, obtiene la velocidad del viento  a intervalos de 3 segundos y se lo comunica a un sistema centralizado (mediante un socket).
- El sistema centralizado, registra cada lectura en una estructura de datos en memoria.
- Por cada sensor se creará un arreglo.
- Dentro de cada arreglo se almacenará otro arreglo por cada día del mes.
- El arreglo de un día debe almacenar todas las lecturas de ese día, para el sensor correspondiente.
- El sistema centralizado se encargará de generar reportes estadísticos con la información registrada.
- Este sistema procesa la estructura de registro de datos e imprime el valor mínimo, el máximo y el  promedio de las lecturas para cada sensor.

## ¿Qué incluye?
### Archivos
- [x] sensor.c (generación de reporte por sensor)
- [x] consola.c (consola central)

## Funciones
1. Generar reporte
        Método para calcular el reporte por sensor, recibe un arreglo, la cantidad de sensores, los datos por sensor, al igual que los días. De forma automática se genera el reporte por # de sensor y lo imprime en pantalla, con el valor mínimo de temperatura, el máximo y el promedio. 
        
2. Salir del programa
        Se termina el programa y libera la memoria.


## ¿Cómo correrlo?
Simplemente hay que compilar el archivo sensor que viene incluido como en la siguiente línea de código, para así después correrlo agregando la IP del usuario. En las siguientes líneas de código viene un ejemplo. 

    gcc -o sensor sensor.c

    ./sensor 192.168.0.7  

Se le hará una pregunta al usuario de cuantos sensores quiere conectar, una vez hecho esto, estará esperando conexiones por parte de la consola. 

Ya hecho esto, lo siguiente que compilar el archivo consola. En este caso se debe escribir la misma IP que se escribió para el archivo de sensor, así como un ID de sensor para que el programa sepa que ese sensor se está conectando.

    gcc -o consola consola.c
    
    ./consola 192.168.0.7 1 

## Consideraciones
Los números de la temperatura si se generan de forma aleatoria y si se mandan al sensor. Sin embargo, no se reflejan en el reporte.

Podemos ver el lado del archivo consola, aquí es en donde generamos los números random para escribirlos en el buffer y podemos ver que si los genera random.

![hello](https://i.imgur.com/DF5OW5g.png)

Ahora, podemos ver lo que se imprime al mismo tiempo en el archivo sensor, donde vemos que son los mismos números.

![hello1](https://i.imgur.com/D2IA4NY.png)

El problema viene en la generación del reporte, podemos ver que el valor que se imprime es el primero que se generó en los números random, por lo que tiene ese pequeño problema, pero al tener más de un sensor, el promedio ya da un número distinto. 

![hello2](https://i.imgur.com/SYu5CbX.png)