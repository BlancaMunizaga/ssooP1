#pragma once
#include "../general_func/general_func.h"

typedef struct crFILE {
  // 0 read | 1 write
  int mode;
  // pointer to current writing | reading pos
  int p_pos;
  // pointer to index block
  int p_index;
  // path of the file
  char *path;
} crFILE;

typedef struct dir_block {
  char entries[64][32];
} dir_block;

typedef struct bitmap_block {
  char entries[18432];
} bitmap_block;

typedef struct index_block {

  char size[4];
  char hLinks[4];
  char dataP[500][4];
  char indrP[10][4];

} index_block;

typedef struct data_block {

  char data[2048];

} data_block;

typedef struct ind_dir_block {

  char dataP[512][4];

} ind_dir_block;

int cr_write(crFILE *file_desc, void *buffer, int nbytes);

int search(int *begin, char name[]);

crFILE *cr_open(char *path, char mode); /* Jose   */

int cr_read(crFILE *file_desc, void *buffer, int nbytes); /* Jose   */

int cr_close(crFILE *file_desc); /* Daniel */

int cr_rm(char *path); /* Jose   */

int cr_hardlink(char *orig, char *dest); /* Henry  */

int cr_unload(char *orig, char *dest); /* Henry  */

int cr_load(char *orig);

int search_and_dir(int *begin, char name[]);

int min(int a, int b);
