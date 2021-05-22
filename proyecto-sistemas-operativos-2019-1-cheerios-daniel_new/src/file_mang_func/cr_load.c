#include "file_mang_func.h"
#include <math.h>
#include <dirent.h>

extern char path_disk[400];

#define PATH_MAX 4096

char *getName(char* orig) {
    char *dir = malloc(strlen(orig));
    char *result = malloc(27 * sizeof(char));
    strcpy(dir, orig);
    char delim[] = "/";
    char *tok = strtok(dir, delim);
    
    while(tok != NULL)
    {
        strcpy(result, tok);
        tok = strtok(NULL, delim);
    }
    free(dir);
    return result;
}

// Funcion poco elegante para transformar un int a un arreglo de char
unsigned char *toChar_load(unsigned int num) {
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

// Obtiene la direccion de la primera entrada de directorio disponible
int get_dir_load(FILE* f, unsigned int num_dir) {
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

// Rellena un char con 0's a la izquierda
char* fill_load(char* num, int max) {
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
int toBinary_load(int decimal) {
    if (decimal < 2) {
        return decimal;
    }
    return toBinary_load(decimal / 2) * 10 + decimal % 2;
}

unsigned int get_index_load(unsigned int dir, char *path, FILE* f) {
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

// Obtiene el indice del primer bloque libre, esto lo hace revisando el
// el bloque de bitmap. Si modify es distinto de 0 (true) entonces
// modifica el bitmap cambiando a 1 el bloque correspondiente
// Si no queda espacio retorna 0
unsigned int get_free_index_load(FILE* f, int modify) {
    fseek(f, 2048, SEEK_SET);
    unsigned char *buffer = malloc(sizeof(unsigned char) * 2048 * 4);
    char* num = malloc(9);
    fread(buffer, 1, 2048 * 4, f);
    for (int i = 0; i < 2048 * 4; i++) {
        snprintf(num, 9, "%d", toBinary_load((int) buffer[i]));
        num = fill_load(num, 8);

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

int load_file(FILE* f, char* orig, char* name, unsigned int num_dir) {
    FILE* in = fopen(orig, "r+");
    fseek(in, 0, SEEK_END);
    unsigned int size_f = ftell(in);
    fseek(in, 0, SEEK_SET);
    unsigned int indice = get_free_index_load(f, 1);
    if (!indice) {
        fclose(in);
        printf("No hay espacio suficiente\n");
        return -1;
        // Falta espacio
    }
    unsigned int hl = 0;
    
    // Crear bloque indice
    unsigned char *size = toChar_load(size_f);
    unsigned char *hlink = toChar_load(hl);
    fseek(f, 2048 * indice, SEEK_SET);
    fwrite(size, 1, 4, f);
    fwrite(hlink, 1, 4, f);
    // Crear bloques de datos
    unsigned int index;
    unsigned char *buffer = malloc(sizeof(unsigned char) * 2048 * 4);
    unsigned char *ptr;
    unsigned int writed = 0;
    for (unsigned int i = 0; i * 2048 < size_f; i++) {  // Por cada bloque de info del archivo a copiar
        index = get_free_index_load(f, 1);                   // Obtengo el primer indice libre
        if (!index) {
            // Falta espacio
            break;
        }
        ptr = toChar_load(index);                            // Lo transformo a un arreglo de char
        fseek(f, 2048 * indice + 8 + i * 4, SEEK_SET);  // Posiciono el puntero del disco en el siguiente espacio para punteros
        fwrite(ptr, 1, 4, f);                           // Guardo el indice
        free(ptr);
        fseek(f, index * 2048, SEEK_SET);               // Posiciono el puntero en el bloque libre
        if (writed + 2048 > size_f) {                  // Si queda menos de 2048 bytes por copiar, copia lo necesario
            unsigned int last_size = size_f - writed;
            fread(buffer, 1, last_size, in);
            fwrite(buffer, 1, last_size, f);
            writed += last_size;
        } else {                                        // Sino, copia el bloque completo
            fread(buffer, 1, 2048, in);
            fwrite(buffer, 1, 2048, f);
            writed += 2048;
            if (i == 499) {                             // Si se completaron los 500 punteros, sal del ciclo
                break;
            }
        }
    }
    if (writed < size_f) {                              // Si falto por guardar entonces ocupo los bloques de dir indirecto
        unsigned int index_ind;
        for (int i = 0; i < 10; i++) {
            //printf("Bloque indirecto nro %d en %u\n", i, 2048 * indice + 2008 + i * 4);
            index_ind = get_free_index_load(f, 1);               // Obtengo el indice para el bloque de direccionamiento indirecto
            if (!index_ind) {
                // Falta espacio
                printf("No hay espacio suficiente\n");
                break;
            } 
            ptr = toChar_load(index_ind);
            //printf("%u, %u, %u, %u = %u", ptr[0], ptr[1], ptr[2], ptr[3], index_ind);
            fseek(f, 2048 * indice + 2008 + i * 4, SEEK_SET);
            fwrite(ptr, 1, 4, f);                       // Y lo guardo
            free(ptr);
            // Ahora repito el mismo proceso de antes
            for (unsigned int j = 0; j < 512; j++) {
                index = get_free_index_load(f, 1);                   // Obtengo el primer indice libre
                if (!index) {
                    // Falta espacio
                    break;
                }
                ptr = toChar_load(index);                            // Lo transformo a un arreglo de char
                fseek(f, 2048 * index_ind + j * 4, SEEK_SET);   // Posiciono el puntero del disco en el siguiente espacio para punteros
                fwrite(ptr, 1, 4, f);                           // Guardo el indice
                free(ptr);
                fseek(f, index * 2048, SEEK_SET);               // Posiciono el puntero en el bloque libre
                if (writed + 2048 > size_f) {                  // Si queda menos de 2048 bytes por copiar, copia lo necesario
                    unsigned int last_size = size_f - writed;
                    fread(buffer, 1, last_size, in);
                    fwrite(buffer, 1, last_size, f);
                    writed += last_size;
                    break;
                } else {                                        // Sino, copia el bloque completo
                    fread(buffer, 1, 2048, in);
                    fwrite(buffer, 1, 2048, f);
                    writed += 2048;
                }
            }
            if (writed >= size_f) {
                break;
            }
        }
    }
    // Agrego la entrada de directorio
    unsigned int dir = get_dir_load(f, num_dir);
    unsigned char tipo = 4; 
    unsigned char* nrobloque = toChar_load(indice);
    fseek(f, 32 * dir + num_dir * 2048, SEEK_SET);
    fwrite(&tipo, 1, 1, f);
    fwrite(name, 1, 27, f);
    fwrite(nrobloque, 1, 4, f);

    free(buffer);
    free(nrobloque);
    free(size);
    free(hlink);
    fclose(in);
    return 0;
}

int create_folder(FILE* f, char* foldername) {
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
        index = get_index_load(index, tok, f);
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

    unsigned int ind_new = get_free_index_load(f, 1); // Aqui ademas se reserva el bloque en el bitmap
    unsigned int num_dir = get_dir_load(f, ind_dir);
    if (num_dir == -1) {
        return 0;
    }

    // Ahora debo escribir en el disco la entrada de directorio

    unsigned char tipo = 2;
    unsigned char* nrobloque = toChar_load(ind_new);
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
    return ind_new;
}

int loader(char* disk, char* orig, int bloque_directorio) {
    char *name = getName(orig);
    DIR *d;
    struct dirent *dir;
    d = opendir(orig);
    size_t size = PATH_MAX;

    // Backtracking: Abro la ruta. Si es carpeta (nodo del arbol), creo la correspondiente
    // en el disco y reviso uno por uno los elementos dentro de ella. Por cada una de ellas
    // vuelvo a llamar a la funcion ahora con la ruta hacia ese elemento en especifico
    // En el otro caso (hojas del arbol), es un archivo, por tanto lo cargo en la direccion
    // de su carpeta contenedora

    if (d) {
        FILE* f = fopen(disk, "r+");
        int bloque = create_folder(f, orig);
        fclose(f);
        char *dname = malloc(255);
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(dir -> d_name, ".") && strcmp(dir -> d_name, "..")) {
                strncpy(dname, dir->d_name, 254);
                dname[254] = '\0';
                char path[size];
                strcpy(path, orig);
                strcat(path, "/");
                strcat(path, dname);
                loader(disk, path, bloque);
            }
        }
        closedir(d);
    } else {
        FILE* f = fopen(disk, "r+");
        load_file(f, orig, name, bloque_directorio);
        fclose(f);
    }
    free(name);
    return 0;
}

int cr_load(char* orig) {
    // Hare todo en una nueva funcion que servira de recursion
    // asi ademas evito abrir muchas veces el disco
    loader(path_disk, orig, 0);
    return 0;
}