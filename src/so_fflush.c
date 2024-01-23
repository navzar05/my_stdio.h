#include "../include/so_stdio.h"

int so_fflush(SO_FILE *stream)
{
	if (stream->lastOperation != 'w')
		return 0;

	size_t totalWritten = 0;

	while (totalWritten < stream->index_buffer) {
		ssize_t written = write(stream->fd, stream->buffer + totalWritten, stream->index_buffer - totalWritten);

		if (written < 0) {
			stream->error_code = 1;
			return SO_EOF;
		}
		totalWritten += written;
	}

	memset(stream->buffer, 0, sizeof(BUFSIZE));
	stream->index_buffer = 0;


	return 0;
}
