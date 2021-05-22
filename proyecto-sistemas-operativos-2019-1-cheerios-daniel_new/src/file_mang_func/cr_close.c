#include <stdlib.h>
#include <stdio.h>
#include "file_mang_func.h"

// 2KB = 2048B
// 1B  = 8b
// 1B  = 2h

int cr_close(crFILE* file_desc){
    /*
    Cierra el Archivo indicado por file_desc.
    Debe garantizar que cuando retorne el archivo esté actualizado en disco
    */
    if(file_desc == NULL){
        return 0;
    }
    int exists = cr_exists(file_desc->path);
    if(!file_desc->mode && !exists){  // Si el archivo está en modo read debe existir
        printf("ERROR[cr_close]: El archivo no existe\n");
        free(file_desc);  // Free crFILE pointer
        return 0;
    }
    free(file_desc);  // Free crFILE pointer
    return 0;
}
