#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include "utils.h"

int isdir( const char *path )
{
	DIR *dir = opendir(path);
	if( dir ) { closedir( dir ); return 1; }
	return 0;
}

int fexists( const char *path )
{
	FILE *f = fopen( path, "r" );
	if( !f ) return 0;
	fclose( f );
	return 1;
}

void *fload( const char *path, size_t *fsize )
{
	FILE *f = fopen( path, "r" );
	if( !f ) return NULL;

	fseek( f, 0L, SEEK_END );
	size_t sz = ftell( f );
	fseek( f, 0L, SEEK_SET );
	if( fsize ) *fsize = sz;

	char *buf = malloc( sz + 1 );
	if( !buf ) { fclose( f ); return NULL; }

	fread( buf, sz, 1, f );
	fclose( f );
	return buf;
}

// COM_ParseFile from Xash
char *strparse( char *data, char *token )
{
	int	c, len;

	if( !token )
		return NULL;

	len = 0;
	token[0] = 0;

	if( !data )
		return NULL;
// skip whitespace
skipwhite:
	while(( c = ((char)*data)) <= ' ' )
	{
		if( c == 0 )
			return NULL;	// end of file;
		data++;
	}

	// skip // comments
	if( c=='/' && data[1] == '/' )
	{
		while( *data && *data != '\n' )
			data++;
		goto skipwhite;
	}

	// handle quoted strings specially
	if( c == '\"' )
	{
		data++;
		while( 1 )
		{
			c = (char)*data;

			// unexpected line end
			if( !c )
			{
				token[len] = 0;
				return data;
			}
			data++;

			if( c == '\\' && *data == '"' )
			{
				token[len++] = *data++;
				continue;
			}

			if( c == '\"' )
			{
				token[len] = 0;
				return data;
			}
			token[len] = c;
			len++;
		}
	}

	// parse single characters
	if( c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ',' )
	{
		token[len] = c;
		len++;
		token[len] = 0;
		return data + 1;
	}

	// parse a regular word
	do
	{
		token[len] = c;
		data++;
		len++;
		c = ((char)*data);

		if( c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ',' )
			break;
	} while( c > 32 );

	token[len] = 0;

	return data;
}

int get_info_key( char *data, char *key, char *out, uint32_t outlen )
{
	char *p = data;
	char token[256] = { 0 };
	while( p = strparse( p, token ) )
	{
		if( !strncasecmp( token, key, sizeof(token) ) )
		{
			strparse( p, token );
			strncpy( out, token, outlen );
			return 1;
		}
	}
	return 0;
}
