#pragma once

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
    // tamaño del archivo
    int tamaño;
    // pos de lectura o escritura dentro del archivo
    int bytes_r_w
};

osFile *os_open(char *filename, char name);
int os_read(osFile *file_desc, void *buffer, int nbytes);
int os_write(osFile *file_desc, void *buffer, int nbytes);
int os_close(osFile *file_desc);
