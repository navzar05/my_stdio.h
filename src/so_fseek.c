#include "../include/so_stdio.h"


int so_fseek(SO_FILE *stream, long offset, int whence)
{
	if (stream->lastOperation == 'w') {
		if (so_fflush(stream) < 0) {
			stream->error_code = 1;
			return SO_EOF;
		}
	} else {
		memset(stream->buffer, 0, BUFSIZE);
		stream->index_buffer = 0;
		stream->readBytes = 0;
	}

	off_t off = lseek(so_fileno(stream), offset, whence);

	if (off < 0) {
		stream->error_code = errno;
		return SO_EOF;
	}
	return 0;
}
