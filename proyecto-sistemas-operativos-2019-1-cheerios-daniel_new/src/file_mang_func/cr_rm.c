#include "file_mang_func.h"
#include <stdio.h>
extern char path_disk[400];

void rm_data_block(int base, FILE *f) {
  // no se debe setear todo en 0, pero dejar el bloque referenciado como libre
}

void rm_index_block(int base, int *bloques, FILE *f) {
  unsigned char* pointer[4];
  int max = min(*bloques, 500);
  int block = 0;
  while ((*bloques > 0) & (block < max)) {
    fseek(f, base + 8 + block * 4, SEEK_SET);    // reset pointer
    fread(pointer, sizeof(unsigned char), 4, f); // posicion de bloque de datos
    rm_data_block((int) strtol((char*)pointer, NULL, 16), f);
    block++;
    (*bloques)--;
  }
}

void rm_indirect(int base, int *bloques, FILE *f) {
  unsigned char *pointer[4];
  int block = 0;
  while ((*bloques > 0) & (block < 10)) {
    fseek(f, base, SEEK_SET);
    fread(pointer, sizeof(unsigned char), 4, f); // posicion de bloque de datos
    rm_data_block((int) strtol((char*)pointer, NULL, 16), f);
    block++;
    (*bloques)--;
  }
}

int cr_rm(char *path) {
  char *path_copy = malloc(sizeof(path));
  strcpy(path_copy, path);
  if (!(path_copy[0] == '/')) {
    return 0;
  }

  char delim[] = "/";
  char *ptr = strtok(path_copy, delim);
  int byte_partida = 0;
  int dir;

  while (ptr != NULL) {
    dir = search_and_dir(&byte_partida, ptr);
    if (dir == -1) {
      printf("doesnt exist\n");
      return 0;
    }
    ptr = strtok(NULL, delim);
  }

  unsigned char *line = malloc(sizeof(unsigned char) * 32);
  FILE *f = fopen(path_disk, "rb");
  fseek(f, 32 * dir, SEEK_SET);
  fread(line, sizeof(unsigned char), 32, f);

  char *index_block = malloc(sizeof(char) * 2);
  for (int j = 30; j < 32; j++) {
    index_block[j - 1] = (char)line[j];
  } // find link to file

  fseek(f, 32 * dir, SEEK_SET);
  unsigned char invalid = 0x01; // link to file invalid
  fputc((int)invalid, f);       // write to file
  fseek(f, (int) strtol((char*)index_block, NULL, 16), SEEK_SET);
  char *hardlinks_c = malloc(sizeof(char) * 4);
  int hardlinks;
  char *size = malloc(sizeof(char) * 4);
  fread(line, sizeof(unsigned char), 32, f);
  for (int j = 0; j < 5; j++) {
    size[j - 4] = line[j];
  }
  for (int j = 5; j < 9; j++) {
    hardlinks_c[j - 4] = line[j];
  }
  int bloques = (int) strtol((char*)size, NULL, 16) / 2048;
  hardlinks = (int) strtol((char*)hardlinks_c, NULL, 16);
  hardlinks -= 1;
  if (hardlinks) { // si el archivo sigue existiendo
    sprintf(hardlinks_c, "%x", hardlinks);
    fseek(f, (int) strtol((char*)index_block, NULL, 16) + 4, SEEK_SET);
    fputc((int)hardlinks_c, f);
  } else { // borrar
    rm_index_block((int) strtol((char*)index_block, NULL, 16), &bloques, f);
    rm_indirect((int) strtol((char*)index_block, NULL, 16), &bloques, f);
  }
  return 1;
}
