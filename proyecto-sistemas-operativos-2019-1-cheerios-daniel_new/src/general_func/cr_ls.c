#include "general_func.h"

extern char path_disk[400];

unsigned int get_index(unsigned int dir, char *path, FILE* f) {
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

// Esta funcion es tal cual la ayudantia (levemente modificada)
int read_entry( unsigned char *buffer ) {
	if ( buffer[0] == (unsigned char)1 ) { // Si el primer byte es 1 entonces la entrada es invalida
		return 0;
	} else {
		if ( buffer[0] == (unsigned char)2 ) { // Corresponde a un directorio
			printf( "/%s\n", buffer + 1);
			//printf("index: %u\n", (unsigned int)buffer[30] * 256 + (unsigned int)buffer[31]);
		} else if (buffer[0] == (unsigned char)4) { // Corresponde a un archivo
			printf( "%s\n", buffer + 1);
			//printf("index: %u\n", (unsigned int)buffer[30] * 256 + (unsigned int)buffer[31]);
		};
		return 0;
	};
};

void cr_ls(char *path) {
	FILE* f = fopen( path_disk, "rb" );
	unsigned char *buffer = malloc( sizeof( unsigned char ) * 32 );
	if (!strcmp(path, "/") || !strcmp(path, "")) {
		//Estoy en root
		printf("Path: root\n--------------------------\n");
		for( int i = 0; i < 64; i++ ) {
			fseek( f, 32 * i, SEEK_SET );  // Itero sobre cada bloque
			fread( buffer, sizeof( unsigned char ), 32, f ); // Guardo en el buffer el contenido del bloque
			read_entry( buffer ); // Lo leo
		};
	} else {
		unsigned int index = 0;
		// Hare una especie de split del path
		char *dir = malloc(strlen(path));
		strcpy(dir, path);
		char delim[] = "/";
		char *tok = strtok(dir, delim);
		
		while(tok != NULL)
		{
			// Aqui tok corresponde al directorio actual. Debo buscar su index
			index = get_index(index, tok, f);
			// Si index es 0, no existe el directorio
			if (index == 0) {
				break;
			}
			tok = strtok(NULL, delim);
		}
		
		if(index == 0) {
			printf("Directorio no existe\n");
		} else {
			printf("Path: %s\n--------------------------\n", path);
			// Si estoy aqui es porque obtuve el index del directorio pedido
			for(unsigned int i = 0; i < 64; i++ ) {
				fseek( f, 32 * i + index * 2048, SEEK_SET );  // Itero sobre cada bloque
				fread( buffer, sizeof( unsigned char ), 32, f ); // Guardo en el buffer el contenido del bloque
				read_entry( buffer ); // Lo leo
			};
		}
		free(dir);
	}

	free(buffer);
	fclose(f);
}