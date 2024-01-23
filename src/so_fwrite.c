#include "../include/so_stdio.h"

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	if (stream->lastOperation == 'r' && stream->mode[1] == '+') {
		stream->index_buffer = 0;
		stream->readBytes = 0;
	}

	size_t totalSize = size * nmemb;
	size_t totalWrittenBytes = 0;
	unsigned char byteToWrite = 0;

	for (size_t i = 0; i < totalSize; i++) {
		byteToWrite = ((unsigned char *)ptr)[i];
		if (so_fputc(byteToWrite, stream) == SO_EOF) {
			stream->error_code = 1;
			break;
		}
		totalWrittenBytes++;
	}

	stream->lastOperation = 'w';
	return totalWrittenBytes / size;
}
