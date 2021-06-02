#include "error.h"

void os_strerror(OS_ERROR err){
    switch(err){
        case OS_SEGFAULT:
            printf("Error: Se intenta leer fuera de un archivo.");
            break;
        case OS_FILENOTFOUND:
            printf("Error: El archivo no existe.");
            break;
        case OS_DISKNOTFOUND:
            printf("Error: El disco que busca no existe");
            break;
        case OS_PARTITIONOTFOUND:
            printf("Error: La particion no existe o es invalida.");
            break;
        case OS_SIZE:
            printf("Error: La particion no cabe en el disco.");
            break;
        case OS_MBTFULL:
            printf("Error: La MBT está llena.");
            break;
        case OS_INVALIDBITMAP:
            printf("Error: El bloque de bitmap introducido es inválido.");
            break;
        case OS_INVALIDID:
            printf("Error: El ID es inválido.");
            break;
        case OS_FILEEXISTS:
            printf("Error: Ya existe un archivo con ese nombre.");
            break;
    }
}