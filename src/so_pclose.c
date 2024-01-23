#include "../include/so_stdio.h"

int so_pclose(SO_FILE *stream)
{
	if (stream == NULL)
		return -1;

	so_fflush(stream);
	close(stream->fd);

	int status = 0;
	int ret = waitpid(stream->childPid, &status, 0);

	free(stream);

	if (ret < 0)
		return -1;

	return WEXITSTATUS(status);
}
