#include "../file_manager/os_API.h"
#include "../file_manager/osfile.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (*argv[1] == '0') // monta el disco y muestra el masterboot table y luego muestra los archivos  de cada partición
    {
        for (int i = 1; i < 5; i++)
        {
            printf("\n\nProbando particion %i\n", i);
            os_mount("src/discos/simdisk.bin", i);
            if (i == 1)
            {
                os_mbt();
            }
            if (particion_valida)
            {
                printf("Los archivos de esta partición son:\n");
                os_ls();
            }

            fclose(disco);
        }
    }
    else if (*argv[1] == '1')
    { // el disco que se quiere montar es invalido porque no existe el archivo
        os_mount("src/discos/disco_no_existe.bin", 0);
    }
    else if (*argv[1] == '2')
    {
        // Ver si un archivo existe.
        char *filename = argv[2];
        os_mount("src/discos/simdisk.bin", 4);
        printf("Viendo si existe el archivo [%s]\n", filename);
        os_exists(filename);
        printf("Borrando el archivo [%s]\n", filename);
        os_rm(filename);
        printf("Mostrando que el archivo fue borrado con ls [%s]\n", filename);
        os_ls();
        printf("Intentando ver archivo si existe el archivo recien borrado.\n");
        os_exists(filename);
        printf("Intentando borrar archivo que no existe \n");
        os_rm(filename);
        fclose(disco);
    }
    else if (*argv[1] == '3')
    { //montamos el disco y mostramos todo el bitmap
        os_mount("src/discos/simdisk.bin", 4);
        printf("Imprimiendo todo el bitmap\n");
        os_bitmap(0);
        //mostramos un bloque especifico del bitmap
        printf("Imprimiendo el bloque 2 del bitmap\n");
        os_bitmap(2);
        //Pedir un bloque invalido
        printf("Imprimiendo bloque invalido (20) del bitmap\n");
        os_bitmap(20);
        fclose(disco);
    }
    else if (*argv[1] == '4')
    {
        //Montamos la 4 partición 4
        os_mount("src/discos/simdisk.bin", 4);
        printf("Disco antes de la nueva particion: \n");
        //Imprimimos la mbt
        os_mbt();
        //Creamos la particion
        printf("Disco después de la nueva particion: \n");
        os_create_partition(6, 4000);
        //Imprimimos la mbt
        printf("Mostrando mbt despues de crear nueva partición: \n");
        os_mbt();
        //Borramos la particion
        printf("Disco después de borrar la particion recién creada: \n");
        os_delete_partition(6);
        //Volvemos a imprimir la mbt
        printf("Mostrando mbt despues de borrar la particion: \n");
        os_mbt();
        //Probando error de borrar particion invalida
        printf("Intentando borrar particion invalida: \n");
        os_delete_partition(6);
        // Probar errores
        printf("Probando crear una partición demasiado grande: \n");
        os_create_partition(6, 40000000);
        // Va a tirar error.
        printf("Probando crear una partición con ID invalido: \n");
        os_create_partition(180, 400);
        printf("Mostrando mbt despues de errores: \n");
        os_mbt();
        fclose(disco);
    }
    else if (*argv[1] == '5')
    {
        //Montamos la 4 partición 4
        os_mount("src/discos/simdisk.bin", 2);
        //Usamos os_open para abrir en modo de lectura nene.txt
        osFile *file_desc = os_open("nene.txt", 'r');
        //lo leemos entero
        printf("leemos el tamaño completo\n");
        unsigned char buffer[file_desc->tamano];
        int bytes_leidos = os_read(file_desc, buffer, file_desc->tamano);
        // y escribimos los bytes en un  archivo txt para ver el resultado
        for (int i = 0; i < file_desc->tamano; i++)
        {
            printf("%c", buffer[i]);
        }
        os_close(file_desc);

        printf("\nbytes leidos = %d\n", bytes_leidos);

        fclose(disco);

        os_mount("src/discos/simdisk.bin", 2);
        osFile *file_desc = os_open("nene.txt", 'r');
        printf("Leemos 3000 bytes \n");
        int bytes_leidos = os_read(file_desc, buffer, 3000);
        // y escribimos los bytes en un  archivo txt para ver el resultado
        for (int i = 0; i < 3000; i++)
        {
            printf("%c", buffer[i]);
        }
        printf("\nbytes leidos = %d\n", bytes_leidos);

        printf("Leemos 521 bytes restante \n");
        int bytes_leidos = os_read(file_desc, buffer, 521);
        for (int i = 0; i < 521; i++)
        {
            printf("%c", buffer[i]);
        }
        os_close(file_desc);
        printf("\nbytes leidos = %d\n", bytes_leidos);

        fclose(disco);
    }
    return 0;
}
