#include "general_func.h"

extern char path_disk[400];
/*
Función para imprimir el bitmap. Cada vez que se llama esta función, imprime 
en stderr el estado actual del disco de acuerdo al contenido del bitmap. 
Debe imprimir una lı́nea con el contenido del bitmap (un 1 por cada bloque 
ocupado y un 0 por cada bloque libre), en la siguiente lı́nea la cantidad de
bloques ocupados, y en una tercera lı́nea la cantidad de bloques libres.
*/

const char *bit_rep[16] = {
    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

void print_byte(uint8_t byte){
    // printf("%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
    fprintf( stderr, "%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
}

int count_byte(uint8_t byte){
    int count_1 = 0;
    char linea[8];
    sprintf(linea, "%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);

    for (int i = 0; i < 8; i++){
        if( linea[i] == '1'){
            count_1++;
        }
    }
    return count_1;
}

// 2048 B = 2KB
// 8b = 1B
// hexdump -C simdiskfilled.bin -n 2048 [permite ver el primer bloque]
// 1 Byte = 2 Hex

// 4 primeros bytes
// 11111000 00000000 00000000 00000000 -> F8000000 en hexa
// deberian ser 65536 bloques
// imprime en stderr
// fprintf( stderr, "tengo acceso en bitmap al path_disk: %s\n", path_disk);

void cr_bitmap(){
    FILE* f = fopen(path_disk, "rb" );
	unsigned char *buffer = malloc( sizeof(unsigned char) * 8 );
    // buffer de 8 Byte
    int free_blocks = 0;
    int occu_blocks = 0; 
	for( int i = 256; i < 1280; i++ ) {    
		fseek( f, 8 * i, SEEK_SET );
		fread( buffer, sizeof( unsigned char ), 8, f );
        for (int j = 0; j < 8; j++){
            uint8_t b = (uint8_t) buffer[j];
            print_byte(b);
            occu_blocks += count_byte(b);
            free_blocks += (8-count_byte(b));
            // printf(" ");
            fprintf( stderr, " ");
            }
        // printf("\n");
        fprintf( stderr, "\n");
	};
    // printf("\n");
    fprintf( stderr, "\n");
    
    fprintf( stderr, "Bloques totales:  %d\n",occu_blocks+free_blocks);
    fprintf( stderr, "Bloques ocupados: %d\n",occu_blocks);
    fprintf( stderr, "Bloques libres:   %d\n",free_blocks);

    // printf("Bloques totales:  %d\n",occu_blocks+free_blocks);
    // printf("Bloques ocupados: %d\n",occu_blocks);
    // printf("Bloques libres:   %d\n",free_blocks);

    fclose(f);
	free( buffer );
}
