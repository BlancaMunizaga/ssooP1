#include "../file_manager/os_API.h"
#include "../file_manager/osfile.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    // Primero por cada partición hacemos os_mount imprimimos  el mbt y vemos los acrhivos que tienen cada partición (os_ls)
    // for(int i = 0; i<5; i++){
    //     printf("\n\nProbando particion %i\n", i);
    //     os_mount("src/discos/simdisk.bin", i);
    //     if (i==0){
    //         os_mbt();
    //     }
    //     if(particion_valida){
    //         os_ls();
    //     }
        
    //     fclose(disco);
    // }
    
    //Montamos la segunda particion
    os_mount("src/discos/simdisk.bin", 2);
    // printf("Disco antes de la nueva particion: \n");
    // //Imprimimos la mbt
    // os_mbt();
    // printf("Archivos: \n");
    // //Imprimimos los archivos de la particion
    // os_ls();
    // printf("Bitmap: \n");
    // //Imprimimos todos los bloques bitmap y la cantidad bloques libres y ocupados
    // os_bitmap(0);
    
    //Creamos la particion
    printf("Disco después de la nueva particion: \n");
    os_create_partition(6, 4000);
    //Imprimimos la mbt
    os_mbt();
    //Borramos la particion
    //os_delete_partition(6);
    //Volvemos a imprimir la mbt
    //os_mbt();
    
    // //Mostramos los archivos existentes en la particion
    // os_ls();
    // // Vemos si en la partición existe el archivo mystery.mp3 (que exixte)
    // printf("");
    // int ret = os_exists("mystery.mp3");
    // printf("exists = %d\n", ret);
    // //Vemos si en la partición existe mysteri.mp3 (que no existe)
    // int ret1 = os_exists("mysteri.mp3");
    // printf("exists = %d\n", ret1);
    // // eliminamos el archivo mystery.mp3
    // os_rm("mystery.mp3");
    // // Hacemos os_ls para mostrar que el archivo fue borrado
    // os_ls();

    // //Cerramos esta particion
    // fclose(disco);
    
    // //Montamos la siguiente particion, la 4
    // os_mount("src/discos/simdisk.bin", 4);
    // //Usamos os_open para abrir en modo de lectura nene.txt
    // osFile * file_desc = os_open("nene.txt", 'r');
    // unsigned char buffer[file_desc->tamano];
    // // leemos el archivo completo
    // int bytes_leidos = os_read(file_desc, buffer, file_desc->tamano);
    // // y escribimos los bytes en un  archivo txt para ver el resultado
    // FILE* prueba = fopen("pruebaaaaaa.txt", "wb+");
    // for (int i = 0; i<file_desc->tamano; i++)
    // {
    //     fputc(buffer[i], prueba);
    // }
    // fclose(prueba);
    // // Cerramos el arhivo nene.txt (ver leaks para comprobar)
    // os_close(file_desc);
    // printf("bytes leidos = %d\n", bytes_leidos);

    // //Mostramos el estado actual de la mbt
    // os_mbt();
    // //Reseteamos la mbt
    // os_reset_mbt();
    // //Mostramos como quedo la mbt
    // os_mbt();
    fclose(disco);
}
