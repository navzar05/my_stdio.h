#include "../include/so_stdio.h"
#include <sys/ioctl.h>

int so_feof(SO_FILE *stream)
{
	return stream->eof;
}
