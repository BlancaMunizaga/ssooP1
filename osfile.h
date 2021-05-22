#pragma once

// Declaramos el struct para el process
struct osfile;
/* struct process*/
typedef struct osfile osFile;
struct osfile
{
    char *filename;
    int file_mode;
};

osFile *os_open(char *filename, char name);
int os_read(osFile *file_desc, void *buffer, int nbytes);
int os_write(osFile *file_desc, void *buffer, int nbytes);
int os_close(osFile *file_desc);
int os_rm(char *filename);