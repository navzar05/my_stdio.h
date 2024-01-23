#include "../include/so_stdio.h"

int so_fputc(int c, SO_FILE *stream)
{
	if (stream->index_buffer == BUFSIZE) {
		if (so_fflush(stream) == SO_EOF) {
			stream->error_code = 1;
			return SO_EOF;
		}
	}

	stream->buffer[stream->index_buffer++] = (unsigned char)c;
	stream->lastOperation = 'w';
	return c;
}
