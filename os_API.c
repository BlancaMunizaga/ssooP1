#include "os_API.h"
#include <unistd.h>
#include <stdio.h>

/*FUNCIONES GENERALES*/
/* 
Funcion para montar el disco en una partición en especifico. 
Establece como variable global la ruta local donde se encuentra el archivo .bin 
correspondiente al disco y tambien guarda como variable global la partición a montar
*/

void os_mount(char *diskname, int partition)
{
    printf("os_mount");
    file_name = diskname;
    disco = fopen(diskname, "r+b");

    id_particion = partition;

    // Obtener el path y guardar la ruta al archivo en variable global file_name
    getcwd(file_name, sizeof(file_name));
    strcat(file_name, "/");
    strcat(file_name, diskname);

    fseek(disco, 8 * partition, SEEK_SET); // Primer byte de la entrada validez+id_puntero
    fread(buffer_entrada, sizeof(buffer_entrada), 1, disco);
    int primer_byte = buffer_entrada;
    if (primer_byte < 127)
    {
        printf('partición no valida');
    }
    else
    {
        printf('partición valida');
        // Leyendo 3 bytes para encontrar el id_absoluto_particion y se guarda en la variable global
        fread(id_absoluto_particion, sizeof(id_absoluto_particion), 1, disco);
        fread(cantidad_bloques_particion, sizeof(cantidad_bloques_particion), 1, disco);
    }

    fseek(disco, 1, SEEK_CUR); // segundo byte
}

void os_bitmap(unsigned num)
{
    printf("os_bitmap");
}

int os_exists(char *filename)
{
    printf("os_exists");
}

void os_ls()
{
    printf("os_ls");
}

/*FUNCIONES MASTER BOOT*/

void os_mbt()
{

    printf("os_mbt");
    for (int i = 0; i < 128; i++)
    {
        fseek(disco, 8 * i, SEEK_SET); // Primer byte de la entrada validez+id_puntero
        fread(buffer_entrada, sizeof(buffer_entrada), 1, disco);
        int primer_byte = buffer_entrada;
        if (primer_byte < 127)
        {
            // no valida
        }
        else
        {
            // imprimir
            fread(id_absoluto_particion, sizeof(id_absoluto_particion), 1, disco);
            fread(cantidad_bloques_particion, sizeof(cantidad_bloques_particion), 1, disco);
        }
    }
}

void os_create_partition(int id, int size)
{
    printf("os_create_partition");
}

void os_delete_partition(int id)
// Restarle 127 al int que representa el primer byte de la entrada en la MBT
{
    fseek(disco, 8 * id, SEEK_SET); // Primer byte de la entrada validez+id_puntero
    fread(buffer_entrada, sizeof(buffer_entrada), 1, disco);
    int primer_byte = buffer_entrada;
    if (primer_byte < 127)
    {
        printf('partición no valida');
    }
    else
    {
        primer_byte -= 128;
        fputc(primer_byte, disco);
    }
}

void os_reset_mbt()
{
    printf("os_reset_mbt");
}

// extra

int is_valid(int primer_byte_entrada)
{
    return primer_byte_entrada > 127;
}