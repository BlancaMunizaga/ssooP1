#pragma once

typedef enum os_error {
    OS_SEGFAULT,
    OS_FILENOTFOUND,

} OS_ERROR;


unsigned char tamano_archivo[5];

// Declaramos el struct para el process
struct osfile;
/* struct process*/
typedef struct osfile osFile;
struct osfile
{

    // 0 read | 1 write
    int mode;
    // pos absoluta del bloque indice
    int pos_indice;
    // tamaño del archivo en bytes
    int tamano;
    // pos de lectura o escritura dentro del archivo en bytes
    int bytes_r_w;
    // bloque de datos de lectura o escritura dentro del archivo en bytes
    int bloque_de_datos;
};

int bloque_de_datos(int byte_r_w);
osFile *os_open(char *filename, char name);
int os_read(osFile *file_desc, unsigned char  *buffer, int nbytes);
int os_write(osFile *file_desc, unsigned char *buffer, int nbytes);
int os_close(osFile *file_desc);
