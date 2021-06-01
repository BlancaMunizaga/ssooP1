#include "../file_manager/os_API.h"
#include "../file_manager/osfile.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    // for(int i = 0; i<5; i++){
    //     printf("\n\nProbando particion %i\n", i);
    //     os_mount("src/discos/simdisk.bin", i);
    //     os_mbt();
    //     if(particion_valida){
    //         os_ls();
    //     }
    //     fclose(disco);
    // }
    os_mount("src/discos/simdisk.bin", 2);
    printf("Disco antes de la nueva particion: \n");
    os_mbt();
    os_create_partition(5, 50);
    printf("Disco despues de la nueva particion: \n");
    os_mbt();
    fclose(disco);
}
