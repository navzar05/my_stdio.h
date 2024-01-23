#include "../include/so_stdio.h"


size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	if (stream->lastOperation == 'w' && stream->mode[1] == '+') {
		if (so_fflush(stream) < 0) {
			stream->error_code = 1;
			return SO_EOF;
		}
	}

	size_t totalSize = size * nmemb;
	size_t totalSizeRead = 0;
	int AuxByte = 0;

	for (size_t i = 0; i < totalSize; i++) {
		AuxByte = so_fgetc(stream);

		if (AuxByte == SO_EOF)
			if (stream->error_code != 0 || stream->eof)
				break;

		((unsigned char *)ptr)[i] = AuxByte;
		totalSizeRead++;
	}

	stream->lastOperation = 'r';
	return totalSizeRead / size;
}
