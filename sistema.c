#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

static int do_getattr( const char *path, struct stat *st )
{

	printf( "\t Leyendo los atributos de: \n", path );

	st->st_uid = getuid(); //Obtenemos el usuario que esta montando
	st->st_gid = getgid(); //Obtenemos el grupo al que pertenece
	st->st_atime = time( NULL ); //Ultimo acceso al archivo en cuestion
	st->st_mtime = time( NULL ); //Ultima modificacion del archivo

	if ( strcmp( path, "/" ) == 0 )
	{
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2;
	}
	else
	{
		st->st_mode = S_IFREG | 0644; //Verificamos si es directorio, archivo u otro
		st->st_nlink = 1; //Numero de links duros
		st->st_size = 1024; //Longitud del archivo en bytes
	}
	return 0;
}

/**
 * Funcion que lee los archivos contenidos en el directorio en cuestion
 */
static int do_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi )
{
	printf( "Obteniendo lista del directorio: %s\n", path );

	filler( buffer, ".", NULL, 0 ); // Llena el buffer a llenar con los nombres de los archivos del directorio actual
	filler( buffer, "..", NULL, 0 ); // Llena el buffer a llenar con los nombres de los archivos del directorio padre

	return 0;
}

//Agregar aqui correspondencias entre las funciones de FUSE
// y las implementadas
static struct fuse_operations operations = {
    .getattr	= do_getattr,
    .readdir	= do_readdir,
};

int main( int argc, char *argv[] )
{
	return fuse_main( argc, argv, &operations, NULL );
}
