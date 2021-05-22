#pragma once

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

void cr_mount(char* diskname);   /* Dunkan */

void cr_bitmap();                /* Dunkan */

void print_binary(int from, int to);

int cr_exists(char *path);       /* Dunkan */

void cr_ls(char *path);          /* Daniel */

int cr_mkdir(char *foldername);  /* Henry  */
