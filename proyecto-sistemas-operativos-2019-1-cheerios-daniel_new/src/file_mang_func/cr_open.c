#include "file_mang_func.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

crFILE* cr_open(char* path, char mode){
  int exists = cr_exists(path);

  crFILE* foo = malloc(sizeof(crFILE));
  foo->p_index = 0;
  foo->p_pos = 0;
  foo->path = "";
  if (mode == 'r'){
    foo->mode = 0;
    if (exists){
      foo->path = malloc(sizeof(path) + 1);
      strcpy(foo->path, path);
    } else {
      printf("ERROR: Archivo no existe.\n");
      free(foo);
      return NULL;
    }
  }
  else if (mode == 'w'){
    foo->mode = 1;
    if (exists){
      printf("ERROR: Archivo con ese nombre ya existe.\n");
      free(foo);
      return NULL;
    } else {
      foo->path = malloc(sizeof(path) + 1);
      strcpy(foo->path, path);
    }
  }
  return foo;
}
