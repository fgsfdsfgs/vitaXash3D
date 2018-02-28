#pragma once

#include <stdlib.h>
#include <stdint.h>

int isdir( const char *path );
void *fload( const char *path, size_t *fsize );
char *strparse( char *data, char *token );
int get_info_key( char *data, char *key, char *out, uint32_t outlen );
