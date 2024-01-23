#include "../include/so_stdio.h"

int so_fclose(SO_FILE *stream)
{

	int ret = 0;

	if (stream->mode[0] == 'w' || stream->mode[0] == 'a' ||
	stream->mode[1] == '+')
		if (so_fflush(stream) < 0) {
			free(stream);
			return SO_EOF;
		}

	ret = close(so_fileno(stream));
	free(stream);
	if (ret < 0)
		return SO_EOF;
	return 0;
}
