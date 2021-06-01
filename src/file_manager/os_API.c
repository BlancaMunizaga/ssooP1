#include "os_API.h"
#include "lista_particiones.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


/*FUNCIONES GENERALES*/
/* 
Funcion para montar el disco en una partición en especifico. 
Establece como variable global la ruta local donde se encuentra el archivo .bin 
correspondiente al disco y tambien guarda como variable global la partición a montar
*/

void os_mount(char *diskname, int partition)
{
    file_name = diskname;
    disco = fopen(diskname, "rb+");
    if(disco == NULL){
        printf("File did not open!\n");
    }
        

    id_particion = partition;
    bool found = false;
    for (int id = 0; id < 128; id++)
    {
        fseek(disco, 8 * id, SEEK_SET); // Primer byte de la entrada validez+id_puntero
        fread(buffer_entrada, sizeof(buffer_entrada), 1, disco);
        int primer_byte = buffer_entrada[0];
        if (primer_byte - 128 == partition)
        {
            // particion encontrada
            // Leyendo 3 bytes para encontrar el id_absoluto_particion y se guarda en la variable global
            fread(id_absoluto_particion, sizeof(id_absoluto_particion), 1, disco);
            // Leyendo 4 bytes para encontrar la cantidad de bloques de la particion y se guarda en variable global
            fread(cantidad_bloques_particion, sizeof(cantidad_bloques_particion), 1, disco);
            // Leyendo el primer bloque directorio, que es el inicio de la particion
            id_abs = id_absoluto_particion[2] + (id_absoluto_particion[1] << 8) + (id_absoluto_particion[0] << 16);
            cantidad_bloques = cantidad_bloques_particion[3] + (cantidad_bloques_particion[2] << 8) + (cantidad_bloques_particion[1] << 16) + (cantidad_bloques_particion[0] << 24);
            inicio_particion = id_abs*2048 + 1024;
            // Leyendo cantidad de bloques bitmap y guardando en variable global
            cantidad_bloques_bitmap = cantidad_de_bitmaps(cantidad_bloques);

            printf("Id abs: %i\n", id_abs);
            printf("cantidad bloques: %i\n", cantidad_bloques);
            printf("Inicio partición: %i\n", inicio_particion);
            printf("Cantidad de bloques bitmap: %i\n", cantidad_bloques_bitmap);
            found = true;
            particion_valida = 1;
            break;

        }
    }
    if (!found){
        printf("La particion no es valida.\n");
        particion_valida = 0;
    }
    // Obtener el path y guardar la ruta al archivo en variable global file_namei
    // getcwd(file_name, sizeof(file_name));
    // strcat(file_name, "/");
    // strcat(file_name, diskname);
}

void os_bitmap(unsigned num)
{
    printf("os_bitmap\n");
    // Avanzar 1 KB para saltar la MBT + el id_absoluto (inicio_particion)
    // + 2KB = primer bloque bitmap
    // fseek(disco, inicio_particion + 2048, SEEK_SET);
    // for (int bloque; bloque < cantidad_bloques_bitmap; bloque++)

    int libres = 0;
    int ocupados = 0;

    //int bitmap;
    unsigned char buffer_byte[1];
    if (num != 0 && num <= cantidad_bloques_bitmap)
    {
        //avazamos hasta el incio de la partición y luego avanzamos hacia el incio del bloque de bitmap
        fseek(disco, inicio_particion + (num * 2048), SEEK_SET);
        for (int byte = 0; byte < 2048; byte++)
        {
            fread(buffer_byte, sizeof(buffer_byte), 1, disco);
            

            // fprintf(stderr, "%x", (int) buffer_byte[0]);
            fprintf(stderr, "0x%02X\n", (unsigned int) buffer_byte[0] & 0xFF);

            if (byte != 0 && byte % 4 == 0)
            {
                fprintf(stderr, "\n");
            }
            unsigned char aux_buffer = buffer_byte[0];
            int aux2_buffer = aux_buffer;
            unsigned char mask = 0x7F;
            for (int contador_shift = 7; contador_shift > 0; contador_shift--)
            {
                
                int bit;
                bit = aux2_buffer >> contador_shift;
                if (bit == 1)
                {
                    // 10010000
                    // 144 - 126
                    //shift 00000001
                    // int 1
                    // 0x7F = 01111111
                    
                    // 01111111, 110000000, 11100000
                    // 00010000
                    ocupados++; 
                }
                else
                {
                    libres++;
                }
                aux2_buffer = aux2_buffer & mask;
                mask = mask >> 1;
            }
        }
        fprintf(stderr, "\n");
        fprintf(stderr, "bloques ocupados: %i \n", ocupados);
        fprintf(stderr, "bloques libres: %i \n", libres);
    }
    else if (num == 0)
    {
        fseek(disco, inicio_particion + 2048, SEEK_SET);
        for (int byte = 0; byte < 2048 * cantidad_bloques_bitmap; byte++)
        {
            fread(buffer_byte, sizeof(buffer_byte), 1, disco);
            fprintf(stderr, "%x", (int) buffer_byte[0]);
            if (byte != 0 && byte % 4 == 0)
            {
                fprintf(stderr, "\n");
            }
            unsigned char aux_buffer = buffer_byte[0];
            int aux2_buffer = aux_buffer;
            unsigned char mask = 0x7F;
            for (int contador_shift = 7; contador_shift > 0; contador_shift--)
            {
                int bit;
                bit = aux2_buffer >> contador_shift;
                if (bit == 1)
                {
                    ocupados++;  
                }
                else
                {
                    libres++;
                }
                aux2_buffer = aux2_buffer & mask;
                mask = mask >> 1;
            }
        }
        fprintf(stderr, "\n");
        fprintf(stderr, "bloques ocupados: %i \n", ocupados);
        fprintf(stderr, "bloques libres: %i \n", libres);
    }
    else
    {
        printf("El bloque bitmap ingresado es invalido");
    }
}

int os_exists(char *filename)
{
    for (int entrada = 0; entrada < 64; entrada++) // Se recorre cada entrada del bloque directorio.
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
                return 1; // Si son iguales se retorna 1
            }
        }
    }
    return 0; //  si se recorre todo el directorio sin encontrar el archivo, se retorna 0
}

void os_ls()
{
    printf("Archivos particion %i: \n", id_particion);
    for (int entrada = 0; entrada < 64; entrada++) // Se recorre cada entrada del bloque directorio.
    {
        fseek(disco, inicio_particion + (32 * entrada), SEEK_SET);
        
        unsigned char byte_validez[1];
        fread(byte_validez, sizeof(byte_validez), 1, disco);
        
        int validez = byte_validez[0];
        
        if (validez) // Si la entrada es valida.
        {
            unsigned char id_relativo_bloque_indice[3];
            unsigned char nombre_archivo[28];

            fread(id_relativo_bloque_indice, sizeof(id_relativo_bloque_indice), 1, disco);
            fread(nombre_archivo, sizeof(nombre_archivo), 1, disco);

            printf("Nombre archivo: %s\n", nombre_archivo); // Se muestra en consola el nombre del archivo.
        }
    }
}

/*FUNCIONES MASTER BOOT*/

void os_mbt()
{

    printf("os_mbt\n");
    // Recorrer todas las particiones
    for (int id = 0; id < 128; id++)
    {
        fseek(disco, 8 * id, SEEK_SET); // Primer byte de la entrada validez+id_puntero
        fread(buffer_entrada, sizeof(buffer_entrada), 1, disco);
        int primer_byte = buffer_entrada[0];
        if (primer_byte < 128)
        {
            // no valida
        }
        else
        {
            // sacar info de particion.
            fread(id_absoluto_particion, sizeof(id_absoluto_particion), 1, disco);
            fread(cantidad_bloques_particion, sizeof(cantidad_bloques_particion), 1, disco);
            int pid = primer_byte - 128;
            // Transformar los 3-4 bytes a int (Puede estar mal, hay que probarlo)
            id_abs = id_absoluto_particion[2] + (id_absoluto_particion[1] << 8) + (id_absoluto_particion[0] << 16);
            cantidad_bloques = cantidad_bloques_particion[3] + (cantidad_bloques_particion[2] << 8) + (cantidad_bloques_particion[1] << 16) + (cantidad_bloques_particion[0] << 24);

            // mostrar info de particion en consola.
            printf("id: %i,  id absoluto: %i,  cantidad de bloques: %i\n", pid, id_abs, cantidad_bloques);
        }
    }
}

void os_create_partition(int id, int size)
{
    printf("os_create_partition\n");
    // Recorrer todas las particiones
    Lista * lista_particiones = lista_init(); 
    int pos_libre_mbt = -1;
    for (int i = 0; i < 128; i++)
    {
        fseek(disco, 8 * i, SEEK_SET); // Primer byte de la entrada validez+id_puntero
        fread(buffer_entrada, sizeof(buffer_entrada), 1, disco);
        int primer_byte = buffer_entrada[0];
        if (primer_byte < 128)
        {
            // no valida
            if(pos_libre_mbt >= 0){
                pos_libre_mbt =  8 * i;
            }
        }
        else
        {
            // sacar info de particion.
            fread(id_absoluto_particion, sizeof(id_absoluto_particion), 1, disco);
            fread(cantidad_bloques_particion, sizeof(cantidad_bloques_particion), 1, disco);
            int pid = primer_byte - 128;
            // Transformar los 3-4 bytes a int (Puede estar mal, hay que probarlo)
            id_abs = id_absoluto_particion[2] + (id_absoluto_particion[1] << 8) + (id_absoluto_particion[0] << 16);
            cantidad_bloques = cantidad_bloques_particion[3] + (cantidad_bloques_particion[2] << 8) + (cantidad_bloques_particion[1] << 16) + (cantidad_bloques_particion[0] << 24);

            // mostrar info de particion en consola.
            Particion * partition = process_init(pid, id_abs, cantidad_bloques);
            sortedInsert(lista_particiones, partition);
        }
    }
    Particion * partition = process_init(0, 2097152, 0); // final del disco
    sortedInsert(lista_particiones, partition);
    id += 128;
    Particion * current = lista_particiones -> head;
    int pos = 0;
    bool seguir = true;
    while (current != NULL && seguir)
    {
        if(current->id_abs - pos >= size){
            seguir = false;
        
            unsigned char bytes[8];
            bytes[0] = (unsigned char) id; // Bit validez y pid
            // 3 Bytes de id_absoluto
            bytes[3] = pos & 0xff; 
            bytes[2] = (pos >> 8) & 0xff;
            bytes[1] = (pos >> 16) & 0xff;

            bytes[7] = size & 0xff; 
            bytes[6] = (size >> 8) & 0xff;
            bytes[5] = (size >> 16) & 0xff;
            bytes[4] = (size >> 24) & 0xff;

            fclose(disco);
            disco = fopen(file_name, "wb+");
            fseek(disco, pos_libre_mbt, SEEK_SET);

            for(int i=0; i < 8; i++){
                printf("byte: %i", (int) bytes[i]);
                printf("bite que se esta escribiendo: %i", i);
                fputc((int) bytes[i], disco);

            }
            fclose(disco);
            disco = fopen(file_name, "r+b");
        }
        pos = current->id_abs + current -> cantida_de_bloques + 1;
        current = current->next;
    }
    if(seguir){
        printf("SEGMENTATION FAULT\n");
    }
}

void os_delete_partition(int id)
{
    fseek(disco, 8 * id, SEEK_SET); // Primer byte de la entrada validez+id_puntero
    fread(buffer_entrada, sizeof(buffer_entrada), 1, disco);
    int primer_byte = buffer_entrada[0];
    if (primer_byte < 127)
    {
        printf("partición no valida\n");
    }
    else
    {
        fseek(disco, -1, SEEK_CUR);
        primer_byte -= 128;
        fputc(primer_byte, disco);
    }
}

void os_reset_mbt()
{
    printf("os_reset_mbt");
    /* recorremos todas las entradas y llamamos a delete_partition por cada una*/
    for (int id = 0; id < 128; id++)
    {
        os_delete_partition(id);
    }
}

int os_rm(char *filename)
{
    for (int entrada = 0; entrada < 64; entrada++) // Se recorre cada entrada del bloque directorio.
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
                fseek(disco, inicio_particion + 32 * entrada, SEEK_SET);
                fputc(0, disco); // se cambia el bit de validez a 0.
                return 1;
            }
        }
    }
    return 0; //  si se recorre todo el directorio sin encontrar el archivo, se retorna 0
}

// extra

int is_valid(int primer_byte_entrada)
{
    return primer_byte_entrada > 127;
}

int cantidad_de_bitmaps(int n_bloques)
// cantidad de bitmaps requeridos para una particion de tamaño
{
    if (n_bloques % 16384 == 0)
    {
        return (n_bloques / 16384);
    }
    else
    {
        return (n_bloques / 16384) + 1;
    }
}

int primera_pos_vacia_mbt(){
    return 0;
}