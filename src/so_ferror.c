#include "../include/so_stdio.h"

int so_ferror(SO_FILE *stream)
{
	return stream->error_code;
}
