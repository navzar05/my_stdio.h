#include "../include/so_stdio.h"


int so_fgetc(SO_FILE *stream)
{
	if (stream->index_buffer == BUFSIZE || stream->index_buffer == stream->readBytes) {
		memset(stream->buffer, 0, BUFSIZE);
		stream->index_buffer = 0;

		stream->readBytes = read(stream->fd, stream->buffer, BUFSIZE);

		if (stream->readBytes < 0) {
			stream->error_code = 1;
			return SO_EOF;
		}

		if (stream->readBytes == 0) {
			stream->eof = 1;
			return SO_EOF;
		}
	}

	unsigned char result = stream->buffer[stream->index_buffer++];

	stream->lastOperation = 'r';

	return result;
}
