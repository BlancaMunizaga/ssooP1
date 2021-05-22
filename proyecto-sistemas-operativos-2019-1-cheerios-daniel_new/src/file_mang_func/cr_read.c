#include "file_mang_func.h"
#include <stdio.h>
#include <string.h>
extern char path_disk[400];

int min(int a, int b) {
  if (a < b) {
    return a;
  }
  return b;
}

void read_data_block(void *buffer, int base, int amount, FILE *f) {
  unsigned char block_buffer[2048];
  fseek(f, base, SEEK_SET);
  fread(block_buffer, sizeof(unsigned char), amount, f);
  strcat(buffer, (char*) block_buffer);
}

void read_index_block(void *buffer, int base, int *bloques, int nbytes,
                      FILE *f) {
  unsigned char *pointer[4];
  int max = min(*bloques, 500);
  int block = 0;
  while ((*bloques > 0) & (block < max)) {
    fseek(f, base + 8 + block * 4, SEEK_SET);    // reset pointer
    fread(pointer, sizeof(unsigned char), 4, f); // posicion de bloque de datos
    int amount =
        min(2048,
            nbytes - block * 2048); // para saber cuanto leer dentro del bloque
    read_data_block(buffer, (int)strtol((char*)pointer, NULL, 16), amount, f);
    block++;
    (*bloques)--;
  }
}

void read_indirect(void *buffer, int base, int *bloques, int nbytes, FILE *f) {
  unsigned char *pointer[4];
  int max = min(*bloques, 10);
  int block = 0;
  while ((*bloques > 0) & (block < max)) {
    fseek(f, base, SEEK_SET);
    fread(pointer, sizeof(unsigned char), 4, f); // posicion de bloque de datos
    int amount =
        min(2048,
            nbytes - block * 2048); // para saber cuanto leer dentro del bloque
    read_data_block(buffer, (int)strtol((char*)pointer, NULL, 16), amount, f);
    block++;
    (*bloques)--;
  }
}

int search_and_dir(int *begin, char name[]) {
  // actualiza el begin al byte del prox dir a revisar
  // leer de a cada entrada de dir = 32 bytes
  // printf("%d %s\n", *begin, name);
  FILE *f = fopen(path_disk, "rb");
  unsigned char *buffer = malloc(sizeof(unsigned char) * 32);
  // cada char son 8 bits = 1 byte
  // buffer de 32 Byte
  unsigned char invalid = 0x01;
  unsigned char valid_dir = 0x02;
  unsigned char valid_file = 0x04;
  int index = 1;

  (*begin) = (*begin) * 64; // begin es el bloque
  for (int i = (*begin); i < (*begin) + 64; i++) {
    fseek(f, 32 * i, SEEK_SET);
    fread(buffer, sizeof(unsigned char), 32, f);

    uint8_t validation = (uint8_t)buffer[0];

    if (validation == invalid) {
      printf("%d Invalid\n", index);
    }

    else if (validation == valid_dir || validation == valid_file) {
      // printf("%d valid_dir or valid_file\n", index);
      char *linea = malloc(sizeof(char) * 27);
      for (int j = 1; j < 28; j++) {
        // name
        linea[j - 1] = (char)buffer[j];
      }
      // printf("Nombre: %s\n", linea);
      if (strcmp(linea, name) == 0) {

        uint8_t b = (uint8_t)buffer[31];
        uint16_t index = b | (uint16_t)buffer[30] << 8;

        // printf("Es: %u\n", (unsigned int)index);
        fclose(f);
        free(buffer);
        *begin = index;
        return i;
      }
    }
    index++;
  }

  fclose(f);
  free(buffer);
  return -1;
}

int cr_read(crFILE *file_desc, void *buffer, int nbytes) {
  char *path = malloc(sizeof(file_desc->path));
  strcpy(path, file_desc->path);
  if (!(path[0] == '/')) {
    // si no parte del raiz no existe
    // porque el path esta mal escrito
    return 0;
  }

  char delim[] = "/";
  char *ptr = strtok(path, delim);
  int byte_partida = 0;
  int dir;

  while (ptr != NULL) {
    // printf("%s\n", ptr);
    // ptr es cada dir y file
    dir = search_and_dir(&byte_partida, ptr);
    if (dir == -1) {
      // si la resp es 0 -> algun dir
      // no existia o el archivo mismo ->
      // no existe
      printf("doesnt exist\n");
      return 0;
    }
    ptr = strtok(NULL, delim);
    // printf("\n");
  }

  unsigned char *line = malloc(sizeof(unsigned char) * 32);
  // printf("dir is: %d\n", dir);

  FILE *f = fopen(path_disk, "rb");
  fseek(f, 32 * dir, SEEK_SET);
  fread(line, sizeof(unsigned char), 32, f);
  // find index block of file
  char *index_block = malloc(sizeof(char) * 2);
  for (int j = 30; j < 32; j++) {
    // index_block
    index_block[j - 1] = (char)line[j];
  }

  fseek(f, (int)strtol((char*)index_block, NULL, 16), SEEK_SET);
  char size[4];
  fread(line, sizeof(unsigned char), 32, f);

  for (int j = 0; j < 5; j++) {
    size[j] = line[j]; // tamaño del archivo
  }

  int bloques = min(strtol((char*)size, NULL, 16), nbytes) / 2048; // cuanta memoria ver
  read_index_block(buffer, (int)strtol((char*)index_block, NULL, 16), &bloques, nbytes, f);
  int indirect = (int)strtol((char*)index_block, NULL, 16) + 500 * 4 + 8;

  read_indirect(buffer, indirect, &bloques, nbytes, f);

  fclose(f);
  return 1;
}
