#include <stdio.h>
#include <stdlib.h>
/* General */

char *file_name;

FILE *disco;

int id_particion;

unsigned char id_absoluto_particion[3];

unsigned char cantidad_bloques_particion[4];

int id_abs;

int cantidad_bloques;

unsigned char buffer_entrada[1];

int inicio_particion;

int cantidad_bloques_bitmap;

int is_valid(int primer_byte_entrada);

int cantidad_de_bitmaps(int n_bloques);

void os_mount(char *diskname, int partition);

void os_bitmap(unsigned num);

int os_exists(char *filename);

void os_ls();

/* Funciones Master Boot */
void os_mbt();

void os_create_partition(int id, int size);

void os_delete_partition(int id);

void os_reset_mbt();

int os_rm(char *filename);