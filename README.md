# Proyecto---Bash

## Descripción del Proyecto
Este proyecto consiste en la implementación de un intérprete de comandos interactivo (Shell) para sistemas Linux. Ha sido desarrollado en lenguaje C siguiendo estrictos estándares de ingeniería de sistemas.
El objetivo principal es demostrar el dominio de la programación a bajo nivel, utilizando exclusivamente **Llamadas al Sistema (System Calls)** para la gestión de procesos, manipulación de archivos y control de memoria, evitando el uso de librerías de alto nivel para las operaciones críticas del kernel.

## ⚙️ Compilación y Ejecución

Para compilar el código fuente y generar el binario ejecutable, abre una terminal en la carpeta del proyecto y ejecuta:

```bash
Comando,Uso,Descripción Técnica
cd,cd <ruta>,Cambia el directorio de trabajo (chdir).
pwd,pwd,Muestra la ruta absoluta actual (getcwd).
mkdir,mkdir <nombre>,Crea un directorio con permisos 0755 (mkdir).
rm,rm <target>,Elimina archivos (unlink) o directorios (rmdir).
mv,mv <viejo> <nuevo>,Renombra o mueve archivos (rename).
cp,cp <src> <dst>,"Copia binaria de archivos (open, read, write)."
cat,cat <archivo>,Vuelca el contenido de un archivo a STDOUT.
ls,ls,"Lista el directorio actual (opendir, readdir)."
exit,exit,Cierra el descriptor de entrada y termina el proceso.
