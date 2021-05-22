#include "file_mang_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern char path_disk[400];

// Funcion que recibe el nombre del archivo (el path si esta dentro de
// un directorio y el puntero (entero) del bloque de indice del archvo
// el cual se obtiene del bloque de directorio. En cr_ls esta el print
// Luego, esta funcion se encarga de descargar el archivo en la ruta
// dada.
int unload_file(char *name, unsigned int index_file) {
    FILE* f = fopen(path_disk, "rb");       // Aca hay que poner el nombre del disco
	unsigned char *buffer = malloc( sizeof( unsigned char ) * 2048 );
    fseek(f, 2048 * index_file, SEEK_SET);              // Posiciono el puntero en el bloque indice del archivo
    fread(buffer, 1, 2048, f);                          // Leo el bloque completo
    unsigned int size = (unsigned int) buffer[3];       // Obtengo el tamaño del archivo
    size += (unsigned int) buffer[2] * pow(2, 8);
    size += (unsigned int) buffer[1] * pow(2, 16);
    size += (unsigned int) buffer[0] * pow(2, 24); 
    FILE* f_new = fopen(name, "w+");                    // Abro el archivo a cargar
    unsigned char *block = malloc( sizeof( unsigned char ) * 2048 );
    unsigned int writed = 0;
    for(int i = 2; i < 502; i++) {                      // Reviso la lista de punteros
        unsigned int ptr = 0;                           // Almaceno el puntero en un entero (no se me ocurrio otra forma)
        ptr += (unsigned int) buffer[i * 4 + 3];
        ptr += (unsigned int) buffer[i * 4 + 2] * pow(2, 8);
        ptr += (unsigned int) buffer[i * 4 + 1] * pow(2, 16);
        ptr += (unsigned int) buffer[i * 4] * pow(2, 24);
        fseek(f, 2048 * ptr, SEEK_SET);             // Posiciono el puntero en el bloque de datos correspondiente
        if (writed + 2048 > size) {                 // Si queda por escribir menos de un bloque
            fread(block, 1, size - writed, f);
            fwrite(block, 1, size - writed, f_new);
            writed += (size - writed);
            break;
        } else {
            fread(block, 1, 2048, f);               // Lo leo y lo almaceno en block
            fwrite(block, 1, 2048, f_new);          // Copio el contenido de block en el nuevo archivo
            writed += 2048;
        }
    }

    for(int i = 502; i < 512; i++) {                    // Reviso los punteros de direccionamiento indirecto
        unsigned int ptr = 0;
        ptr += (unsigned int) buffer[i * 4 + 3];
        ptr += (unsigned int) buffer[i * 4 + 2] * pow(2, 8);
        ptr += (unsigned int) buffer[i * 4 + 1] * pow(2, 16);
        ptr += (unsigned int) buffer[i * 4] * pow(2, 24);
        if (writed < size) {                                 // Si existe un puntero
            unsigned char *ind = malloc( sizeof( unsigned char ) * 2048 ); 
            fseek(f, 2048 * ptr, SEEK_SET);
            fread(ind, 1, 2048, f);                     // Almaceno el bloque de direccionamiento indirecto
            for(int j = 0; j < 512; j++) {              // Itero sobre la totalidad de sus bloques
                unsigned int ptr2 = 0;
                ptr2 += (unsigned int) ind[j * 4 + 3];
                ptr2 += (unsigned int) ind[j * 4 + 2] * pow(2, 8);
                ptr2 += (unsigned int) ind[j * 4 + 1] * pow(2, 16);
                ptr2 += (unsigned int) ind[j * 4] * pow(2, 24);
                fseek(f, 2048 * ptr2, SEEK_SET);
                if (writed + 2048 > size) {
                    fread(block, 1, size - writed, f);
                    fwrite(block, 1, size - writed, f_new);
                    writed += (size - writed);
                    break;
                } else {
                    fread(block, 1, 2048, f);
                    fwrite(block, 1, 2048, f_new);
                    writed += 2048;
                }
            }
        }
    }
    free(buffer);
    free(block);
    fclose(f);
    fclose(f_new);
    return 0;
}


int cr_unload(char *orig, char *dest){
    int o_exists = cr_exists(orig);
    if(o_exists){
        int *o_idx = 0;
        search(o_idx, orig);
        unsigned int orig_idx = (unsigned int) *o_idx;
        int res = unload_file(dest, orig_idx);  // To fix for working with generic orig and dest
        return res;
    }
    printf("ERROR[cr_unload]: origen o destino no existen");
    return 0;
}
