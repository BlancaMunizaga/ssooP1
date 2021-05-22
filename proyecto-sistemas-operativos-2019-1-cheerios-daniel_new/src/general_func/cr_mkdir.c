#include "general_func.h"
#include <math.h>

extern char path_disk[400];

// Rellena un char con 0's a la izquierda
char* fill(char* num, int max) {
    if(strlen(num) < max) {
        char* result = malloc(9);
        strcpy(result, "");
        for(int i = 0; i < max - strlen(num); i++) {
            strcat(result, "0");
        }
        strcat(result, num);
        return result;
    }
    return num;
}

// Transforma un numero decimal a binario (en entero)
int toBinary(int decimal) {
    if (decimal < 2) {
        return decimal;
    }
    return toBinary(decimal / 2) * 10 + decimal % 2;
}

unsigned int get_index_mk(unsigned int dir, char *path, FILE* f) {
	unsigned char *buffer = malloc( sizeof( unsigned char ) * 32 );
	unsigned int index = 0;
	for(int i = 0; i < 64; i++ ) {
		fseek( f, 32 * i + dir * 2048, SEEK_SET );  // Itero sobre cada bloque
		fread( buffer, sizeof( unsigned char ), 32, f ); // Guardo en el buffer el contenido del bloque
		if ( buffer[0] != (unsigned char)1 ) {
			if (!strcmp(path, buffer + 1)) { // Si el nombre del elemento es igual, entonces obtengo su index
				index = (unsigned int)buffer[30] * 256 + (unsigned int)buffer[31]; 
			}
		}
	};
	free(buffer);
	return index;
}

// Obtiene la direccion de la primera entrada de directorio disponible
int get_dir(FILE* f, unsigned int num_dir) {
    unsigned char *buffer = malloc(sizeof(unsigned char) * 2048 * 4);
    for(unsigned int i = 0; i < 64; i++) {
        fseek(f, 32 * i + num_dir * 2048, SEEK_SET);
        fread(buffer, 1, 32, f);
        if (buffer[0] != (unsigned char)2 && buffer[0] != (unsigned char)4) {
            free(buffer);
            return i;
        }
    }
    free(buffer);
    return -1;
}

unsigned int get_free_index(FILE* f, int modify) {
    fseek(f, 2048, SEEK_SET);
    unsigned char *buffer = malloc(sizeof(unsigned char) * 2048 * 4);
    char* num = malloc(9);
    fread(buffer, 1, 2048 * 4, f);
    for (int i = 0; i < 2048 * 4; i++) {
        snprintf(num, 9, "%d", toBinary((int) buffer[i]));
        num = fill(num, 8);
        //printf("%s\n", num); // Hasta aqui esto es un cr_bitmap

        // Si encuentro un numero igual a 0 (libre) retorno
        for(int j = 0; j < 8; j++) {
            if(num[j] == '0') {
                unsigned char new_value = buffer[i] + (unsigned char) pow(2, 7 - j);
                if (modify) {
                    fseek(f, 2048 + i, SEEK_SET);
                    fwrite(&new_value, 1, 1, f);
                }
                free(buffer);
                free(num);
                return i * 8 + j;
            }
        }
    }
    free(num);
    free(buffer);
    return 0;
}

unsigned char *toChar(unsigned int num) {
    unsigned char *result = malloc(sizeof(unsigned char) * 4);
    unsigned int nnum = num;
    unsigned int bbbig = nnum / pow(2, 24);
    nnum = nnum % (unsigned int) pow(2, 24);
    unsigned int bbig = nnum / pow(2, 16);
    nnum = nnum % (unsigned int) pow(2, 16);
    unsigned int big = nnum / pow(2, 8);
    nnum = nnum % (unsigned int) pow(2, 8);
    unsigned int little = nnum % 256;
    result[0] = (unsigned char) bbbig;
    result[1] = (unsigned char) bbig;
    result[2] = (unsigned char) big;
    result[3] = (unsigned char) little;
    return result;
}

int cr_mkdir(char* foldername) {
    FILE* f = fopen(path_disk, "r+");
    // Variable para guardar el indice del directorio en el que se creara el nuevo
    unsigned int ind_dir = 0;
    // Hare una especie de split del foldername, ya que este puede estar dentro de otro
    char *dir = malloc(strlen(foldername));
    strcpy(dir, foldername);
    char delim[] = "/";
    char *tok = strtok(dir, delim);
    unsigned int index = 0;
    
    while(tok != NULL)
    {
        // Aqui tok corresponde al directorio actual. Debo buscar su index
        index = get_index_mk(index, tok, f);
        // Si index es 0, no existe el directorio
        if (index == 0) {
            // Quiza deberia mostrar un error aqui
            break;
        }
        ind_dir = index;
        tok = strtok(NULL, delim);
    }

    // Variables de aqui en adelante
    // tok -> nombre de la carpeta
    // ind_dir -> numero de bloque en que se encuentra el directorio "padre"
    // ind_new -> numero de bloque en que se encontrara el nuevo directorio
    // num_dir -> posicion en que se encontrara el ind_new en el directorio "padre"

    unsigned int ind_new = get_free_index(f, 1); // Aqui ademas se reserva el bloque en el bitmap
    unsigned int num_dir = get_dir(f, ind_dir);
    if (num_dir == -1) {
        return 0;
    }

    // Ahora debo escribir en el disco la entrada de directorio

    unsigned char tipo = 2;
    unsigned char* nrobloque = toChar(ind_new);
    fseek(f, 32 * num_dir + ind_dir * 2048, SEEK_SET);
    fwrite(&tipo, 1, 1, f);
    fwrite(tok, 1, 27, f);
    fwrite(nrobloque, 1, 4, f);

    // Ahora debo invalidar todas las entradas del bloque de directorio
    tipo = 1;
    for (int i = 0; i < 64; i++) {
        fseek(f, 32 * i + ind_new * 2048, SEEK_SET);
        fwrite(&tipo, 1, 1, f);
    }
    
    free(nrobloque);
    free(dir);
    fclose(f);
    return ind_new;
}