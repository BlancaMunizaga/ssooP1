#include "general_func.h"

extern char path_disk[400];

/*
Funcion para ver si un archivo o carpeta existe en la ruta especificada por
path. Retorna 1 si el archivo o carpeta existe y 0 en caso contrario.
*/
const char *bit_rep2[16] = {
    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

void print_byte2(uint8_t byte){
    // printf("%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
    printf("%s%s", bit_rep2[byte >> 4], bit_rep2[byte & 0x0F]);
}


char* str_byte(uint8_t byte1, uint8_t byte2){
    char* linea = malloc(sizeof(char)* 16);
    sprintf(linea,"%s%s%s%s", bit_rep2[byte1 >> 4], bit_rep2[byte1 & 0x0F], bit_rep2[byte2 >> 4], bit_rep2[byte2 & 0x0F]);

    return linea;
}

// Algunas direcciones de simdiskformat.bin
// /thanos/segfault.jpg
// /thanos/thanos.gif
// /free.jpg
// /memes/t2.png

int search(int* begin, char name[]){
    // actualiza el begin al byte del prox dir a revisar
    // leer de a cada entrada de dir = 32 bytes
    // printf("%d %s\n", *begin, name);
    FILE* f = fopen(path_disk, "rb" );
	unsigned char *buffer = malloc( sizeof(unsigned char) * 32 );
    // cada char son 8 bits = 1 byte
    // buffer de 32 Byte
    unsigned char invalid    = 0x01;
    unsigned char valid_dir  = 0x02;
    unsigned char valid_file = 0x04;
    int index = 1;

    (*begin) = (*begin)*64; // begin es el bloque
	for( int i = (*begin); i < (*begin)+64; i++ ) {
		fseek( f, 32 * i, SEEK_SET );
		fread( buffer, sizeof( unsigned char ), 32, f );

        uint8_t validation = (uint8_t) buffer[0];

        if (validation==invalid){
            // printf("%d Invalid\n", index);
        }

        else if (validation==valid_dir || validation==valid_file){
            // printf("%d valid_dir or valid_file\n", index);
            char* linea = malloc(sizeof(char)*27);
            for (int j = 1; j < 28; j++){
                // name
                linea[j-1] = (char) buffer[j];
            }
            // printf("Nombre: %s\n", linea);
            if (strcmp(linea, name) == 0){

                uint8_t b = (uint8_t) buffer[31];
                uint16_t index = b | (uint16_t)buffer[30] << 8;

                // printf("Es: %u\n", (unsigned int)index);
                fclose(f);
                free(buffer);
                *begin = index;
                return 1;
            }

        }
        index++;
    }

    fclose(f);
    free(buffer);
    return 0;
}

// Esta siendo probado con  -> /thanos/thanos.gif
int cr_exists(char* path){
    if (!(path[0] == '/')){
        // si no parte del raiz no existe
        // porque el path esta mal escrito
        return 0;
    }

	char delim[] = "/";
	char *ptr = strtok(path, delim);
    int byte_partida = 0;
    int resp;

	while(ptr != NULL){
		// printf("%s\n", ptr);
        // ptr es cada dir y file
        resp = search(&byte_partida, ptr);
        if (!resp){
            // si la resp es 0 -> algun dir
            // no existia o el archivo mismo ->
            // no existe
            return 0;
        }
		ptr = strtok(NULL, delim);
        // printf("\n");
	}
    // si paso por todo y encontro todos -> existe
    return 1;
}
