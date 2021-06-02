#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void os_strerror(OS_ERROR err){
    switch(err){
        case OS_SEGFAULT:
            printf("Error: Se intenta leer fuera de un archivo.\n");
            break;
        case OS_FILENOTFOUND:
            printf("Error: El archivo no existe.\n");
            break;
        case OS_DISKNOTFOUND:
            printf("Error: El disco que busca no existe\n");
            break;
        case OS_PARTITIONOTFOUND:
            printf("Error: La particion no existe o es invalida.\n");
            break;
        case OS_SIZE:
            printf("Error: La particion no cabe en el disco.\n");
            break;
        case OS_MBTFULL:
            printf("Error: La MBT está llena.\n");
            break;
        case OS_INVALIDBITMAP:
            printf("Error: El bloque de bitmap introducido es inválido.\n");
            break;
        case OS_INVALIDID:
            printf("Error: El ID es inválido.\n");
            break;
        case OS_FILEEXISTS:
            printf("Error: Ya existe un archivo con ese nombre.\n");
            break;
        case OS_WRONGMODE:
            printf("Error: El modo introducido no es compatible con esta accion.\n");
            break;
    }
}