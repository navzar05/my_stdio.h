#include "../include/so_stdio.h"

long so_ftell(SO_FILE *stream)
{
	long pos = lseek(so_fileno(stream), 0, SEEK_CUR);

	if (pos == -1)
		return SO_EOF;

	if (stream->lastOperation == 'r')
		pos -= (stream->readBytes - stream->index_buffer);
	else if (stream->lastOperation == 'w')
		pos += stream->index_buffer;

	return pos;
}
