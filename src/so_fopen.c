#include "../include/so_stdio.h"

SO_FILE *so_fopen(const char *pathname, const char *mode)
{

	if (mode[0] != 'r' && mode[0] != 'w' && mode[0] != 'a' && mode[1] != '+' && mode[1] != '\0')
		return NULL;


	SO_FILE *fp = (SO_FILE *)calloc(1, sizeof(SO_FILE));

	if (fp == NULL)
		return NULL;


	memcpy(fp->mode, mode, 2);
	memset(fp->buffer, 0, BUFSIZE);
	fp->index_buffer = 0;
	fp->childPid = 0;
	fp->lastOperation = 0;
	fp->eof = 0;
	fp->error_code = 0;

	switch (mode[0]) {
	case 'r':
		if (fp->mode[1] == '+')
			fp->fd = open(pathname, O_RDWR);
		else
			fp->fd = open(pathname, O_RDONLY);
		break;
	case 'w':
		if (fp->mode[1] == '+')
			fp->fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC, 0600);
		else
			fp->fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		break;
	case 'a':
		if (fp->mode[1] == '+')
			fp->fd = open(pathname, O_APPEND | O_RDWR | O_CREAT, 0600);
		else
			fp->fd = open(pathname, O_APPEND | O_WRONLY | O_CREAT, 0600);
		break;
	default:
		fp->error_code = errno;
		return NULL;

	}
	if (fp->fd < 0) {
		free(fp);
		return NULL;
	}


	return fp;
}
