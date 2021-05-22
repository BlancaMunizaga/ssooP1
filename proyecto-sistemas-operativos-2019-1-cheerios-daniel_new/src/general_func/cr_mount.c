#include "general_func.h"

char path_disk[400];

/*
Función para montar el disco. Establece como variable global la
ruta local donde se encuentra el archivo .bin correspondiente al disco.
*/

void cr_mount(char* diskname){
    if (getcwd(path_disk, sizeof(path_disk)) != NULL) {
       }
    printf("mounting disk\n");    
    strcat(path_disk, "/");
    strcat(path_disk, diskname);
    printf("disk mounted: %s\n", path_disk);
}
