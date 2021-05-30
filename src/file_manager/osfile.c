#include "osfile.h"
#include "os_API.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int find_pos_indice_block(char *filename)
{
    for (int entrada; entrada < 64; entrada++) // Se recorre cada entrada del bloque directorio.
    {
        fseek(disco, inicio_particion + 32 * entrada, SEEK_SET);
        unsigned char byte_validez[1];
        fread(byte_validez, sizeof(byte_validez), 1, disco);
        int validez = (int)byte_validez[0];
        if (validez) // si la entrada es valida se continua.
        {
            unsigned char id_relativo_bloque_indice[3];
            char nombre_archivo[28];
            fread(id_relativo_bloque_indice, sizeof(id_relativo_bloque_indice), 1, disco);
            fread(nombre_archivo, sizeof(nombre_archivo), 1, disco);
            if (strcmp(nombre_archivo, filename)) // Se compara el nombre del archivo con el que se busca.
            {
                int id_absoluto_bloque_indice;
                id_absoluto_bloque_indice = id_relativo_bloque_indice[2] + (id_relativo_bloque_indice[1] << 8) + (id_relativo_bloque_indice[0] << 16);
                id_absoluto_bloque_indice += inicio_particion;
                return id_absoluto_bloque_indice; // Si son iguales se retorna 1
            }
        }
    }
    return 0;
}

osFile *os_open(char *filename, char mode)
{
    int exists = os_exists(filename);
    int pos_indice = find_pos_indice_block(filename);

    if (mode == 'r')
    {
        if (exists)
        {
            osFile *osfile = malloc(sizeof(osFile));
            osfile->mode = 0;
            osfile->pos_indice = pos_indice;

            fseek(disco, pos_indice, SEEK_SET);
            unsigned char tamano_archivo[5];
            fread(tamano_archivo, sizeof(tamano_archivo), 1, disco);
            long int tamano = (long int) tamano_archivo[4] + ( (long int) tamano_archivo[3] << 8) + ( (long int) tamano_archivo[2] << 16) + ( (long int) tamano_archivo[1] << 24) + ( (long int) tamano_archivo[0] << 32);
            osfile->tamano = (long int) tamano;
            return osfile;
        }
        else
        {
            printf("ERROR: Archivo no existe.\n");
            return NULL;
        }
    }
    else if (mode == 'w')
    {

        if (exists)
        {
            printf("ERROR: Archivo con ese nombre ya existe.\n");
            return NULL;
        }
        else
        {
            osFile *osfile = malloc(sizeof(osFile));
            osfile->mode = 1;
            osfile->pos_indice = pos_indice;
            
            fseek(disco, pos_indice, SEEK_SET);
            unsigned char tamano_archivo[5];
            fread(tamano_archivo, sizeof(tamano_archivo), 1, disco);
            long int tamano = (long int) tamano_archivo[4] + ( (long int) tamano_archivo[3] << 8) + ( (long int) tamano_archivo[2] << 16) + ( (long int) tamano_archivo[1] << 24) + ( (long int) tamano_archivo[0] << 32);
            osfile->tamano = (long int) tamano;
            return osfile;
        }
    }
    return NULL;
}

int os_read(osFile *file_desc, void *buffer, int nbytes)
{
    int tamano = file_desc->tamano;
    int bytes_r_w = file_desc->bytes_r_w;
    int bytes_leidos = 0;
    if (bytes_r_w + nbytes <= tamano)
    {
        for (int i = 0; i < nbytes; i++)
        {
            int numero_bloque_datos = bloque_de_datos(bytes_r_w);
            fseek(disco, file_desc->pos_indice + 5 + numero_bloque_datos * 3, SEEK_SET);
            unsigned char puntero[3];
            fread(puntero, sizeof(puntero), 1, disco);
            int pos_bloque_datos;
            pos_bloque_datos = puntero[2] + (puntero[1] << 8) + (puntero[0] << 16);

            int pos_byte = bytes_r_w - 2048 * numero_bloque_datos;
            fseek(disco, inicio_particion + pos_bloque_datos + pos_byte, SEEK_SET);
            fread(buffer, 1, 1, disco);
            bytes_r_w += 1;
            bytes_leidos++;
        }
    }
    return bytes_leidos;
}

int os_write(osFile *file_desc, void *buffer, int nbytes)
{
    int tamano = file_desc->tamano;
    int bytes_r_w = file_desc->bytes_r_w;
    int bytes_leidos = 0;
    if (bytes_r_w + nbytes <= tamano)
    {
        for (int i = 0; i < nbytes; i++)
        {
        }
    }
    return bytes_leidos;
}

int os_close(osFile *file_desc)
{
    return 0;
}

// extya

int bloque_de_datos(int byte_r_w)
// cantidad de bitmaps requeridos para una particion de tamaño
{
    if (byte_r_w % 2048 == 0)
    {
        return (byte_r_w / 2048) - 1;
    }
    else
    {
        return (byte_r_w / 2048);
    }
}