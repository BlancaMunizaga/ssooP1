#include "file_mang_func.h"

extern char path_disk[400];

int global_written = 0;
int block_written  = 0; // indica cuantos he escrito en el
// bloque actual
int N_block =0;

int num_bloques_dir_ind = 0;
int num_bloques_datos   = 0;

const char *bit_rep_3[16] = {
    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

char* byte4_str(uint32_t byte4){
    char* linea = malloc(sizeof(char)* 32);
    sprintf(linea,"%s%s%s%s%s%s%s%s",\
    bit_rep_3[(byte4 >> 28) & 0x0000000F],\
    bit_rep_3[(byte4 >> 24) & 0x0000000F],\
    bit_rep_3[(byte4 >> 20) & 0x0000000F],\
    bit_rep_3[(byte4 >> 16) & 0x0000000F],\
    bit_rep_3[(byte4 >> 12) & 0x0000000F],\
    bit_rep_3[(byte4 >> 8) & 0x0000000F],\
    bit_rep_3[(byte4 >> 4) & 0x0000000F],\
    bit_rep_3[byte4 & 0x0000000F]);
    // 0x0F = 0000 1111
    // 0x0000000F = 0000 0000 0000 0000 0000 0000 0000 1111
    return linea;
}

// stackOverflow
// https://stackoverflow.com/questions/36793073/how-to-convert-from-a-binary-string-to-binary-unsigned-char
unsigned char stringToBinary(char tmpSubjectStatus[]){
    int tmpInteger = 0;
    while (*tmpSubjectStatus)
    {
        tmpInteger <<= 1;
        if (*tmpSubjectStatus == '1')
            tmpInteger += 1;
        tmpSubjectStatus++;
    }
    return (unsigned char) tmpInteger;
}

void escribir_4byte_from_int_2(int indice_bloque, int where){
    // 100000000000101 = 16389 usa 7 bits
    // 0100000000000101 -> 2 bytes
    // 00000000 00000000 01000000 00000101 
    // 00000000 00000000 01000000 00000101
    // where = 4 * i
    FILE* h = fopen(path_disk, "r+" );    
    fseek( h, where, SEEK_SET ); 
    uint32_t b = (uint32_t) indice_bloque;
    // printf("EN uint32_t %u\n", b);
    // escribir 4 bytes
    char* linea = malloc(32*sizeof(char));
    linea = byte4_str(b);
    // printf("TRANSFORMADO A BIN: %s\n", linea);
    
    char* linea1 = malloc(8*sizeof(char));
    char* linea2 = malloc(8*sizeof(char));
    char* linea3 = malloc(8*sizeof(char));
    char* linea4 = malloc(8*sizeof(char));

    sprintf(linea1, "%c%c%c%c%c%c%c%c", linea[0], linea[1], linea[2], linea[3], linea[4], linea[5], linea[6], linea[7]);
    sprintf(linea2, "%c%c%c%c%c%c%c%c", linea[8], linea[9], linea[10], linea[11], linea[12], linea[13], linea[14], linea[15]);
    sprintf(linea3, "%c%c%c%c%c%c%c%c", linea[16], linea[17], linea[18], linea[19], linea[20], linea[21], linea[22], linea[23]);
    sprintf(linea4, "%c%c%c%c%c%c%c%c", linea[24], linea[25], linea[26], linea[27], linea[28], linea[29], linea[30], linea[31]);

    // printf("LINEA1: %s\n", linea1); 
    // printf("LINEA2: %s\n", linea2); 
    // printf("LINEA3: %s\n", linea3); 
    // printf("LINEA4: %s\n", linea4);                 

    unsigned char byte1 = stringToBinary(linea1);
    unsigned char byte2 = stringToBinary(linea2);
    unsigned char byte3 = stringToBinary(linea3);
    unsigned char byte4 = stringToBinary(linea4);

    fputc( (int) byte1, h);
    fputc( (int) byte2, h);
    fputc( (int) byte3, h);
    fputc( (int) byte4, h);

    fclose(h);

    free(linea);   // STACK
    free(linea1);  // STACK	 
    free(linea2);  // STACK	 
    free(linea3);  // STACK	 
    free(linea4);  // STACK	 
}       

void escribir_4byte_from_int(int indice_bloque, int where){
    // 100000000000101 = 16389 usa 7 bits
    // 0100000000000101 -> 2 bytes
    // 00000000 00000000 01000000 00000101 
    // 00000000 00000000 01000000 00000101
    // where = 4 * i
    FILE* h = fopen(path_disk, "r+" );    
    fseek( h, where, SEEK_SET ); 
    uint32_t b = (uint32_t) indice_bloque;
    // printf("EN uint32_t %u\n", b); 
    // escribir 4 bytes
    char* linea = malloc(32*sizeof(char));
    linea = byte4_str(b);
    // printf("TRANSFORMADO A BIN: %s\n", linea);
    
    char* linea1 = malloc(8*sizeof(char));
    char* linea2 = malloc(8*sizeof(char));
    char* linea3 = malloc(8*sizeof(char));
    char* linea4 = malloc(8*sizeof(char));

    sprintf(linea1, "%c%c%c%c%c%c%c%c", linea[0], linea[1], linea[2], linea[3], linea[4], linea[5], linea[6], linea[7]);
    sprintf(linea2, "%c%c%c%c%c%c%c%c", linea[8], linea[9], linea[10], linea[11], linea[12], linea[13], linea[14], linea[15]);
    sprintf(linea3, "%c%c%c%c%c%c%c%c", linea[16], linea[17], linea[18], linea[19], linea[20], linea[21], linea[22], linea[23]);
    sprintf(linea4, "%c%c%c%c%c%c%c%c", linea[24], linea[25], linea[26], linea[27], linea[28], linea[29], linea[30], linea[31]);

    // printf("LINEA1: %s\n", linea1); 
    // printf("LINEA2: %s\n", linea2); 
    // printf("LINEA3: %s\n", linea3); 
    // printf("LINEA4: %s\n", linea4);                 

    unsigned char byte1 = stringToBinary(linea1);
    unsigned char byte2 = stringToBinary(linea2);
    unsigned char byte3 = stringToBinary(linea3);
    unsigned char byte4 = stringToBinary(linea4);

    fputc( (int) byte1, h);
    fputc( (int) byte2, h);
    fputc( (int) byte3, h);
    fputc( (int) byte4, h);

    fclose(h);

    free(linea);   // STACK
    free(linea1);  // STACK	 
    free(linea2);  // STACK	 
    free(linea3);  // STACK	 
    free(linea4);  // STACK	 
}       


int byte_pos(uint8_t byte){
    char linea[8];
    sprintf(linea, "%s%s", bit_rep_3[byte >> 4], bit_rep_3[byte & 0x0F]);

    for (int i = 0; i < 8; i++){
        if( linea[i] == '0'){
            return i;
        }
    }
    return 9;  // 9 marca que no encontro
}

char* change_bit_in_byte(uint8_t byte, int pos, char* linea){
    sprintf(linea, "%s%s", bit_rep_3[byte >> 4], bit_rep_3[byte & 0x0F]);
    linea[pos] = '1';
    return linea;
}

uint32_t transform_8buffer_32(uint8_t a1,uint8_t a2, uint8_t a3, uint8_t a4){
    // EIMPORTANTEIMPORTANTEIMPORTANTEIMPORTANTEIMPORTANTE [8913664], [0]
    // uint32_t i32 = a1 | (a2 << 8) | (a3 << 16) | (a4 << 24);

    // o es esta la correcta

    uint32_t i32 = a4 | (a3 << 8) | (a2 << 16) | (a1 << 24);
    return i32;
}

int search_free_space(){
    // busca un bloque libre para transformarlo en uno de datos
    // tengo que actualizar el bitmap cuando creo
    // el bloque de datos
    FILE* f = fopen(path_disk, "rb" );
	unsigned char *buffer = malloc( sizeof(unsigned char) * 8 );
    // buffer de 8 Byte
	for( int i = 256; i < 1280; i++ ) {    
		fseek( f, 8 * i, SEEK_SET );
		fread( buffer, sizeof( unsigned char ), 8, f );
        for (int j = 0; j < 8; j++){
            uint8_t b = (uint8_t) buffer[j];
            // tenemos 1 bytes es decir 8 bits
            // necesitamos saber si en esos 8 bits hay un 0
            // y si lo hay donde y cambiarlo por 1
            int pos = byte_pos(b);
            if (pos<9){
                fclose(f);
                // 256*8 = 2048 Bytes * 8 = bits

                // ESCRIBIR actualizar bitmap

                // FILE* h = fopen(path_disk, "wb" );
                // FILE* h = fopen(path_disk, "w+" );
                FILE* h = fopen(path_disk, "r+" );
                int posicion = 8*i+j;

                char* linea = malloc(8*sizeof(char));
                linea = change_bit_in_byte(b, pos, linea);
                unsigned char imp = stringToBinary(linea);

                // printf("VIEJA LINEA: %u\n", buffer[j]);
                // printf("NUEVA LINEA: %s\n", linea);
                // printf("NUEVA LINEA TRANSFORMADA: %u\n", imp);

                // printf("POSICION %d\n", posicion);
                // MUERE AQUI OJO
                fseek( h, posicion, SEEK_SET );
                fputc( (int) imp, h);
                
                fclose(h);
                
                // print_binary(2048/8, (2048+128)/8);
                
                // int indice_bloque = ((posicion*2048) + pos);
                // int indice_bloque = (((((i-256)*8)+j)*2048)+pos);
                // int indice_bloque = (((((i-256)*8)+j+pos)*2048));

                // printf("i:   %d\n",i);
                // printf("j:   %d\n",j);
                // printf("pos: %d\n",pos);
                int indice_bloque = (((((i-256+j)*8)+pos)*2048));
                
                free(linea); // STACK
                free(buffer);
                
                return indice_bloque; // pos en bits  
                }
            }
        }
    return 0;
}

int actualizar_bloque(crFILE* file_desc){
    // se encarga de decirme donde hay un bloque para escribir
    // actualiza la info del index block
    // si no quedan bloques retorna 0

    // cada puntero es de 4 bytes
    FILE* f = fopen(path_disk, "rb" );    
	unsigned char *buffer = malloc( sizeof(unsigned char) * 4 );

    // buscamos un espacio para direccionar dentro del bloque indice
    // 2000 bytes
    int inicio = (file_desc->p_index/4) + 2;
    // leo del bloque indice la parte de direccionamiento directo
	for( int i = inicio; i < inicio + 500; i++ ) {
		fseek( f, 4 * i, SEEK_SET );
		fread( buffer, sizeof( unsigned char ), 4, f );

        int es_cero = 1;
        for (int j = 0; j < 4; j++){
            uint8_t c = (uint8_t) buffer[j];
            if (c){
                es_cero=0;
            }
        }


        if (es_cero){
            // printf("Hay espacio en direccionamiento directo\n");
            // debo buscar un bloque libre para que sea
            // el bloque de datos
            // printf("MURIO EN EL SEARCH\n");
            int indice_bloque = search_free_space();
            // printf("NO MURIO EN EL SEARCH\n");
            // en cr_write nos ocuparemos de actualizar los
            // bytes escritos en el bloque indice
            if (indice_bloque){
                // printf("Hay espacio nuevo bloque de datos [%d]\n", 4 * i);    
                fclose(f);
                num_bloques_datos++;
                // ESCRIBIR el puntero a la dir encontrada en la parte de
                // direccionamiento directo
                escribir_4byte_from_int_2(indice_bloque, 4*i);

                // prints para ver que sucede
                // printf("numero Bloque escrito: %d\n", N_block);
                // printf("Indice Bloque: %d\n", indice_bloque);                
                // print_binary(((2048*40)-32)/8, ((2048*40)+150)/8);
                // print_binary(((4*i)-32)/8, ((i*4)+150)/8);
                // printf("I: %d\n", i*4);

                free(buffer); // STACK                             
                return indice_bloque;
            }
        }
    };
    
    // 40 bytes de punteros a bloques de dir indirecto
    // printf("probando con indirecto\n");
    // for( int i = inicio + 502; i < inicio + 512; i++ ) {
    for( int i = inicio + 500; i < inicio + 510; i++ ) { 

		fseek( f, 4 * i, SEEK_SET );
		fread( buffer, sizeof( unsigned char ), 4, f );

        int es_cero = 1;
        for (int j = 0; j < 4; j++){
            uint8_t c = (uint8_t) buffer[j];
            if (c){
                es_cero=0;
            }
        }
        uint32_t b = transform_8buffer_32((uint8_t)buffer[0], (uint8_t)buffer[1],(uint8_t)buffer[2], (uint8_t)buffer[3]);

        // print_binary(((4*i)-32)/8, ((i*4)+150)/8);

        if (!es_cero){ // si encuentro un puntero a un bloque de dir. indirecto
            // printf("buscando espacio dentro de bloque de direccionamiento indirecto\n");
            // si entre -> b es un puntero a un bloque de dir indirecto
            // debo revisar si este bloque tiene espacio para otro puntero
            // a un bloque de datos
            for( int j = (int) b; j < ((int)b) + 512; j++ ) {    
                fseek( f, 4 * j, SEEK_SET );
		        fread( buffer, sizeof( unsigned char ), 4, f );

                int es_cero_2 = 1;
                for (int j = 0; j < 4; j++){
                    uint8_t c = (uint8_t) buffer[j];
                    if (c){
                        es_cero_2=0;
                    }
                }

                if (es_cero_2){
                    // printf("hay espacio en este bloque de dir. indirecto\n");
                    // hay espacio para otro puntero de datos
                    int indice_bloque = search_free_space();
                    if (indice_bloque){
                        // printf("hay espacio para nuevo bloque de datos\n");
                        num_bloques_datos++;
                        // si encontre un lugar para un bloque de datos
                        // ESCRIBIR el puntero a la dir encontrada en el bloque
                        // de direccionamiento indirecto
                        // printf("Indice Bloque: %d\n", indice_bloque);
                        fclose(f);
                        escribir_4byte_from_int(indice_bloque, 4*j);     
                        return indice_bloque;
                    }
                }
            }
        }
        
        else{ // si buscando me encuentro uno vacio
            // quiere decir que los anteriores estaban llenos
            // printf("Hay espacio para nuevo puntero a un bloque de direccionamiento indirecto\n");
            //ahora debemos buscar un espacio vacio para 
            //un bloque de dir indirecto guardar su direccion
            //luego buscar un espacio vacio para un bloque de datos que va a 
            //guardar su direccion en el nuevo bloque de dir indirecto
            int indice_bloque_indirect = search_free_space();
            // ESCRIBIR el puntero a la dir encontrada en la parte
            // de direccionamiento indirecto
            if (indice_bloque_indirect){
                // printf("Hay espacio para nuevo bloque\n");

                // debo escribir esta direccion en el bloque indice
                escribir_4byte_from_int_2(indice_bloque_indirect, 4*i);
                num_bloques_dir_ind ++;

                for( int j = indice_bloque_indirect; j<indice_bloque_indirect\
                 +512; j++ ) {    
                    fseek( f, 4 * j, SEEK_SET );
                    fread( buffer, sizeof( unsigned char ), 4, f );
                    
                    int es_cero_3 = 1;
                    for (int j = 0; j < 4; j++){
                        uint8_t c = (uint8_t) buffer[j];
                        if (c){
                            es_cero_3=0;
                        }
                    }
                    // No parece ser realmente necesario revisar si esta vacio
                    // si lo acabo de crear pero de igual manera lo dejo

                    if (es_cero_3){
                        // printf("Hay espacio para nuevo puntero a datos\n");
                        // espacio para otro puntero de datos
                        int indice_bloque = search_free_space();
                        if (indice_bloque){
                            // printf("Hay espacio para nuevo bloque de datos\n");
                            num_bloques_datos++;
                            // si encontre un lugar para un bloque de datos
                            // ESCRIBIR el puntero a la dir encontrada en el bloque
                            // de direccionamiento indirecto
                            // printf("Indice Bloque: %d\n", indice_bloque);
                            fclose(f);
                            escribir_4byte_from_int(indice_bloque, 4*j);       
                            return indice_bloque;
                        }
                    }
                }
            }
        }
	};
    
    fclose(f);
	free( buffer );
    return 0;  // si no funciono nada -> se ha alcanzado el limite de archivo
}

int cr_write(crFILE* file_desc, void* buffer, int nbytes){
    // printf("bytes rest: %d\n", nbytes);
    // printf("Pointer:    %p\n", &buffer);
    unsigned char *p =(unsigned char *) buffer;
    int written = 0;
    int size_f = 8;  // amount of bytes to be wroten in each iteration
    int bytes_rest = nbytes;  // bytes restantes

    if (!file_desc->mode){
        fprintf( stderr, "Error: operation not allowed\n");
        return 0;}
    
    if (block_written%2048==0){
        N_block ++;
        // printf("MURIO AQUI [block_written %d]\n",block_written);
        file_desc->p_pos = actualizar_bloque(file_desc);
        // printf("NO MURIO AQUI\n");
    }
    if (!file_desc->p_pos){
        printf("Error: disk full you can't write more\n");
        return written; // no se puede seguir escribiendo
    }
    
    else{
        // printf("MURIO AQUI 1\n");
        FILE* h = fopen(path_disk, "r+" );
        // printf("MURIO AQUI 2\n");
        fseek( h, file_desc->p_pos, SEEK_SET );
        // printf("MURIO AQUI 3\n");
        for (int i = 0; i < size_f; i++){
            // p[i];  // es un byte
            // ESCRIBIR los bytes en la posicion dada
            // printf("%c\n", ((char *) p) [i]);
            // printf("%x\n", p[i]);
            // printf("%u\n", p[i]);
            // printf("\n");
            
            fputc( (int) p[i], h);
            
            written++;
            block_written ++;
            bytes_rest --;
        }
        // printf("MURIO AQUI 4\n");
        fclose(h);
        // printf("MURIO AQUI 5\n");
        file_desc->p_pos = file_desc->p_pos+size_f; // actualizo el puntero
        // printf("MURIO AQUI 6 [%d]{%d}\n",file_desc->p_pos, bytes_rest);
    }

    if (bytes_rest > 0){
        // MUERE PORQUE ESTO SE LLAMA MUCHAS VECES OJO
        written += cr_write(file_desc, &p[written], bytes_rest);
    }

    // antes de devolver a written tengo que actualizar en bloque indice
    // los bytes escritos en sus bloques almacenados

    // ESCRIBIR los bytes escritos en el bloque indice

    // 4 Bytes al inicio del bloque para el tamaño del archivo.
    int inicio = (file_desc->p_index);
    // printf("DIR BLOQUE INDICE: %d\n",inicio);
    escribir_4byte_from_int(written, inicio);       
    // 81920
    // printf("NUM. Bloques[datos] escritos: %d\nNUM. Bloques dir ind. usados: %d\n", num_bloques_datos, num_bloques_dir_ind);
    return written;
}






/*

-------------------------------------------------------------------
Pruebas escritura de 8.9 MB [Libro]

......................................................
Prueba funcion transform_8buffer_32:

con el de arriba [dir: 8.913.664]
Bloques totales:  65536
Bloques ocupados: 4357 
Bloques libres:   61179

Con el de abajo [dir: 1.048.576]  // tiene mas sentido
Bloques totales:  65536
Bloques ocupados: 4357
Bloques libres:   61179
......................................................

4357      *2048 = 8.912.896 bits 

num de bloques equivalente al 6.648254395%
que de 134.2 MB es 8.921957398 MB
esta bien

se usaron 8 Bloques de dir ind.

se escribieron:   8896512 Bytes [toma en cuenta el bloque indice y de dir ind.]

-------------------------------------------------------------------
PROBLEMAS:

-------------------------------------------------------------------
NOTAS
- Cr_open debe actualizar bitmap con el bloque indice y crearlo OJO
- se uso el simdiskformat
-------------------------------------------------------------------

*/
