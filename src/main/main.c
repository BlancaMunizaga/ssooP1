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
    //os_ls();
    //os_bitmap(0);
    //os_ls();
    //os_create_partition(6, 4000);
    //printf("Disco despues de la nueva particion: \n");
    //os_mbt();
    //int ret = os_exists("mystery.mp3");
    //printf("%d\n", ret);
    //int ret1 = os_exists("mysteri.mp3");
    //printf("%d\n", ret1);
    //os_delete_partition(3);
    //os_ls();
    //os_rm("mysteri.mp3");
    //printf("Disco después de la nueva partición: \n");
    //os_mbt();
    //os_ls();

    os_mount("src/discos/simdisk.bin", 4);
    osFile * file_desc = os_open("pepeloni.txt", 'r');
    unsigned char buffer[file_desc->tamano];
    
    int bytes_leidos = os_read(file_desc, buffer, file_desc->tamano);
    FILE* prueba = fopen("prueba.txt", "wb+");
    for (int i = 0; i<file_desc->tamano; i++)
    {
        fputc(buffer[i], prueba);
    }
    fclose(prueba);
    printf("%s\n", buffer);
    printf("bytes leidos = %d\n", bytes_leidos);
    fclose(disco);
}
