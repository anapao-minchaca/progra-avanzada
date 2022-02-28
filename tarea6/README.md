# Actividad 6: Sockets y Señales

## Integrantes
* *Karen Isabel Morgado Castillo A01027446 [CSF]*
* *Ana Paola Minchaca García A01026744 [CSF]*

## Semáforos
* Se desea diseñar un sistema para controlar la sincronización de semáforos en un cruce de dos vías principales en la ciudad.
* Para que todo funcione correctamente y no ocurran accidentes, solo habrá un semáforo en verde a la vez.
* En el cruce existirán 4 semáforos, uno en cada esquina, que se comunicarán entre sí para cederse el turno de ponerse en verde.
* Cada semáforo estará en verde 30 segundos y posteriormente se podrá en rojo y le cederá el turno al semáforo que se encuentre a su derecha, enviándole una señal SIGUSR1.
* Mientras un semáforo no reciba esta señal deberá permanecer en rojo.
* Se desea contar con una consola de administración centralizada que se encuentra en un centro de monitoreo remoto.
* Cada semáforo al cambiar de estado, le enviará a la consola su nuevo estado y ésta mostrará en la pantalla el estado actual de cada semáforo.
* Como medida de seguridad, desde la consola se puede enviar un mensaje a todos los semáforos para ponerlos en rojo o en intermitente (de 00:00 a 6:00) según el siguiente criterio:
    * cuando la consola recibe un Ctrl+Z le envía el mensaje de ponerse en rojo,
    * cuando recibe un Ctrl+C le envía el de ponerse en intermitente.
    * cuando los semáforos reciben los mensajes anteriores, cambian de estado inmediatamente sin importar cómo se encontraban en ese momento.
    * El mismo mensaje se envía para regresar los semáforos al estado en que se encontraban. Por ejemplo, si se manda el mensaje de ponerse en rojo una vez, todos los semáforos almacenarán su estado actual y se pondrán en rojo; cuando vuelvan a recibir el mismo mensaje, restaurarán su estado al mismo punto en que se encontraban inicialmente.
* La consola siempre que reciba un cambio de estado de un semáforo, mostrará la información en la pantalla. Los semáforos solo deben responder a la señal SIGUSR1 enviada por otro semáforo. Deben bloquear todas las demás señales.
* La consola de monitoreo solo debe responder a las señales Ctrl+C y Ctrl+Z. Todas las demás señales deben bloquearse.

## ¿Qué incluye?
### Archivos
- [x] semaforo.c (control de semáforos)
- [x] consola.c (consola central)

## Funciones
En consola.c
1. Control C (Intermitente)
        
        Método para escribir en el buffer la letra "I" de intermitente.
        
2. Control Z (Rojo)

        Método para escribir en el buffer la letra "R" de rojo.

En semaforo.c
1. Control Semáforo
        
        Método para que los semáforos empiecen en verde y duren 30 segundos.
        
2. Cambio Semáforo

        Método para que los semáforos cambien a rojo y que el siguiente cambie a verde.

## ¿Cómo correrlo?
Simplemente hay que compilar el archivo consola que viene incluido como en la siguiente línea de código, para así después correrlo agregando la IP del usuario. En las siguientes líneas de código viene un ejemplo. 

    gcc -o consola consola.c

    ./consola 192.168.0.7  

Paso seguido se debe correr el archivo semáforo de la misma manera, este comando se debe correr en 4 terminales distintas para asegurar la creación de los 4 semáforos. 
    
    gcc -o semaforo semaforo.c

    ./semaforo 192.168.0.7 

Aparecerá en pantalla el ID de ese semáforo y acto seguido se le preguntará al usuario el color con el que va a empezar ese semáforo. Un ejemplo de esto sería como lo siguiente. 

![image](https://i.imgur.com/SvvYfUp.png)

Ahí vemos que después nos pide el PID del siguiente semáforo, aquí hay que escribir el PID del semáforo que está a la derecha, así todos los semáforos quedarán conectados y sabrán cúal se encuentra en verde.

![image](https://i.imgur.com/lxJEhDS.png)

Del lado de la consola vemos que imprime que semáforo está en que estado, esto cambia cada 30 segundos. 

De igual forma, cuando hacemos Ctrl-C o Ctrl-Z, imprime en pantalla que señal se recibió y se debe volver a escribir Ctrl-C o Ctrl-Z si quiere que regrese al estado que estaba antes.

![image](https://i.imgur.com/COzXq9J.png)
![image](https://i.imgur.com/PmfZxnN.png)