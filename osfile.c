#include "osfile.h"
#include "os_API.h"

int find_pos_indice_block(char *filename)
{
    for (int entrada; entrada < 64; entrada++) // Se recorre cada entrada del bloque directorio.
    {
        fseek(disco, inicio_particion + 32 * entrada, SEEK_SET);
        unsigned char byte_validez;
        fread(byte_validez, sizeof(byte_validez), 1, disco);
        int validez = (int)byte_validez;
        if (validez) // si la entrada es valida se continua.
        {
            unsigned char id_relativo_bloque_indice[3];
            unsigned char nombre_archivo[28];
            fread(id_relativo_bloque_indice, sizeof(id_relativo_bloque_indice), 1, disco);
            fread(nombre_archivo, sizeof(nombre_archivo), 1, disco);
            if (strcmp(nombre_archivo, filename, 32)) // Se compara el nombre del archivo con el que se busca.
            {
                int id_absoluto_bloque_indice;
                id_absoluto_bloque_indice = id_relativo_bloque_indice[0] + (id_relativo_bloque_indice[1] << 8) + (id_relativo_bloque_indice[2] << 16);
                id_absoluto_bloque_indice += inicio_particion;
                return id_absoluto_bloque_indice; // Si son iguales se retorna 1
            }
        }
    }
}

osFile *os_open(char *filename, char mode)
{
    int exists = os_exists(filename);
    int pos_indice = find_pos_indice_block(filename);

    if (mode = "r")
    {
        osfile->mode = 0;
        if (exists)
        {
            osFile *osfile = malloc(sizeof(osFile));
            osfile->pos_indice = pos_indice;

            fseek(disco, pos_indice, SEEK_SET);
            unsigned char tamaño_archivo[5];
            fread(tamaño_archivo, sizeof(tamaño_archivo), 1, disco);
            tamaño = tamaño_archivo[0] + (tamaño_archivo[1] << 8) + (tamaño_archivo[2] << 16) + (tamaño_archivo[3] << 24) + (tamaño_archivo[3] << 32);
            osfile->tamaño = tamaño_archivo;
            return osfile
        }
        else
        {
            printf("ERROR: Archivo no existe.\n");
            return NULL;
        }
    }
    else if (mode = "w")
    {
        osfile->mode = 1;
        if (exists)
        {
            printf("ERROR: Archivo con ese nombre ya existe.\n");
            return NULL;
        }
        else
        {
            osFile *osfile = malloc(sizeof(osFile));
            osfile->pos_indice = pos_indice;

            fseek(disco, pos_indice, SEEK_SET);
            unsigned char tamaño_archivo[5];
            fread(tamaño_archivo, sizeof(tamaño_archivo), 1, disco);
            tamaño = tamaño_archivo[0] + (tamaño_archivo[1] << 8) + (tamaño_archivo[2] << 16) + (tamaño_archivo[3] << 24) + (tamaño_archivo[3] << 32);
            osfile->tamaño = tamaño_archivo return osfile
        }
    }
}

int os_read(osFile *file_desc, void *buffer, int nbytes)
{
    int tamaño = file_desc->tamaño;
    int bytes_r_w = file_desc->bytes_r_w;
    int bytes_leidos = 0;
    if (bytes_r_w + nbytes <= tamaño)
    {
        for (i = 0; i < nbytes; i++)
        {
            int numero_bloque_datos = bloque_de_datos(bytes_r_w);
            fseek(disco, file_desc->pos_indice + 5 + numero_bloque_datos * 3, SEEK_SET);
            unsinged char puntero[3];
            fread(puntero, sizeof(puntero), 1, disco);
            int pos_bloque_datos;
            pos_bloque_datos = puntero[0] + (puntero[1] << 8) + (puntero[2] << 16);

            int pos_byte = bytes_r_w - 2048 * numero_bloque_datos;
            fseek(disco, inicio_particion + pos_bloque_datos + pos_byte, SEEK_SET);
            fread(buffer[i], 1, 1, disco);
            bytes_r_w += 1;
            bytes_leidos++
        }
    }
    return bytes_leidos
}

int os_write(osFile *file_desc, void *buffer, int nbytes)
{
    int tamaño = file_desc->tamaño;
    int bytes_r_w = file_desc->bytes_r_w;
    int bytes_leidos = 0;
    if (bytes_r_w + nbytes <= tamaño)
    {
        for (i = 0; i < nbytes; i++)
        {
        }
    }
    return bytes_leidos
}

int os_close(osFile *file_desc)
{
}

// extya

int bloque_de_datos(int byte_r_w)
// cantidad de bitmaps requeridos para una particion de tamaño
{
    if (n_bloques % 2048 == 0)
    {
        return (n_bloques / 2048) - 1;
    }
    else
    {
        return (n_bloques / 2048);
    }
}