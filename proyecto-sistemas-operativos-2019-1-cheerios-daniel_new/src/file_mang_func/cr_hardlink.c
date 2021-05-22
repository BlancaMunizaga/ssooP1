#include "file_mang_func.h"
#include <stdio.h>
#include <stdlib.h>


extern char path_disk[400];


uint8_t thisIs(char *path){
    FILE* f = fopen(path_disk, "rb" );
	unsigned char *buffer = malloc( sizeof(unsigned char) * 32 );
    char* linea = malloc(sizeof(char)*27);
    // cada char son 8 bits = 1 byte
    // buffer de 32 Byte
    int *begin;
    unsigned char invalid    = 0x01;
    unsigned char valid_dir  = 0x02;
    unsigned char valid_file = 0x04;
    int index = 1;

    (*begin) = (*begin)*64; // begin es el bloque
	for( int i = (*begin); i < (*begin)+64; i++ ) {
	    fseek( f, 32 * i, SEEK_SET );
	    fread( buffer, sizeof( unsigned char ), 32, f );
        uint8_t validation = (uint8_t) buffer[0];
        for (int j = 1; j < 28; j++){
            // name
            linea[j-1] = (char) buffer[j];
        }
        // printf("Nombre: %s\n", linea);
        if (strcmp(linea, path) == 0){
            free(buffer);
            free(linea);
            return validation;
        }
    }
    free(linea);
    free(buffer);
    return (uint8_t)0;
}


int cr_hardlink(char *orig, char *dest){
    /*
    Crea un hardlink del archivo orig en dest.
    Aumenta la cantidad de referencias al archivo orig.
    Agregar entrada de dest fclose(f);         // Cerramos el disco
    */

   FILE *f = fopen(path_disk, "r+");
   int o_exists = cr_exists(orig);
   int d_exists = cr_exists(dest);
   uint8_t o_type = thisIs(orig);
   uint8_t d_type = thisIs(dest);
   if(o_exists && d_exists && o_type == 0x04 && d_type == 0x02){
       unsigned char *buffer = malloc(sizeof(unsigned char) * 4);
       int *o_idx = 0;
       int *d_idx = 0;
       search(o_idx, orig);
       search(d_idx, dest);
       fseek(f, (long)*o_idx + 4, SEEK_SET);  // Estoy parado en los bytes de cantidad de hardlinks
       fread(buffer, sizeof(unsigned char), 4, f);
       unsigned int countHL = (unsigned int) *buffer;
       countHL++;
       char update[4];
       sprintf(update, "%x", countHL);
       fseek(f, (long)*o_idx + 4, SEEK_SET);  // Estoy parado en los bytes de cantidad de hardlinks
       fwrite(update, 1, 4, f);
       for( int i = *d_idx; i < *d_idx+64; i++) {
	        fseek( f, 32 * i, SEEK_SET );
	        fread( buffer, sizeof( unsigned char ), 32, f );
            uint8_t validation = (uint8_t) buffer[0];
            if(validation == 0x03 || validation > 0x04){  // si es una entrada vacía
                fseek(f, 32*(i-1), SEEK_SET);
                fputc(o_type, f);  // Es un archivo
                
                // Creando hardlink en directorio
                char *token = strtok(orig, "/");
                char *aux;
                while(token !=NULL){
                    *aux = *token;
                    token = strtok(NULL, "/");
                }
                strcat(dest, "/");
                strcat(dest, aux);
                for(int i=0; i<27; i++){
                    fputc((unsigned char)*(dest + i), f);
                }
                char index[4];
                unsigned long numb = (unsigned long) o_idx;  // Escribimos idx en tabla
                fputc((numb >>24)&0xFF, f);
                fputc((numb >>16)&0xFF, f);
                fputc((numb >>8)&0xFF, f);
                fputc(numb&0xFF, f);
            }
        }
       free(buffer);
   }
   else{
       printf("ERROR[cr_hardlink]: origen o destino no existen en Disco");
       fclose(f);
       return 0;
   }
   fclose(f);
   return 0;
}
