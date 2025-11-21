# Proyecto---Bash

## Descripción del Proyecto
Este proyecto presenta la implementación de un intérprete de comandos interactivo (Shell) diseñado para entornos Linux y desarrollado íntegramente en lenguaje C. El propósito fundamental de este programa es ilustrar el funcionamiento interno de un sistema operativo mediante la aplicación práctica de conceptos de bajo nivel.
Para cumplir con rigorosos estándares de ingeniería, el desarrollo se ha centrado en el uso exclusivo de Llamadas al Sistema (System Calls) para las operaciones críticas. Esto implica que la gestión de procesos, la manipulación de archivos y el control de memoria se realizan interactuando directamente con el Kernel, prescindiendo de las abstracciones que ofrecen las librerías estándar de alto nivel.

## Funcionalidades del Sistema

- Comandos Integrados (Built-in):

```bash
cd : 
Modifica el directorio de trabajo actual mediante la syscall chdir.

pwd : 

Obtiene y muestra la ruta absoluta del directorio actual usando getcwd.

mkdir : 

Solicita al sistema la creación de un nuevo directorio con permisos 0755.

rm :

Elimina inodos del sistema de archivos, ya sean ficheros (unlink) o directorios (rmdir).

mv :

Modifica la entrada de directorio para renombrar o mover archivos (rename).

cp :

Realiza una copia byte a byte de un archivo origen a un destino (open, read, write).

cat :

Lee secuencialmente un archivo y vuelca su contenido al descriptor de salida estándar.

ls :

Itera sobre las entradas del directorio actual para listar su contenido.

exit :

Cierra ordenadamente los descriptores abiertos y termina la ejecución del proceso.
