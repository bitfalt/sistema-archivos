# sistema-archivos

# Instituto Tecnológico de Costa Rica
## Escuela de Ingeniería en Computación
### Implementación de un sistema de archivos

- Greivin Mauricio Fernández Brizuela c.2022437510
- Daniel Alonso Garbanzo Carvajal c.2022117129
- Erika Michelle Cerdas Mejias c.2022138199

**IC-6600 - Principios de Sistemas Operativos**
* Fecha de entrega: 30 de octubre

## Descripción de la tarea

Esta tarea implementa un sistema de archivos en lenguaje C, donde se gestionan archivos, su almacenamiento en bloques de memoria y su administración mediante comandos. El sistema simula la creación, lectura, escritura, listado y eliminación de archivos en bloques de memoria.

## Descripción de la solución

Todos los archivos se simulan mediante una estructura de datos, los cuales se almacenan en un array de dicha estructura para saber cuáles archivos existen, asi mismo como su información (data en el archivo y el bloque al que pertenecen). Cada funcionalidad (`CREATE`, `WRITE`, `READ`, `DELETE`, `LIST`) están implementadas en diferentes funciones en el archivo de `file_system.c`. Además, el archivo `file_reader.c` se encarga de hacer el "parsing" del archivo de entrada. Los diferentes archivos que se usaron para pruebas están en la carpeta `input`, donde se intenta abarcar la mayor cantidad de errores posibles para demostrar el "error handling" adecuado. Si se intenta crear un archivo ya existente, el programa indica un error explicando que el archivo ya existe.


## Requisitos para correr el proyecto
- `Make` instalado para compilar el proyecto, en caso de algún cambio.

Para correr el programa se utiliza el siguiente comando:
```bash
./file_system input/<file>
```
Se debe reemplazar file por el archivo de prueba que se quiere correr, los archivos existentes son: `test1.txt`, `test2.txt`, ..., `test6.txt`.
